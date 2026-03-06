#include "UCT.h"
#include "Game.h"
using namespace std;

UCT::UCT(const int _M, const int _N, const int *_top, int **_board, const int _lastX, const int _lastY, const int _noX, const int _noY)
{
    start_time = (clock() + 0.0) / CLOCKS_PER_SEC;
    
    Game::M = _M;
    Game::N = _N;
    Game::noX = _noX;
    Game::noY = _noY;


    if (Game::new_game)
    {
        Game::new_game = false;
        Game::board = new int *[Game::M];
        Game::top = new int[Game::N];
        for (int i = 0; i < Game::M; ++i)
        {
            Game::board[i] = new int[Game::N];
        }
    }
    if (Node::new_pool)
    {
        Node::new_pool = false;
        Node::pool = new Node *[POOL_SIZE];
    }
    for (int i = 0; i < Game::M; ++i)
    {
        memcpy(Game::board[i], _board[i], Game::N * sizeof(int));
    }
    memcpy(Game::top, _top, Game::N * sizeof(int));
}


Node *UCT::uctSearch()
{
    root = new Node(-1, -1, USER_TURN, nullptr);
    Node::used_nodes = 0;
    if (root->firstTest())
    {
        return root;
    }

    int origin_board[Game::M][Game::N];
    int origin_top[Game::N];
    for (int i = 0; i < Game::M; ++i)
    {
        memcpy(origin_board[i], Game::board[i], Game::N * sizeof(int));
    }
    memcpy(origin_top, Game::top, Game::N * sizeof(int));

    while ((clock() + 0.0) / CLOCKS_PER_SEC - this->start_time < TIME_LIMIT)
    {
        for (int i = 0; i < Game::M; ++i)
        {
            memcpy(Game::board[i], origin_board[i], Game::N * sizeof(int));
        }
        memcpy(Game::top, origin_top, Game::N * sizeof(int));
        Node *node = treePolicy(root);

        double d_profit = 0;
        d_profit = defaultPolicy(node);
        backUp(node, d_profit);
    }
    return bestChild(root);
}


Node *UCT::treePolicy(Node *v)
{
    while ((!v->isEnd()))
    {
        if (v->canExpand())
        {
            return expand(v);
        }
        else
        {
            v = bestChild(v);
            Game::board[v->s_x][v->s_y] = v->turn;
            --Game::top[v->s_y];
            if (v->s_x - 1 == Game::noX && v->s_y == Game::noY)
                --Game::top[v->s_y];
        }
    }
    return v;
}


Node *UCT::expand(Node *v)
{
    // 复制状态
    // 转换棋权
    int sw_turn = nextTurn(v->turn);
    // 下棋
    int index = rand() % v->expand_node_rest;
    int sy = v->emp[index];
    --Game::top[sy];
    int sx = Game::top[sy];
    if (sx - 1 == Game::noX && sy == Game::noY)
        --Game::top[sy];
    Game::board[sx][sy] = sw_turn;
    // 生成节点
    v->children[sy] = Node::newNode(sx, sy, sw_turn, v);
    swap(v->emp[index], v->emp[--v->expand_node_rest]);
    return v->children[sy];
}


Node *UCT::bestChild(Node *v)
{
    int index = -1;
    double max_profit = -RAND_MAX;
    for (int i = 0; i < Game::N; ++i)
    {
        if (!v->children[i])
            continue;

        double raw_profit = 0;
        double Nv = v->children[i]->visit_times;
        double Qv = v->children[i]->profit;

        raw_profit = Qv / Nv + sqrt(2 * log(v->visit_times) / Nv) * UCT_C;

        if (raw_profit > max_profit)
        {
            index = i;
            max_profit = raw_profit;
        }
    }
    return v->children[index];
}


int UCT::defaultPolicy(Node *v)
{
    int turn = v->turn;
    int state = 0;
    int sx = v->s_x;
    int sy = v->s_y;
    while (1)
    {
        state = profit(sx, sy);
        if (state != NOT_END)
            break;
        turn = nextTurn(turn);
        move(turn, &sx, &sy);
    }
    if (v->turn == USER_TURN)
    {
        state = -state;
    }
    return state;
}


void UCT::backUp(Node *v, int delta)
{
    // 回溯更新收益值
    Node *now = v;
    while (now)
    {
        ++(now->visit_times);
        now->profit += delta;
        delta = 0 - delta;
        now = now->parent;
    }
}


int UCT::profit(int x, int y)
{
    if (userWin(x, y, Game::M, Game::N, Game::board))
    {
        return PROFIT_LOSE;
    }
    if (machineWin(x, y, Game::M, Game::N, Game::board))
    {
        return PROFIT_WIN;
    }
    if (isTie(Game::N, Game::top))
    {
        return PROFIT_TIE;
    }
    return NOT_END;
}


void UCT::move(int turn, int *sx, int *sy)
{
    int y = rand() % Game::N;
    while (Game::top[y] == 0)
    {
        y = rand() % Game::N;
    }
    --Game::top[y];
    int x = Game::top[y];
    Game::board[x][y] = turn;
    if (x - 1 == Game::noX && y == Game::noY)
    {
        --Game::top[y];
    }
    *sx = x;
    *sy = y;
}


int UCT::nextTurn(int originturn)
{
    if (originturn == USER_TURN)
        return MACHINE_TURN;
    return USER_TURN;
}


UCT::~UCT()
{
    root->clear();
    delete root;
}
