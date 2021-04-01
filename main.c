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
} PFInfo;

void drawMap(int* tiles, int start, int current, int finish)
{
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
    int startX;
    int startY;
    rtile(start, startX, startY);
    
    int finishX;
    int finishY;
    rtile(finish, finishX, finishY);
    
    int X;
    int Y;
    for(int i = 0; i < length; i++)
    {
        if(tiles[i] != 0)
        {
            continue;
        }
        
        rtile(i, X, Y);
        
        path[i].toStart = sqrtf(abs(powf(startX - X, 2)) + abs(powf(startY - Y, 2)));
        path[i].toFinish = sqrtf(abs(powf(finishX - X, 2)) + abs(powf(finishY - Y, 2)));
    }
}

int in_array(int v, int* array, int length)
{
    for(int i = 0; i < length; i++)
    {
        if(array[i] == -1)
        {
            return 0;
        }
        
        if(array[i] == v)
        {
            return 1;
        }
    }
    
    return 0;
}

void getTrail(int* trail, int* tiles, int* visited, PFInfo* path, int start, int finish, int length)
{
    int current = start;
    int X;
    int Y;
    
    visited[0] = start;
    visited[1] = -1;
    int u = 0;
    
    for(int i = 0; i < length; i++)
    {
        //printf("%d\n", i);
        
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
        const char* dirsS[8] = {
            "Up",
            "Up right",
            "Up left",
            "Left",
            "Right",
            "Down",
            "Down right",
            "Down left",
        };
        
        float lowest;
        int lowestTile = -1;
        for(int j = 0; j < sizeof(dirs) / sizeof(int); j++)
        {
            if(dirs[j] == 0 || tiles[dirs[j]] != 0 || in_array(dirs[j], visited, length))
            {
                continue;
            }
            
            if(lowestTile == -1 || path[dirs[j]].toFinish < lowest)
            {
                //printf("%d: j = %d ? `%s`\n", i, j, dirsS[j]);
                lowestTile = dirs[j];
                lowest = path[dirs[j]].toFinish;
            }
        }
        
        if(lowestTile == -1)
        {
            lowestTile = visited[i - 2];
            u--;
        }
        else
        {
            trail[u] = lowestTile;
            u++;
        }
        
        current = lowestTile;
        
        if(!in_array(lowestTile, visited, W * H))
        {
            visited[i] = lowestTile;
        }
        
        drawMap(tiles, start, current, finish);
        printf("%d\n", i);
        getchar();
        system("clear");
        
        if(current == finish)
        {
            break;
        }
    }
}

int main()
{
    PFInfo path[W * H];
    memset(path, 0, sizeof(path));
    
    int trail[W * H];
    memset(trail, -1, sizeof(trail));
    
    int visited[W * H];
    memset(trail, -1, sizeof(trail));
    
    int tiles[W * H] = {
        /*
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1,
        1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        //*/
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    
    //printf("Hello, World! %d %d\n", W, H);
    
    const int start = tile(1, 1);
    const int finish = tile(14, 10);
    int current;
    
    fillPathValues(path, tiles, start, finish, W * H);
    
    getTrail(trail, tiles, visited, path, start, finish, W * H);
    
    /*
    for(int i = 0; i < W * H; i++)
    {
        if(trail[i] == -1)
        {
            break;
        }
        
        printf("%d ", trail[i]);
    }
    printf("\n");
    //*/
    
    for(int i = 0; i < W * H; i++)
    {
        if(trail[i] == -1)
        {
            break;
        }
        
        current = trail[i];
        
        drawMap(tiles, start, current, finish);
        printf("%d\n", i);
        
        getchar();
        system("clear");
    }
    
    return 0;
}
