#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

char** fileToMatrix(const char* path, int* rows, int* cols, const char padding){
    int maxRows = 1024;
    int maxCols = 1024;
    FILE* file = fopen(path, "r");
    if(file == NULL){
        printf("Couldn't find file at %s\n", path);
        return NULL;
    }

    char** contents = malloc(maxRows * sizeof(char *));

    char row[maxCols];
    int rowCount = 0;

    while(fgets(row, sizeof(row), file) && rowCount < maxRows){
        row[strcspn(row, "\n")] = '\0';
        contents[rowCount] = malloc((strlen(row) + 1) * sizeof(char));
        strcpy(contents[rowCount], row);
        ++rowCount;
    }

    fclose(file);
    *rows = rowCount;
    // second row in case I decide to add padding
    *cols = strlen(contents[1]);
    return contents;
}

char** getFileContents(const char* path, int* numLines){
    int maxLines = 1100;
    int maxChars = 100;

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

void free2DArray(char** arr, int numLines){
    for(int i = 0; i < numLines; ++i){
        free(arr[i]);
    }
    free(arr);
}

void print2DArray(char **arr, int numLines){
    for(int i = 0; i < numLines; ++i){
        printf("%s\n", arr[i]);
    }
}

int part1Count = 0;
int part2Count = 0;

void searchPart1(const char** matrix, const int row, const int col,
                 const int maxRow, const int maxCol, char currentLetter, char* direction){
    if(currentLetter == 'S'){
        ++part1Count;
        return;
    }

    char targetLetter = '_';

    if(currentLetter == 'X'){
        targetLetter = 'M';
    }
    else if(currentLetter == 'M'){
        targetLetter = 'A';
    }
    else if(currentLetter == 'A'){
        targetLetter = 'S';
    }

    if(row > 0 && matrix[row - 1][col] == targetLetter && strcmp(direction, "n") == 0){
        searchPart1(matrix, row - 1, col, maxRow, maxCol, targetLetter, "n");
    }
    if(row < maxRow - 1 && matrix[row + 1][col] == targetLetter && strcmp(direction, "s") == 0){
        searchPart1(matrix, row + 1, col, maxRow, maxCol, targetLetter, "s");
    }
    if(col > 0 && matrix[row][col - 1] == targetLetter&& strcmp(direction, "w") == 0 ){
        searchPart1(matrix, row, col - 1, maxRow, maxCol, targetLetter, "w");
    }
    if(col < maxCol - 1 && matrix[row][col + 1] == targetLetter && strcmp(direction, "e") == 0){
        searchPart1(matrix, row, col + 1, maxRow, maxCol, targetLetter, "e");
    }

    if(row > 0 && col < maxCol - 1 && matrix[row - 1][col + 1] == targetLetter && strcmp(direction, "ne") == 0){
        searchPart1(matrix, row - 1, col + 1, maxRow, maxCol, targetLetter, "ne");
    }
    if(row < maxRow - 1 && col < maxCol - 1 && matrix[row + 1][col + 1] == targetLetter&& strcmp(direction, "se") == 0 ){
        searchPart1(matrix, row + 1, col + 1, maxRow, maxCol, targetLetter, "se");
    }
    if(row < maxRow - 1 && col > 0 && matrix[row + 1][col - 1] == targetLetter && strcmp(direction, "sw") == 0){
        searchPart1(matrix, row + 1, col - 1, maxRow, maxCol, targetLetter, "sw");
    }
    if(row > 0 && col > 0 && matrix[row - 1][col - 1] == targetLetter && strcmp(direction, "nw") == 0){
        searchPart1(matrix, row - 1, col - 1, maxRow, maxCol, targetLetter, "nw");
    }
}

void iterate(char** matrix, const int maxRow, const int maxCol){
    for(int i = 0; i < maxRow; ++i){
        for(int j = 0; j < maxCol; ++j){
            if(matrix[i][j] == 'X'){
                char currentLetter = 'X';
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "n");
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "s");
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "e");
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "w");
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "ne");
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "se");
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "sw");
                searchPart1(matrix, i, j, maxRow, maxCol, currentLetter, "nw");
            }
        }
    }
}

void searchPart2(char** matrix, const int row, const int col){
    char nwLetter = matrix[row - 1][col - 1];
    char neLetter = matrix[row - 1][col + 1];
    char seLetter = matrix[row + 1][col + 1];
    char swLetter = matrix[row + 1][col - 1];

    bool nwM = nwLetter == 'M';
    bool neM = neLetter == 'M';
    bool seM = seLetter == 'M';
    bool swM = swLetter == 'M';

    bool nwS = nwLetter == 'S';
    bool neS = neLetter == 'S';
    bool seS = seLetter == 'S';
    bool swS = swLetter == 'S';

    if(nwM && swM && neS && seS){
        ++part2Count;
    }
    if(nwM && neM && seS && swS){
        ++part2Count;
    }
    if(nwS && neM && seM && swS){
        ++part2Count;
    }
    if(nwS && neS && seM && swM){
        ++part2Count;
    }
}

void iterateP2(char** matrix, const int maxRow, const int maxCol){
    for(int i = 1; i < maxRow - 1; ++i){
        for(int j = 1; j < maxCol - 1; ++j){
            if(matrix[i][j] == 'A'){
                searchPart2(matrix, i, j);
            }
        }
    }
}



int main(void) {
    clock_t start;
    clock_t end;

    const char* filename = "../input.txt";
    int rows = 0;
    int cols = 0;
    char** matrix = fileToMatrix(filename, &rows, &cols, '.');

    start = clock();
    iterate(matrix, rows, cols);
    end = clock();
    printf("Part1: %d (%f sec)\n", part1Count, ((double) end - start) / CLOCKS_PER_SEC);

    start = clock();
    iterateP2(matrix, rows, cols);
    end = clock();
    printf("Part2: %d (%f sec)\n", part2Count, ((double) end - start) / CLOCKS_PER_SEC);


    return 0;
}
