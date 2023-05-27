#ifndef  __UCT__
#define __UCT__
#include "UCTNode.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <cstring>
#include "Judge.h"
#include "utils.h"



class UCTree{
    public:
        UCTNode* _root;
        
        float start_time;

        UCTree(const int _M, const int _N, const int* _top, int** _board, const int lastX, const int lastY, const int _noX, const int _noY);
   
        int judgeProfit(int x, int y);

        UCTNode* treePolicy(UCTNode* hot);
    
        void randomPlaceItem(int turn,int *x,int*y);

        int changeTurn(int originturn);

        float defaultPolicy(UCTNode* hot);
    
        UCTNode* searchFromRoot();

        ~UCTree();


       
};

#endif