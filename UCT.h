#ifndef __UCT__
#define __UCT__
#include "UCTNode.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <cstring>
#include "Judge.h"
#include "utils.h"

class UCT
{
public:
    Node *root;

    float start_time;

    UCT(const int _M, const int _N, const int *_top, int **_board, const int _lastX, const int _lastY, const int _noX, const int _noY);

    Node *uctSearch();

    Node *treePolicy(Node *);

    Node *expand(Node*);

    Node *bestChild(Node*);

    int defaultPolicy(Node *);

    void backUp(Node*, int);

    int profit(int, int);

    void move(int turn, int *x, int *y);

    int nextTurn(int);

    ~UCT();
};

#endif