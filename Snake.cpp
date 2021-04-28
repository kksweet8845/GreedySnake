#include "Snake.h"
#include <iostream>
#include <stdlib.h>
#include <curses.h>
#include <stdio.h>
#include <time.h>


// static FILE* fop = fopen("trace.txt", "w");

#define COLLIDE_WALL(x, y) (x < 0 || x > this->col || y < 0 || y > this->row)

GreedySnake::GreedySnake(){

}


GreedySnake* GreedySnake::CreateNewSnake(int x, int y, int row, int col){

    GreedySnake* gs = new GreedySnake();
    gs->snakeHead = GreedySnake().newSnakeNode(x, y);
    gs->len = 1;
    gs->snakeTail = gs->snakeHead;
    gs->row = row;
    gs->col = col;
    gs->state = STATE_UP;

    // Special setup for default 10 len
    for(int i=0;i<19;i++){
        gs->addOneNode();
        // fprintf(fop, "New node\n");
    }
    return gs;
}



Snake* GreedySnake::newSnakeNode(int x, int y){
    Snake* sn = new Snake();
    sn->x = x;
    sn->y = y;
    sn->next = NULL;
    return sn;
}

void GreedySnake::update(int turn){

    int x = this->snakeHead->x;
    int y = this->snakeHead->y;

    int cur_state = this->getState();
    // fprintf(fop, "same dir %d, opp dir %d\n", IS_SAME_DIR(cur_state, turn), IS_OPPSITE_DIR(cur_state, turn));
    if((!IS_SAME_DIR(cur_state, turn) && !IS_OPPSITE_DIR(cur_state, turn)) && turn != NONE)
        this->setState(turn);

    this->nextPosition(&x, &y);
    Snake* sn = GreedySnake().newSnakeNode(x, y);
    this->insertAtHead(sn);
    this->removeFromTail();
}


void GreedySnake::nextPosition(int* x, int* y){
    int dx, dy;
    int dir = (((this->getState() & 0xf0) >> 4) | (this->getState() & 0x0f)) - 1;
    // fprintf(fop, "state: %x, dir : %d\n", this->getState(), dir);
    dx = this->direction[dir][0];
    dy = this->direction[dir][1];
    *x += dx;
    *y += dy;
}

void GreedySnake::addOneNode() {

    int x = this->snakeHead->x;
    int y = this->snakeHead->y;

    // this->nextPosition(&x, &y, GO_FORWARD);
    this->nextPosition(&x, &y);

    // fprintf(fop, "new snake node (%d, %d)\n", x, y);
    Snake* sn = GreedySnake().newSnakeNode(x, y);

    this->insertAtHead(sn);

    (this->len)++;
}

void GreedySnake::delOneNode() {

    this->removeFromTail();
    (this->len)--;

}


void GreedySnake::setHead(snake_t_ptr addr){
    if(addr != NULL){
        this->snakeHead = addr;
    }
}

void GreedySnake::setTail(snake_t_ptr addr){
    if(addr != NULL){
        this->snakeTail = addr;
    }
}

snake_t_ptr GreedySnake::getHead() {
    return this->snakeHead;
}

snake_t_ptr GreedySnake::getTail() {
    return this->snakeTail;
}

void GreedySnake::setState(int s){
    this->state = s;
}

int GreedySnake::getState() {
    return this->state;
}

int GreedySnake::getLen() {
    return this->len;
}

void GreedySnake::insertAtHead(snake_t_ptr node){

    this->snakeHead->next = node;
    this->snakeHead = node;
}

void GreedySnake::removeFromTail(){
    Snake* del = this->snakeTail;
    this->snakeTail = this->snakeTail->next;
    delete del;
}


void GreedySnake::saveSnake(char* name){

    FILE* fpt = fopen(name, "w+");
    fwrite(this, sizeof(GreedySnake), 1, fpt);
    for(Snake* cur=this->snakeTail; cur != NULL; cur = cur->next){
        fwrite(&cur->x, sizeof(int), 1, fpt);
        fwrite(&cur->y, sizeof(int), 1, fpt);
    }
    fclose(fpt);
}

GreedySnake* GreedySnake::loadSnake(char* name){
    FILE* fpt = fopen(name, "r");
    GreedySnake* gs = (GreedySnake*) malloc(sizeof(GreedySnake));
    fread(gs, sizeof(GreedySnake), 1, fpt);
    gs->snakeHead = NULL;
    gs->snakeTail = NULL;
    int tmp[2];
    while(fread(tmp, sizeof(int) * 2, 1, fpt)){
        Snake* ns = newSnakeNode(tmp[0], tmp[1]);
        if(gs->snakeHead == NULL){
            gs->snakeHead = gs->snakeTail = ns;
        }else{
            gs->insertAtHead(ns);
        }
    }
    return gs;
}


// ====================================================== //
// ======================== Game ======================== //
// ====================================================== //

Game::Game() {

}

Game::Game(int row, int col) {
    this->setup(row, col);
    this->s1 = GreedySnake().CreateNewSnake(30, 30, row, col);
    this->s2 = GreedySnake().CreateNewSnake(row - 10, col - 10, row, col);
}


void Game::setup(int row, int col) {

    this->gameOver = false;
    this->apple = false;

    this->row = row;
    this->col = col;

    this->apple_pos.x = rand() % col;
    this->apple_pos.y = rand() % row;

    // init src
#ifdef XCURSES
    Xinitscr(argc, argv);
#else
	initscr();
#endif
    keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	noecho();
	attrset(A_NORMAL);
    // getmaxyx(stdscr, row, col); // y, x
}

void Game::setup() {
    // init src
#ifdef XCURSES
    Xinitscr(argc, argv);
#else
	initscr();
#endif
    keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	noecho();
	attrset(A_NORMAL);
    // getmaxyx(stdscr, row, col); // y, x
}


void Game::set_snake1(GreedySnake* addr){
    if(addr != NULL){
        this->s1 = addr;
    }
}
void Game::set_snake2(GreedySnake* addr){
    if(addr != NULL){
        this->s2 = addr;
    }
}

GreedySnake* Game::get_snake1() {
    return this->s1;
}
GreedySnake* Game::get_snake2() {
    return this->s2;
}

int Game::execute(){
    chtype c;
    uint8_t begin = 0;
    bool s1_win, s2_win;
    s1_win = FALSE;
    s2_win = FALSE;

    while(!this->gameOver){

        int prob = rand() % 100;
        this->apple = true;

        c = getch();
        int turn_1 = NONE, turn_2 = NONE;
        switch(c){
            case KEY_UP:
                turn_1 = GO_UP;
                break;
            case KEY_LEFT:
                turn_1 = GO_LEFT;
                break;
            case KEY_RIGHT:
                turn_1 = GO_RIGHT;
                break;
            case KEY_DOWN:
                turn_1 = GO_DOWN;
                break;
            case KEY_F(1):
                this->saveGameProgress();
                return 0;
            case KEY_F(2):
                this->apple_pos.x = rand() % this->col;
                this->apple_pos.y = rand() % this->row;
                break;
            case 'w':
                turn_2 = GO_UP;
                break;
            case 'a':
                turn_2 = GO_LEFT;
                break;
            case 'd':
                turn_2 = GO_RIGHT;
                break;
            case 's':
                turn_2 = GO_DOWN;
                break;
        }

        this->s1->update(turn_1);
        this->s2->update(turn_2);

        this->gameOver = this->checkCollision();

        if(this->s1->getLen() == 1 || this->s2->getLen() == 1){
            this->gameOver |= TRUE;
        }


        this->draw();

        while( !begin ) {
            c = getch();
            if(c == KEY_UP)
                begin = 1;
        }
    }

    return 0;
}


bool Game::checkCollision() {

    // check snake collide with apple
    bool s1_collide_a = false, s2_collide_a = false;
    Snake* cur_s1 = this->s1->getHead();
    Snake* cur_s2 = this->s2->getHead();
    if(this->apple){
        if(this->apple_pos.x == cur_s1->x && this->apple_pos.y == cur_s1->y){
            s1_collide_a = true;
        }
        if(this->apple_pos.x == cur_s2->x && this->apple_pos.y == cur_s2->y){
            s2_collide_a = true;
        }
        if(s1_collide_a || s2_collide_a){
            this->apple = false;
            this->apple_pos.x = rand() % col;
            this->apple_pos.y = rand() % row;
        }
    }

    if(s1_collide_a)
        this->s1->delOneNode();
    if(s2_collide_a)
        this->s2->delOneNode();

    // check snake collide with snake or wall
    bool s1_collide_s2 = false, s2_collide_s1 = false,
         s1_collide_s1 = false, s2_collide_s2 = false,
         s1_collide_wall = false, s2_collide_wall = false;

    // if(cur_s2->x > this->col || cur_s2->x < 0 ||)
    if(COLLIDE_WALL(cur_s2->x, cur_s2->y))
        s2_collide_wall = true;
    // if(cur_s1->y > this->row || cur_s1->y < 0)
    if(COLLIDE_WALL(cur_s1->x, cur_s1->y))
        s1_collide_wall = true;


    for(Snake* cur=this->s1->getTail(); cur != NULL; cur=cur->next){
        if(cur->x == cur_s2->x && cur->y == cur_s2->y){
            s2_collide_s1 = true;
        }
        if(cur->x == cur_s1->x && cur->y == cur_s1->y && cur != cur_s1){
            s1_collide_s1 = true;
        }
    }
    for(Snake* cur=this->s2->getTail(); cur != NULL; cur=cur->next){
        if(cur->x == cur_s1->x && cur->y == cur_s1->y){
            s1_collide_s2 = true;
        }
        if(cur->x == cur_s2->x && cur->y == cur_s2->y && cur != cur_s2){
            s2_collide_s2 = true;
        }
    }
    if(s1_collide_s1 || s1_collide_s2 || s2_collide_s1 || s2_collide_s2
        || s1_collide_wall || s2_collide_wall){
        return true;
    }
    return false;
}

void Game::drawSnake(GreedySnake * s){
    for(Snake* cur = s->getTail(); cur != NULL; cur = cur->next){
        mvaddstr(cur->y, cur->x, "*");
    }
}

void Game::myrefresh() {
    napms(DELAYSIZE);
	move( this->row - 1, this->col - 1);
	refresh();
}


void Game::draw_apple() {
    mvaddstr(this->apple_pos.y, this->apple_pos.x, "$");
}

void Game::draw_wall() {

    for(int i=0;i<this->row;i++){
        for(int j=0;j<this->col+1;j++){
            if(i == 0 || i == 49 )
                mvaddstr(i, j, "--");
            else if(j == 0 || j == 50)
                mvaddstr(i, j, "!");
            else
                continue;
        }
    }
}

void Game::draw() {
    erase();

    drawSnake(this->s1);
    drawSnake(this->s2);

    if(this->apple){
        this->draw_apple();
    }

    this->draw_wall();
    this->myrefresh();
}

void Game::saveGameProgress() {
    FILE* fpt = fopen(".game_progress", "w");
    fwrite(this, sizeof(Game), 1, fpt);
    this->s1->saveSnake(".snake1");
    this->s2->saveSnake(".snake2");

    fclose(fpt);
}

Game* Game::loadGameProgress() {
    FILE* fpt = fopen(".game_progress", "r");
    Game* game = NULL;
    if(fpt){
        game = (Game*) malloc(sizeof(Game));
        while(fread(game, sizeof(Game), 1, fpt));
        game->set_snake1(GreedySnake().loadSnake(".snake1"));
        game->set_snake2(GreedySnake().loadSnake(".snake2"));
        if(game->s1 == NULL || game->s2 == NULL){
            return NULL;
        }
        fclose(fpt);
    }
    return game;
}