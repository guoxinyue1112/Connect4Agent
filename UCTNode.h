#ifndef __UCTNODE__
#define __UCTNODE__


#include <iostream>
#include <time.h>
#include <math.h>
#include <cstring>
#include "Judge.h"
#include "utils.h"
#include <utility>


class UCTNode{
    public:;
        int turn;
        int s_x;
        int s_y;//落子位置,root 为-1、-1
        int depth;

        int visit_times;
        double profit;


        int* emp;
        int expand_node_rest;
        int is_end;

        UCTNode* parent;
        UCTNode** children;

        static UCTNode** pool;
        static bool new_pool;
        static unsigned int used_nodes;

        UCTNode(int s_x,int s_y,int turn ,UCTNode* parent = nullptr);

        


        void backUp(double dp);
        UCTNode* bestChild();

        bool isEnd();     
        bool canExpand();
        void fillNode(int sx,int sy,int turn, UCTNode* parent);
        

        UCTNode* newNode(int sx,int sy, int turn ,UCTNode* parent);

        UCTNode* expand();
        bool punish();
        
        bool test_for_root();
        

        int nextTurn();
        void clear();
    
};

#endif