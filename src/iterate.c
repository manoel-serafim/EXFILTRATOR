#include "libs.h"
#include "iterate.h"
#include "types.h"


void init(state *s, int w, int h)
{
    switch (s->mode)
    {
        
        case DEFAULT:
            s->current_x = 0;
            s->current_y = 0;
            s->height = h;
            s->width = w;
            s->status = 1;
            s->diag = -1;
        break;

        case REVERSE:
            s->current_x = w;
            s->current_y = h;
            s->height = h;
            s->width = w;
            s->status = 1;
            s->diag = -1;
        break;
        case DIAGONAL:
            s->current_x = 0;
            s->current_y = 2;
            s->height = h;
            s->width = w;
            s->status = 1;
            s->diag = 2;
            s->dir = 1;
        break;
        case SEED:
            s->current_x = 0;
            s->current_y = 0;
            s->height = h;
            s->width = w;
            s->status = 1;
            s->diag = 1;
            s->gen = malloc(w * h * sizeof(char));
            memset(s->gen, 0, w * h);
            break;
    }

}


cord yield(state *s)
{
    if(s->mode == DEFAULT)
    {
        int next_x = ++s->current_x;
        if(next_x == s->width - 2) {
            s->current_y++;
            s->current_x = 0;
        }
        int next_y = s->current_y + 1;
        cord c = {
            .x = next_x,
            .y = next_y
        };
        if(next_y == s->height - 2 && next_x == s->width-2) s->status = -1;
        return c;
    }
    if(s->mode == REVERSE)
    {
        int next_x = s->current_x--;
        if(--next_x == 1) {
            s->current_y--;
            s->current_x = s->width;
        }
        
        int next_y = s->current_y - 1;
        cord c = {
            .x = next_x,
            .y = next_y
        };
        if(next_y == 1) s->status = -1;
        return c;
    }
    if(s->mode == DIAGONAL)
    {
        
        if(s->diag == 2 && s->current_x == s->width - 2 && s->dir == -1) s->status = -1;
        int next_x = ++s->current_x;
        int next_y = --s->current_y;
        if(s->dir == 1)
        {
            if(next_x == s->diag - 1)
            {
                s->diag++;
                s->current_y = s->diag;
                s->current_x = 1;
            }
            if(s->diag == s->height - 1) {
                s->dir = -1;
            } 
            
        }if(s->dir == -1) 
        {
          
            if(next_y == ((s->height - 1) - s->diag)){
                s->diag += s->dir;
                s->current_x = (s->height) - s->diag;
                s->current_y = (s->height - 2);
            }

        }    
        
        cord c = {
            c.x = next_x,
            c.y = next_y
        };
        return c;
    }
    if(s->mode == SEED)
    {
        while(1)
        {
            int next_x = rand() % ((s->width - 1)) + 1;
            int next_y = rand() % ((s->height - 1)) + 1;
            if(s->gen[next_y  * (next_x - 1) + next_x] == 1) continue;
            s->gen[next_y * (next_x - 1) + next_x] = 1;
            cord c = {
                .x = next_x,
                .y = next_y
            };
            return c;
        }
        
    }
}