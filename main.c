#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dimensions.h"

#define EPF_tile(X, Y) (X + (W * (Y)))
#define EPF_rtile(T, X, Y) { X = (int)T % W; Y = (int)floorf(T / W); }

void EPF_drawMap(int* tiles, int start, int current, int finish, int W, int H)
{
    int currentTile;
    for(int Y = 0; Y < H; Y++)
    {
        for(int X = 0; X < W; X++)
        {
            currentTile = EPF_tile(X, Y);
            
            if(currentTile == start)
            {
                printf("S ");
                
                continue;
            }
            
            if(currentTile == current)
            {
                printf("C ");
                
                continue;
            }
            
            if(currentTile == finish)
            {
                printf("X ");
                
                continue;
            }
            
            if(tiles[currentTile] == 1)
            {
                printf("# ");
                
                continue;
            }
            
            printf(". ");
        }
        
        printf("\n");
    }
}

void EPF_fillPathValues(float* path, int* tiles, int start, int finish, int W, int H)
{
    int length = W * H;
    
    int startX;
    int startY;
    EPF_rtile(start, startX, startY);
    
    int finishX;
    int finishY;
    EPF_rtile(finish, finishX, finishY);
    
    int X;
    int Y;
    float z;
    for(int i = 0; i < length; i++)
    {
        if(tiles[i] != 0)
        {
            continue;
        }
        
        EPF_rtile(i, X, Y);
        
        float toStart;
        float toFinish;
        
        toStart = fmin(abs(startX - X), abs(startY - Y));
        z = sqrtf(powf(toStart, 2) + powf(toStart, 2));
        toStart = (fmax(abs(startX - X), abs(startY - Y))) - toStart + z;
        
        toFinish = fmin(abs(finishX - X), abs(finishY - Y));
        z = sqrtf(powf(toFinish, 2) + powf(toFinish, 2));
        toFinish = (fmax(abs(finishX - X), abs(finishY - Y))) - toFinish + z;
        
        path[i] = toStart + (toFinish * 1.1f);
    }
}

int EPF_in_array(int v, int* array, int length)
{
    for(int i = 0; i < length; i++)
    {
        if(array[i] == -1)
        {
            return -1;
        }
        
        if(array[i] == v)
        {
            return i;
        }
    }
    
    return -1;
}

void EPF_getTrail(int* trail, int* tiles, int* visited, float* path, int start, int finish, int W, int H)
{
    int length = W * H;
    int current = start;
    int X;
    int Y;
    
    //visited[0] = -1;
    visited[0] = start;
    visited[1] = -1;
    int u = 1;
    int v = 0;
    int vu = 0;
    
    int tLength = 0;
    
    trail[0] = start;
    for(int i = 1; i < length; i++)
    {
        EPF_rtile(current, X, Y);
        
        int dirs[8] = {
            // Up
            Y > 0 ? EPF_tile(X, Y - 1) : 0,
            // Up right
            //Y > 0 && X < W - 1 ? EPF_tile(X + 1, Y - 1) : 0,
            // Up left
            //Y > 0 && X > 0 ? EPF_tile(X - 1, Y - 1) : 0,
            // Left
            X > 0 ? EPF_tile(X - 1, Y) : 0,
            // Right
            X < W - 1 ? EPF_tile(X + 1, Y) : 0,
            // Down
            Y < H - 1 ? EPF_tile(X, Y + 1) : 0,
            // Down right
            //Y < H - 1 && X < W - 1 ? EPF_tile(X + 1, Y + 1) : 0,
            // Down left
            //Y < H - 1 && X > 0 ? EPF_tile(X - 1, Y + 1) : 0,
        };
        
        float lowest;
        int lowestTile = -1;
        for(int j = 0; j < sizeof(dirs) / sizeof(int); j++)
        {
            if(dirs[j] == 0 || tiles[dirs[j]] != 0 || (EPF_in_array(dirs[j], visited, length) != -1))
            {
                continue;
            }
            
            if(lowestTile == -1 || path[dirs[j]] < lowest)
            {
                lowestTile = dirs[j];
                lowest = path[dirs[j]];
            }
        }
        
        if(lowestTile == -1)
        {
            assert(i >= 2);
            u--;
            vu--;
            int vi = vu;
            lowestTile = visited[vi];
            
            int vX;
            int vY;
            EPF_rtile(lowestTile, vX, vY);
        }
        else
        {
            trail[u] = lowestTile;
            u++;
        }
        
        current = lowestTile;
        
        if(EPF_in_array(lowestTile, visited, W * H) == -1)
        {
            visited[v] = lowestTile;
            v++;
            vu = v;
        }
        
        if(current == finish)
        {
            tLength = u + 1;
            
            break;
        }
    }
}

int main()
{
    int W = _W;
    int H = _H;
    
    float path[W * H];
    memset(path, 0, sizeof(path));
    
    int trail[W * H];
    memset(trail, -1, sizeof(trail));
    
    int visited[W * H];
    memset(visited, -1, sizeof(trail));
    
    int tiles[_W * _H] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    
    const int start = EPF_tile(1, 1);
    const int finish = EPF_tile(14, 10);
    int current;
    
    EPF_fillPathValues(path, tiles, start, finish, W, H);
    
    EPF_EPF_getTrail(trail, tiles, visited, path, start, finish, W, H);
    
    for(int i = 0; i < W * H; i++)
    {
        if(trail[i] == -1)
        {
            break;
        }
        
        current = trail[i];
        
        EPF_drawMap(tiles, start, current, finish, W, H);
        
        getchar();
        system("clear");
    }
    
    printf("Done!\n");
    
    return 0;
}
