#ifndef __UCTNODE__
#define __UCTNODE__

#include <iostream>
#include <time.h>
#include <math.h>
#include <cstring>
#include "Judge.h"
#include "utils.h"

class Node
{
public:
    ;
    int turn;
    int s_x;
    int s_y; // 落子位置,root 为-1、-1
    int depth;

    int visit_times;
    double profit;

    int *emp;
    int expand_node_rest;
    int is_end;

    Node *parent;
    Node **children;

    static Node **pool;
    static bool new_pool;
    static unsigned int used_nodes;

    Node(int s_x, int s_y, int turn, Node *parent = nullptr);
    static Node *newNode(int sx, int sy, int turn, Node *parent);

    bool isEnd();

    bool canExpand();

    void fill(int sx, int sy, int turn, Node *parent);

    bool firstTest();

    void clear();
};

#endif