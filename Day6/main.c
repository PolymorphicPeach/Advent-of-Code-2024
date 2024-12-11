#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

void print2DArray(char** arr, int numLines){
    for(int i = 0; i < numLines; ++i){
        int size = strlen(arr[i]);
        //printf("Size: %d\n", size);
        printf("%s\n", arr[i]);
    }
}

char** getFileContents(const char *path, int *numLines){
    int maxLines = 1100;
    int maxChars = 300;

    FILE* file = fopen(path, "r");

    if(file == NULL){
        printf("Couldn't read file contents.\n");
        return NULL;
    }

    char** contents = malloc(maxLines * sizeof(char *));

    if(contents == NULL){
        printf("Couldn't allocate memory for file contents.\n");
        fclose(file);
        return NULL;
    }

    char line[maxChars];
    int lineCount = 0;

    while(fgets(line, sizeof(line), file) && lineCount < maxLines){
        // Replace newline with null terminator
        line[strcspn(line, "\n")] = '\0';
        contents[lineCount] = malloc((strlen(line) + 1) * sizeof(char));

        if (contents[lineCount] == NULL){
            printf("Couldn't allocate memory for file line.\n");
            fclose(file);
            return NULL;
        }

        strcpy(contents[lineCount], line);
        ++lineCount;
    }

    fclose(file);
    // Update numLines outside of scope
    *numLines = lineCount;
    return contents;
}

enum Direction{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

typedef struct{
    int x;
    int y;
    enum Direction dir;
} Guard;

Guard* findGuard(char** map, int rows){
    Guard* guard = malloc(sizeof(Guard));

    for(int i = 0; i < rows; ++i){
        size_t cols = strlen(map[i]);
        for(int j = 0; j < cols; ++j){
            if(map[i][j] == 'v' || map[i][j] == '^'
            || map[i][j] == '<' || map[i][j] == '>'){
                guard->y = i;
                guard->x = j;

                if(map[i][j] == 'v'){
                    guard->dir = SOUTH;
                }
                else if(map[i][j] == '^'){
                    guard->dir = NORTH;
                }
                else if(map[i][j] == '<'){
                    guard->dir = WEST;
                }
                else if(map[i][j] == '>'){
                    guard->dir = EAST;
                }

                return guard;
            }
        }
    }
    guard->x = -1;
    guard->y = -1;
    guard->dir = NORTH;
    return guard;
}

void rotate(Guard* guard){
    if(guard->dir == NORTH){
        guard->dir == EAST;
    }
    else if(guard->dir == SOUTH){
        guard->dir == WEST;
    }
    else if(guard->dir == EAST){
        guard->dir == SOUTH;
    }
    else if(guard->dir == WEST){
        guard->dir == NORTH;
    }
}

void step(Guard* guard, char** map, int rows){
   // printf("Iteration:\n");
    //printf("Current Pos: x = %d,  y = %d\n", guard->x, guard->y);
    //print2DArray(map, rows);

    int maxCols = strlen(map[guard->y]);
    int nextX = guard->x;
    int nextY = guard->y;

    switch(guard->dir){
        case NORTH:
            map[guard->y][guard->x] = '^';
            nextY -= 1;
            //printf("%c in next position\n", map[nextY][nextX]);
            if(nextY < 0){
                map[guard->y][guard->x] = 'X';
                return;
            }
            break;
        case SOUTH:
            map[guard->y][guard->x] = 'v';
            nextY += 1;
           // printf("%c in next position\n", map[nextY][nextX]);
            if(nextY >= rows){
                map[guard->y][guard->x] = 'X';
                return;
            }
            break;
        case EAST:
            map[guard->y][guard->x] = '>';
            nextX += 1;
          //  printf("%c in next position\n", map[nextY][nextX]);
            if(nextX >= maxCols){
                map[guard->y][guard->x] = 'X';
                return;
            }
            break;
        case WEST:
            map[guard->y][guard->x] = '<';
            nextX -= 1;
           // printf("%c in next position\n", map[nextY][nextX]);
            if(nextX < 0){
                map[guard->y][guard->x] = 'X';
                return;
            }
            break;
    }

    if(nextY >= 0 && nextY < rows && nextX >= 0 && nextX < maxCols && map[nextY][nextX] != '#'){
        map[guard->y][guard->x] = 'X';
        guard->x = nextX;
        guard->y = nextY;
        //printf("Valid position, marking X\n");
    }
    else{
        //printf("Must rotate.\n");
        if(guard->dir == NORTH){
            guard->dir = EAST;
        }
        else if(guard->dir == EAST){
            guard->dir = SOUTH;
        }
        else if(guard->dir == SOUTH){
            guard->dir = WEST;
        }
        else if(guard->dir == WEST){
            guard->dir = NORTH;
        }
    }
    //printf("Stepping...\n\n");
    step(guard, map, rows);
}

void part1(char** map, int rows){
    printf("Entering part1 function\n");
    Guard* guard = findGuard(map, rows);
    step(guard, map, rows);
}

int countX(char** map, int rows){
    int numX = 0;
    for(int i = 0; i < rows; ++i){
        int cols = strlen(map[i]);
        for(int j = 0; j < cols; ++j){
            if(map[i][j] == 'X'){
                ++numX;
            }
        }
    }
    return numX;
}

int loops = 0;

void part2Iteration(char** map, int** timesVisited, int rows, Guard* guard) {
    if (timesVisited[guard->y][guard->x] > 4) {
        //printf("Found loop!\n");
        ++loops;
        return;
    }

    int maxCols = strlen(map[guard->y]);
    int nextX = guard->x;
    int nextY = guard->y;

    switch(guard->dir){
        case NORTH:
            map[guard->y][guard->x] = '^';
            nextY -= 1;
            //printf("%c in next position\n", map[nextY][nextX]);
            if(nextY < 0){
                return;
            }
            break;
        case SOUTH:
            map[guard->y][guard->x] = 'v';
            nextY += 1;
            // printf("%c in next position\n", map[nextY][nextX]);
            if(nextY >= rows){
                return;
            }
            break;
        case EAST:
            map[guard->y][guard->x] = '>';
            nextX += 1;
            //  printf("%c in next position\n", map[nextY][nextX]);
            if(nextX >= maxCols){
                return;
            }
            break;
        case WEST:
            map[guard->y][guard->x] = '<';
            nextX -= 1;
            // printf("%c in next position\n", map[nextY][nextX]);
            if(nextX < 0){
                return;
            }
            break;
    }

    if (nextY >= 0 && nextY < rows && nextX >= 0 && nextX < maxCols && map[nextY][nextX] != '#') {
        timesVisited[guard->y][guard->x] += 1;
        guard->x = nextX;
        guard->y = nextY;
    }
    else {
        // Rotate direction
        if (guard->dir == NORTH){
            guard->dir = EAST;
        }
        else if (guard->dir == EAST){
            guard->dir = SOUTH;
        }
        else if (guard->dir == SOUTH){
            guard->dir = WEST;
        }
        else if (guard->dir == WEST){
            guard->dir = NORTH;
        }
    }

    part2Iteration(map, timesVisited, rows, guard);
}


void part2(char** map, int rows, Guard originalGuard){
    for(int i = 0; i < rows; ++i){
        int cols = strlen(map[i]);
        for(int j = 0; j < cols; ++j){
            if(map[i][j] == 'X' && !(originalGuard.x == j && originalGuard.y == i)){
                char** newMap = malloc(rows * sizeof(char*));
                for(int a = 0; a < rows; ++a){
                    newMap[a] = strdup(map[a]);
                }

                int** timesVisited = malloc(rows * sizeof(int*));
                for(int a = 0; a < rows; ++a){
                    timesVisited[a] = malloc(cols * sizeof(int));
                    for(int b = 0; b < cols; ++b){
                        timesVisited[a][b] = 0;
                    }
                }

                Guard* newGuard = malloc(sizeof(Guard));
                newGuard->x = originalGuard.x;
                newGuard->y = originalGuard.y;
                newGuard->dir = originalGuard.dir;
                newMap[i][j] = '#';

                part2Iteration(newMap, timesVisited, rows, newGuard);

                free(newGuard);
                for(int a = 0; a < rows; ++a){
                    free(newMap[a]);
                }
                free(newMap);
                for(int a = 0; a < rows; ++a){
                    free(timesVisited[a]);
                }
                free(timesVisited);

            }
        }
    }
}

int main(void) {
    clock_t start;
    clock_t end;

    start = clock();
    int numLines = 0;
    const char* filename = "../input.txt";
    char** map1 = getFileContents(filename, &numLines);
    Guard* originalGuardPtr = findGuard(map1, numLines);
    Guard originalGuard;
    originalGuard.x = originalGuardPtr->x;
    originalGuard.y = originalGuardPtr->y;
    originalGuard.dir = originalGuardPtr->dir;

    //print2DArray(fileContents, numLines);
    part1(map1, numLines);
    int p1 = countX(map1, numLines);
    end = clock();
    printf("Part1: %d (%f sec)\n", p1, ((double) end - start) / CLOCKS_PER_SEC);

    start = clock();
    part2(map1, numLines, originalGuard);
    end = clock();
    printf("Part2: %d (%f sec)\n", loops, ((double) end - start) / CLOCKS_PER_SEC);

    return 0;
}
