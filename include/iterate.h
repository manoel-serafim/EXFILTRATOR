#ifndef __ITERATE_H_
#define __ITERATE_H_

typedef struct cord
{
    int x;
    int y;
} cord;

typedef enum IT_MODES
{
    DEFAULT,
    REVERSE,
    DIAGONAL,
    SEED
}MODE;


typedef struct state
{
    int width;
    int height;
    int seed; 
    int current_y;
    int current_x;
    MODE mode;
    int status;
    int diag;
    int dir;
    char *gen;
} state;


void init(state *, int, int);
cord yield(state *);


#endif