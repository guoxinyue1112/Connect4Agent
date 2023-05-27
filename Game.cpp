#include "Game.h"

int Game::M = 0;
int Game::N = 0;
int Game::noX = 0;
int Game::noY = 0;
int** Game::board = nullptr;
int* Game::top = nullptr;

bool Game::board_is_new = true;