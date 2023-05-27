#include "UCT.h"
#include "Game.h"
using namespace std;

    
    UCTree::UCTree(const int _M, const int _N, const int* _top, int** _board, const int lastX, const int lastY, const int _noX, const int _noY)
    {
        //TODO
        //cout << "constructing a ucttree" <<endl;
        Game::M = _M;
        Game::N = _N;
        Game::noX = _noX;
        Game::noY = _noY;
        //cout << Game::M<<' '<<Game::N << ' '<<endl;
        start_time = (clock()+0.0)/CLOCKS_PER_SEC;
        //cout << "got start_time" << endl;
        if (Game::board_is_new) {
            Game::board_is_new = false;
            Game::board = new int* [Game::M];
            Game::top = new int [Game::N];
            for(int i =0;i<Game::M;++i){
                Game::board[i] = new int [Game::N];
            }
        }
        if(UCTNode::new_pool){
            UCTNode::new_pool = false;
            UCTNode::pool = new UCTNode* [MAX_POOL];
        }
        for(int i =0;i<Game::M;++i){
            memcpy(Game::board[i],_board[i],Game::N*sizeof(int));
        }
        memcpy(Game::top,_top,Game::N*sizeof(int));

        //cout<<"constructed a new uctree"<<endl;
        

    }
    int UCTree::judgeProfit(int x, int y){
        //cout << "in judgeprofit" << endl;
        // if (x>=Game::M || x<0) cout << "x is "<<x <<endl;
        // if (y>=Game::N || y<0) cout << "y is "<<y <<endl;
        if(userWin(x,y,Game::M,Game::N,Game::board)){
            return PROFIT_LOSE;
        }
        if(machineWin(x,y,Game::M,Game::N,Game::board)){
            return PROFIT_WIN;
        }
        if(isTie(Game::N,Game::top)){
            return PROFIT_TIE;
        }
        return NOT_END;
    }

    UCTNode* UCTree::treePolicy(UCTNode* hot){
        // cout << "in treepolicy" <<endl;
        while((!hot->isEnd())){
            if(hot->canExpand()){
                return hot->expand();
            }
            else{
                hot = hot->bestChild();
                Game::board[hot->s_x][hot->s_y] = hot->turn;
                --Game::top[hot->s_y];
                if (hot->s_x-1==Game::noX&&hot->s_y==Game::noY)
                    --Game::top[hot->s_y];
            }
        }
        return hot;
    }
    
    void UCTree::randomPlaceItem(int turn,int* sx ,int* sy){
        int y = 0;
        do{
            y = rand()%Game::N;
        }while (Game::top[y] == 0);
        --Game::top[y];
        int x = Game::top[y];
        Game::board[x][y] = turn;
        if(x-1 == Game::noX&& y == Game::noY){
            --Game::top[y];
        }
        *sx = x;
        *sy = y;
    }

    int UCTree::changeTurn(int originturn){
        if (originturn == USER_TURN)
            return MACHINE_TURN;
        return USER_TURN;
    }

    float UCTree::defaultPolicy(UCTNode* hot) {
        
        int turn = hot->turn;
        int profit_state = 0;
        int sx = hot->s_x;
        int sy = hot->s_y;
        while(1){
            profit_state = judgeProfit(sx,sy);
            if (profit_state!=NOT_END)
                break;
            turn = changeTurn(turn);
            randomPlaceItem(turn,&sx,&sy);
        }
        if (hot->turn == USER_TURN){
            profit_state = -profit_state;
        }
        return profit_state;
    }


    
    UCTNode* UCTree::searchFromRoot(){
        _root = new UCTNode(-1,-1,USER_TURN,nullptr);
        UCTNode::used_nodes = 0;
        if (_root->test_for_root()){
            return _root;
        }

        int origin_board[Game::M][Game::N];
        int origin_top[Game::N];
        for(int i=0;i<Game::M;++i){
            memcpy(origin_board[i],Game::board[i],Game::N*sizeof(int));
        }
        memcpy(origin_top,Game::top,Game::N*sizeof(int));



        int tst = 0;

        while((clock()+0.0)/CLOCKS_PER_SEC - this->start_time<TIME_LIMIT){
            ++tst;
            
            for(int i=0;i<Game::M;++i){
                memcpy(Game::board[i],origin_board[i],Game::N*sizeof(int));
            }
            memcpy(Game::top,origin_top,Game::N*sizeof(int));
            UCTNode* node = treePolicy(_root);
            
            double d_profit = 0;
            d_profit = defaultPolicy(node);
            node->backUp(d_profit);
            
        }
        return _root->bestChild();
    }

    UCTree::~UCTree(){
        _root->clear();
        delete _root;
    }
