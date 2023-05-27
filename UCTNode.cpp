#include "UCTNode.h"
#include "Game.h"
using namespace std;


unsigned int UCTNode::used_nodes = 0;
UCTNode** UCTNode::pool =nullptr;
bool UCTNode::new_pool = true;
        UCTNode::UCTNode(int s_x,int s_y,int turn ,UCTNode* parent){
            children = new UCTNode*[Game::N];
            emp = new int[Game::N];
            fillNode(s_x,s_y,turn,parent);   
        }


        void UCTNode::backUp(double dp){
            //回溯更新收益值
            UCTNode* now = this;
            while (now){
                ++(now->visit_times);
                now->profit+=dp;
                dp = 0-dp;
                now = now->parent;
            }
        }
        UCTNode *UCTNode::bestChild()
        {
            int ind = -1;
            double max_profit = -RAND_MAX;
            for (int i = 0; i < Game::N; ++i)
            {
                if (!children[i])
                    continue;
                bool conti = false;
                double raw_profit = 0;

                if (conti)
                    raw_profit = -50;
                else
                {

                    double Nv = children[i]->visit_times;
                    double Qv = children[i]->profit;

                    raw_profit = Qv / Nv + sqrt(2 * log(visit_times) / Nv) * UCT_C;
                }
                if (raw_profit > max_profit)
                {
                    ind = i;
                    max_profit = raw_profit;
                }
            }
            return children[ind];
        }

        bool UCTNode::isEnd(){
            if (is_end != -1){
                return is_end;
            }
            if (s_x == -1 && s_y==-1) {      //根节点非终止
                is_end = 0;
                return is_end;
            }
            if(userWin(s_x,s_y,Game::M,Game::N,Game::board) || machineWin(s_x,s_y,Game::M,Game::N,Game::board)||isTie(Game::N,Game::top)){
                is_end = 1;
                return is_end;
            }
            is_end = 0;
            return is_end;
        
        }
        bool UCTNode::canExpand(){
            if(this->expand_node_rest>0)
                return true;
            return false;
        }

        void UCTNode::fillNode(int sx,int sy,int turn, UCTNode* parent){
            visit_times=0;
            profit = 0;
            if(parent){
                depth = parent->depth+1;
            }
            else depth = 0;
            s_x = sx;
            expand_node_rest = 0;
            is_end = -1;
            s_y = sy;
            this->turn = turn;
            this->parent = parent;
            for(int i =0;i<Game::N;++i){
                if(Game::top[i]){
                    emp[expand_node_rest] = i;
                    ++expand_node_rest;
                }
                children[i]=nullptr;
            }
        }


        UCTNode* UCTNode::newNode(int sx,int sy, int turn ,UCTNode* parent){
            if(used_nodes >= MAX_POOL){
                return new UCTNode(sx,sy,turn,parent);
            }
            else if (!pool[used_nodes]){
                pool[used_nodes++] = new UCTNode(sx,sy,turn,parent);
                return pool[used_nodes-1];
            }
            pool[used_nodes++]->fillNode(sx,sy,turn,parent);
            return pool[used_nodes-1];
        }

        UCTNode* UCTNode::expand(){
            //复制状态
            //转换棋权
            int sw_turn = this->nextTurn();
            //下棋
            int index = rand()%expand_node_rest;
            int sy = emp[index];
            --Game::top[sy];
            int sx = Game::top[sy];
            if(sx - 1 == Game::noX && sy == Game::noY)
                --Game::top[sy];
            Game::board[sx][sy] = sw_turn;
            //生成节点
            children[sy] = newNode(sx,sy,sw_turn,this);
            swap(emp[index],emp[--expand_node_rest]);
            return children[sy];
        }

        int UCTNode::nextTurn(){
            if(turn==USER_TURN)
                return MACHINE_TURN;
            return USER_TURN;
        }
        
        void UCTNode::clear(){
            for(int i =0 ;i<Game::N;++i){
                if(children[i]){
                    children[i] = nullptr;
                }
            }
            delete[] children;
            delete[] emp;
        }
    
        bool UCTNode::punish(){

            //必输的话惩罚
            //必胜的话奖励
            if(userWin(s_x,s_y,Game::M,Game::N,Game::board)){
                this->profit = -1e3;
                return true;
            }
            if(machineWin(s_x,s_y,Game::M,Game::N,Game::board)){
                this->profit = 1e3;
                return true;
            }

            for(int i = 0;i<Game::N;++i){
                if(Game::top[i] >0){
                    Game::board[Game::top[i]-1][i] = this->nextTurn();
                    if(this->nextTurn()==USER_TURN&&userWin(Game::top[i]-1,i,Game::M,Game::N,Game::board)){
                        this->profit = -1e3;
                        Game::board[Game::top[i]-1][i] = 0;
                        return true;
                    }
                    if(this->nextTurn()==MACHINE_TURN&&machineWin(Game::top[i]-1,i,Game::M,Game::N,Game::board)){
                        this->profit = 1e3;
                        Game::board[Game::top[i]-1][i] = 0;
                        return true;
                    }
                }
            }
            return false;
            
        }

        bool UCTNode::test_for_root(){
            
            for(int i =0;i<Game::N;++i){
                if(Game::top[i]){
                    Game::board[Game::top[i] - 1][i] = MACHINE_TURN;
                    if (machineWin(Game::top[i] - 1, i, Game::M, Game::N, Game::board)){
                        this->s_x = Game::top[i] - 1;
                        this->s_y = i;
                        //下必胜点
                        return true;
                    }
                    Game::board[Game::top[i]-1][i] = 0;
                }
            }
            for(int i=0;i<Game::N;++i){
                if(Game::top[i]){
                    Game::board[Game::top[i] - 1][i] = USER_TURN;
                    if (userWin(Game::top[i] - 1, i, Game::M, Game::N, Game::board)){
                        this->s_x = Game::top[i] - 1;
                        this->s_y = i;
                        //堵必败点
                        return true;
                    }
                    Game::board[Game::top[i]-1][i] = 0;
                }
            }
            return false;
        }
        