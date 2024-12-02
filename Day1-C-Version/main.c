#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

char** getFileContents(const char *path, int *numLines){
    int maxLines = 2000;
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

char* skipWhiteSpace(char* str){
    while(*str && isspace((unsigned char) *str)){
        ++str;
    }
    return str;
}

void populateLeftRight(char** arr, char** left, char** right, int numLines){
    for(int i = 0; i < numLines; ++i){
        char* line = arr[i];
        line = skipWhiteSpace(line);

        char* whitespaceStart = line;
        while(*whitespaceStart && !isspace((unsigned char) *whitespaceStart)){
            ++whitespaceStart;
        }

        size_t leftLength = whitespaceStart - line;
        left[i] = malloc((leftLength + 1) * sizeof(char));
        strncpy(left[i], line, leftLength);
        left[i][leftLength] = '\0';

        char* rightStart = skipWhiteSpace(whitespaceStart);
        right[i] = strdup(rightStart);
    }
}

void free2DArray(char **arr, int numLines){
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

int compare(const void* a, const void* b){
    return (*(int*) a - *(int*) b);
}

int main(void) {
    clock_t fileReadStart;
    clock_t fileReadEnd;
    clock_t p1Start;
    clock_t p1End;
    clock_t p2Start;
    clock_t p2End;

    fileReadStart = clock();
    const char* filename = "../input.txt";
    int numLines = 0;
    char** fileContents = getFileContents(filename, &numLines);
    if(fileContents == NULL){
        printf("Failed to get file contents, exiting...\n");
        return 1;
    }
    fileReadEnd = clock();

    p1Start = clock();
    char** left = malloc(numLines * sizeof(char *));
    char** right = malloc(numLines * sizeof(char *));
    populateLeftRight(fileContents, left, right, numLines);
    int* l = malloc(numLines * sizeof(int));
    int* r = malloc(numLines * sizeof(int));

    for(int i = 0; i < numLines; ++i){
        l[i] = atoi(left[i]);
        r[i] = atoi(right[i]);
    }

    qsort(l, numLines, sizeof(int), compare);
    qsort(r, numLines, sizeof(int), compare);

    int distance = 0;
    for(int i = 0; i < numLines; ++i){
        distance += abs(l[i] - r[i]);
    }
    p1End = clock();


    p2Start = clock();
    int score = 0;
    for(int i = 0; i < numLines; ++i){
        int num = l[i];
        int freq = 0;

        for(int j = 0; j < numLines; ++j){
            if(r[j] == num){
                ++freq;
            }
            else if(r[j] > num){
                break;
            }
        }

        score += (num * freq);
    }
    p2End = clock();

    double fileReadTime = ((double) fileReadEnd - fileReadStart) / CLOCKS_PER_SEC;
    double p1Time = ((double) p1End - p1Start) / CLOCKS_PER_SEC;
    double p2Time = ((double) p2End - p2Start) / CLOCKS_PER_SEC;

    printf("Input retrieval: %f sec\n", fileReadTime);
    printf("Part1: %d (%f sec)\n", distance, p1Time);
    printf("Part2: %d (%f sec)\n", score, p2Time);

    return 0;
}