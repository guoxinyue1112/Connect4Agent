#include "UCTNode.h"
#include "Game.h"
using namespace std;

unsigned int Node::used_nodes = 0;
Node **Node::pool = nullptr;
bool Node::new_pool = true;

Node::Node(int s_x, int s_y, int turn, Node *parent)
{
    children = new Node *[Game::N];
    emp = new int[Game::N];
    fill(s_x, s_y, turn, parent);
}

bool Node::isEnd()
{
    if (is_end != -1)
    {
        return is_end;
    }
    if (s_x == -1 && s_y == -1)
    { 
        // 根节点非终止
        is_end = 0;
        return is_end;
    }
    if (userWin(s_x, s_y, Game::M, Game::N, Game::board) || machineWin(s_x, s_y, Game::M, Game::N, Game::board) || isTie(Game::N, Game::top))
    {
        is_end = 1;
        return is_end;
    }
    is_end = 0;
    return is_end;
}

bool Node::canExpand()
{
    if (this->expand_node_rest > 0)
        return true;
    return false;
}

void Node::fill(int sx, int sy, int turn, Node *parent)
{
    visit_times = 0;
    profit = 0;
    s_x = sx;
    expand_node_rest = 0;
    is_end = -1;
    s_y = sy;
    this->turn = turn;
    this->parent = parent;
    for (int i = 0; i < Game::N; ++i)
    {
        if (Game::top[i])
        {
            emp[expand_node_rest] = i;
            ++expand_node_rest;
        }
        children[i] = nullptr;
    }
}

Node *Node::newNode(int sx, int sy, int turn, Node *parent)
{
    if (used_nodes >= POOL_SIZE)
    {
        return new Node(sx, sy, turn, parent);
    }
    else if (!pool[used_nodes])
    {
        pool[used_nodes++] = new Node(sx, sy, turn, parent);
        return pool[used_nodes - 1];
    }
    pool[used_nodes++]->fill(sx, sy, turn, parent);
    return pool[used_nodes - 1];
}


void Node::clear()
{
    for (int i = 0; i < Game::N; ++i)
    {
        if (children[i])
        {
            children[i] = nullptr;
        }
    }
    delete[] children;
    delete[] emp;
}

bool Node::firstTest()
{
    for (int i = 0; i < Game::N; ++i)
    {
        if (Game::top[i])
        {
            Game::board[Game::top[i] - 1][i] = MACHINE_TURN;
            if (machineWin(Game::top[i] - 1, i, Game::M, Game::N, Game::board))
            {
                this->s_x = Game::top[i] - 1;
                this->s_y = i;
                // 下必胜点
                return true;
            }
            Game::board[Game::top[i] - 1][i] = 0;
        }
    }
    for (int i = 0; i < Game::N; ++i)
    {
        if (Game::top[i])
        {
            Game::board[Game::top[i] - 1][i] = USER_TURN;
            if (userWin(Game::top[i] - 1, i, Game::M, Game::N, Game::board))
            {
                this->s_x = Game::top[i] - 1;
                this->s_y = i;
                // 堵必败点
                return true;
            }
            Game::board[Game::top[i] - 1][i] = 0;
        }
    }
    return false;
}
