#ifndef SNAKE_H

#define SNAKE_H
#include <curses.h>



#define STATE_UP 0x01
#define STATE_DOWN 0x02
#define STATE_LEFT 0x40
#define STATE_RIGHT 0x80
#define NONE 0x00
#define IS_SAME_DIR(x, y) (x == y)
#define IS_OPPSITE_DIR(x, y) ((x!=y!=NONE) && ((x >> 1) == y || (x << 1) == y))



#define GO_FORWARD 0
#define TURN_LEFT 1
#define TURN_RIGHT 2

#define GO_UP 0x01
#define GO_DOWN 0x02
#define GO_LEFT 0x40
#define GO_RIGHT 0x80

#define DELAYSIZE 100

typedef struct Snake {
    int x, y;
    struct Snake* next;
} snake_t;

typedef snake_t* snake_t_ptr;




class GreedySnake {
    public:
        GreedySnake();

        // Static function
        static GreedySnake* CreateNewSnake(int, int, int, int);
        static Snake* newSnakeNode(int, int);
        static GreedySnake* loadSnake(char*);


        // Member function
        void update(int);
        void saveSnake(char*);
        void addOneNode();
        void delOneNode();

        // getter & setter
        void setHead(snake_t_ptr);
        void setTail(snake_t_ptr);
        snake_t_ptr getHead();
        snake_t_ptr getTail();
        void setState(int);
        int getState();
        int getLen();

    private:
        void nextPosition(int*, int*);
        void insertAtHead(snake_t_ptr);
        void removeFromTail();
        int row, col;
        int len, state;
        snake_t_ptr snakeHead;
        snake_t_ptr snakeTail;
        const int direction_state[4][3][2] = {
            /* state up */ {
                {0, -1}, {-1, 0}, {1, 0}
            },
            /* state right */{
                {1, 0}, {0, -1}, {0, 1}
            },
            /* state down */{
                {0, 1}, {1, 0}, {-1, 0}
            },
            /* state left */{
                {-1, 0}, {0, 1}, {0, -1}
            }
        };
        const int direction[8][2] = {
            // go up
            {0, -1},
            // go down
            {0, 1},
            // go left,
            {0,0},
            {-1,0},
            {0,0},
            {0,0},
            {0, 0},
            // go right
            {1, 0}
        };


};



class Game {
    public:
    // constructor
        Game(int row, int col);
        Game();

    // Static function
        static Game* loadGameProgress();
        static void drawSnake(GreedySnake*);

    // Setter & getter
        void set_snake1(GreedySnake*);
        GreedySnake* get_snake1();
        void set_snake2(GreedySnake*);
        GreedySnake* get_snake2();

    // Member function
        void setup(int, int);
        void setup();
        int execute();
        bool checkCollision();
        void draw();
        void draw_apple();
        void draw_wall();
        void myrefresh();
        void saveGameProgress();

    private:
        bool gameOver;
        bool apple;
        struct {
            int x,y;
        } apple_pos;
        int row, col;
        GreedySnake *s1, *s2;

};



#endif