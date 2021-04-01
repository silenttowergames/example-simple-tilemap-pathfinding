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
    int current = start;
    int X;
    int Y;
    
    //visited[0] = -1;
    visited[0] = start;
    visited[1] = -1;
    int u = 1;
    
    int tLength;
    
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
            if(dirs[j] == 0 || tiles[dirs[j]] != 0 || (in_array(dirs[j], visited, length) != -1))
            {
                continue;
            }
            
            if(lowestTile == -1 || path[dirs[j]].toFinish < lowest)
            {
                lowestTile = dirs[j];
                lowest = path[dirs[j]].toFinish;
            }
        }
        
        if(lowestTile == -1)
        {
            assert(i >= 2);
            lowestTile = start;
            u--;
        }
        else
        {
            trail[u] = lowestTile;
            u++;
        }
        
        current = lowestTile;
        
        if(in_array(lowestTile, visited, W * H) == -1)
        {
            visited[i] = lowestTile;
        }
        
        if(current == finish)
        {
            tLength = i + 1;
            
            break;
        }
    }
    
    // Fix trail
    
    int oldX;
    int oldY;
    int cX;
    int cY;
    
    for(int i = 0; i < length; i++)
    {
        if(trail[i] == -1)
        {
            break;
        }
        
        rtile(trail[i], X, Y);
        
        // Get rid of gaps
        if(i > 0 && (abs(oldX - X) > 1 || abs(oldY - Y) > 1))
        {
            int j;
            for(j = 0; j < i; j++)
            {
                rtile(trail[j], cX, cY);
                
                if(abs(cX - X) <= 1 && abs(cY - X) <= 1)
                {
                    memcpy(&trail[j + 1], &trail[i], sizeof(int) * length - i + 1);
                    
                    if(tLength < length)
                    {
                        trail[length - i + 1] = -1;
                    }
                    
                    break;
                }
            }
            
            i = j + 1;
            
            continue;
        }
        
        oldX = X;
        oldY = Y;
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
    
    /*
    int tX;
    int tY;
    for(int i = 0; i < W * H; i++)
    {
        if(trail[i] == -1)
        {
            break;
        }
        
        rtile(trail[i], tX, tY);
        
        printf("%d:%dx%d ", i, tX, tY);
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
