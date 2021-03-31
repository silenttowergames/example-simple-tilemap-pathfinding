#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define W 16
#define H 12

#define tile(X, Y) (X + (W * (Y)))
#define rtile(T, X, Y) { X = (int)T % W; Y = (int)floorf(T / W); }

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

int in_array(int v, int* array, int length)
{
    for(int i = 0; i < length; i++)
    {
        if(array[i] == v)
        {
            return 1;
        }
    }
    
    return 0;
}

int takeNextStep(int* tiles, int* visited, int visitedI, int start, int current, int finish)
{
    int FX;
    int FY;
    rtile(finish, FX, FY);
    
    int X;
    int Y;
    rtile(current, X, Y);
    
    float difference[4] = {
        // Up
        tile(X, Y - 1),
        // Right
        tile(X + 1, Y),
        // Down
        tile(X, Y + 1),
        // Left
        tile(X - 1, Y),
    };
    float differenceVal[4];
    
    float lowestVal = -1;
    int lowest = -1;
    for(int i = 0; i < sizeof(difference) / sizeof(float); i++)
    {
        if(difference[i] < 0 || difference[i] >= W * H || tiles[(int)difference[i]] == 1 || in_array(difference[i], visited, W * H))
        {
            differenceVal[i] = 0;
            
            continue;
        }
        
        rtile(difference[i], X, Y);
        
        differenceVal[i] = sqrtf(
            powf(X - FX, 2)
            +
            powf(Y - FY, 2)
        );
        
        if(lowest == -1 || differenceVal[i] < lowestVal)
        {
            lowestVal = differenceVal[i];
            lowest = i;
        }
    }
    
    assert(lowest != -1);
    current = difference[lowest];
    
    visited[visitedI] = difference[lowest];
    
    return current;
}

int main()
{
    // FIXME: Dies at dead ends! Needs to get the list, THEN go down it visually!
    int tiles[W * H] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    int visited[W * H];
    memset(visited, -1, sizeof(visited));
    int visitedI = 0;
    
    const int start = tile(1, 1);
    const int finish = tile(14, 10);
    int current = -1;
    
    do {
        system("clear");
        
        if(current == -1)
        {
            current = start;
        }
        else
        {
            current = takeNextStep(tiles, visited, visitedI, start, current, finish);
            
            visitedI++;
        }
        
        drawMap(tiles, start, current, finish);
        
        getchar();
    }
    while(current != finish);
    
    return 0;
}
