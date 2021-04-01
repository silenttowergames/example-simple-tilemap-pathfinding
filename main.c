#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dimensions.h"

#define tile(X, Y) (X + (W * (Y)))
#define rtile(T, X, Y) { X = (int)T % W; Y = (int)floorf(T / W); }

typedef struct PFInfo
{
    float toStart;
    float toFinish;
    float mix;
} PFInfo;

void drawMap(int* tiles, int start, int current, int finish)
{
    int W = _W;
    int H = _H;
    
    int currentTile;
    for(int Y = 0; Y < H; Y++)
    {
        for(int X = 0; X < W; X++)
        {
            currentTile = tile(X, Y);
            
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

void fillPathValues(PFInfo* path, int* tiles, int start, int finish, int length)
{
    int W = _W;
    int H = _H;
    
    int startX;
    int startY;
    rtile(start, startX, startY);
    
    int finishX;
    int finishY;
    rtile(finish, finishX, finishY);
    
    int X;
    int Y;
    float z;
    for(int i = 0; i < length; i++)
    {
        if(tiles[i] != 0)
        {
            continue;
        }
        
        rtile(i, X, Y);
        
        path[i].toStart = fmin(abs(startX - X), abs(startY - Y));
        z = sqrtf(powf(path[i].toStart, 2) + powf(path[i].toStart, 2));
        path[i].toStart = (fmax(abs(startX - X), abs(startY - Y))) - path[i].toStart + z;
        
        path[i].toFinish = fmin(abs(finishX - X), abs(finishY - Y));
        z = sqrtf(powf(path[i].toFinish, 2) + powf(path[i].toFinish, 2));
        path[i].toFinish = (fmax(abs(finishX - X), abs(finishY - Y))) - path[i].toFinish + z;
        
        path[i].mix = path[i].toStart + (path[i].toFinish * 1.1f);
        //path[i].mix = path[i].toStart + path[i].toFinish;
    }
}

int in_array(int v, int* array, int length)
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

void getTrail(int* trail, int* tiles, int* visited, PFInfo* path, int start, int finish, int length)
{
    int W = _W;
    int H = _H;
    
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
        rtile(current, X, Y);
        
        int dirs[8] = {
            // Up
            Y > 0 ? tile(X, Y - 1) : 0,
            // Up right
            Y > 0 && X < W - 1 ? tile(X + 1, Y - 1) : 0,
            // Up left
            Y > 0 && X > 0 ? tile(X - 1, Y - 1) : 0,
            // Left
            X > 0 ? tile(X - 1, Y) : 0,
            // Right
            X < W - 1 ? tile(X + 1, Y) : 0,
            // Down
            Y < H - 1 ? tile(X, Y + 1) : 0,
            // Down right
            Y < H - 1 && X < W - 1 ? tile(X + 1, Y + 1) : 0,
            // Down left
            Y < H - 1 && X > 0 ? tile(X - 1, Y + 1) : 0,
        };
        
        float lowest;
        int lowestTile = -1;
        for(int j = 0; j < sizeof(dirs) / sizeof(int); j++)
        {
            if(dirs[j] == 0 || tiles[dirs[j]] != 0 || (in_array(dirs[j], visited, length) != -1))
            {
                continue;
            }
            
            if(lowestTile == -1 || path[dirs[j]].mix < lowest)
            {
                lowestTile = dirs[j];
                lowest = path[dirs[j]].mix;
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
            rtile(lowestTile, vX, vY);
        }
        else
        {
            trail[u] = lowestTile;
            u++;
        }
        
        current = lowestTile;
        
        if(in_array(lowestTile, visited, W * H) == -1)
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
    
    PFInfo path[W * H];
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
    
    const int start = tile(1, 1);
    const int finish = tile(14, 10);
    int current;
    
    fillPathValues(path, tiles, start, finish, W * H);
    
    getTrail(trail, tiles, visited, path, start, finish, W * H);
    
    for(int i = 0; i < W * H; i++)
    {
        if(trail[i] == -1)
        {
            break;
        }
        
        current = trail[i];
        
        drawMap(tiles, start, current, finish);
        
        getchar();
        system("clear");
    }
    
    printf("Done!\n");
    
    return 0;
}
