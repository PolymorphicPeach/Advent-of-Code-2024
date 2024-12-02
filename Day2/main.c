#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

char** getFileContents(const char *path, int *numLines){
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

int isSafe(char* line){
    int numInts = 20;

    int* intArr = malloc(numInts * sizeof(int));

    const char* itr = line;
    int idx = 0;

    while(*itr != '\0'){
        while(isspace((unsigned char) *itr)){
            ++itr;
        }
        if(*itr != '\0'){
            intArr[idx++] = strtol(itr, (char **) &itr, 10);
        }
    }

    bool increasing = true;

    int firstInt = intArr[0];
    int secondInt = intArr[1];

    if(firstInt > secondInt){
        increasing = false;
    }
    else if(firstInt == secondInt){
        return 0;
    }

    for(int i = 0; i < idx - 1; ++i){
        int first = intArr[i];
        int second = intArr[i + 1];

        if(first == second){
            return 0;
        }

        if(increasing){
            if(first > second){
                return 0;
            }
            if(first < second - 3){
                return 0;
            }
        }
        else{
            if(first < second){
                return 0;
            }
            if(first > second + 3){
                return 0;
            }
        }
    }

    free(intArr);
    return 1;
}

int isSafeP2(char* line){
    int numInts = 20;
    int* intArr = malloc(numInts * sizeof(int));
    const char* itr = line;
    int idx = 0;

    while(*itr != '\0'){
        while(isspace((unsigned char) *itr)){
            ++itr;
        }
        if(*itr != '\0'){
            intArr[idx++] = strtol(itr, (char **) &itr, 10);
        }
    }

    int* temp = malloc((idx - 1) * sizeof(int));

    for(int ex = 0; ex < idx; ++ex){

        int tempIdx = 0;
        for(int i = 0; i < idx; ++i){
            if(i != ex){
                temp[tempIdx++] = intArr[i];
            }
        }

        bool valid = true;
        bool increasing = true;
        int firstInt = temp[0];
        int secondInt = temp[1];

        if(firstInt > secondInt){
            increasing = false;
        }
        else if(firstInt == secondInt){
            valid = false;
        }

        for(int i = 0; i < tempIdx - 1; ++i){
            int first = temp[i];
            int second = temp[i + 1];

            if(first == second){
                valid = false;
                break;
            }
            if(increasing){
                if(first > second){
                    valid = false;
                    break;
                }
                if(first < second - 3){
                    valid = false;
                    break;
                }
            }
            else{
                if(first < second){
                    valid = false;
                    break;
                }
                if(first > second + 3){
                    valid = false;
                    break;
                }
            }
        }

        if(valid){
            free(temp);
            free(intArr);
            return 1;
        }
    }

    free(temp);
    free(intArr);
    return 0;
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
    int safeLines = 0;
    for(int i = 0; i < numLines; ++i){
        safeLines += isSafe(fileContents[i]);
    }
    p1End = clock();

    p2Start = clock();
    int safeLines2 = 0;
    for(int i = 0; i < numLines; ++i){
        if(isSafe(fileContents[i])){
            ++safeLines2;
        }
        else{
            safeLines2 += isSafeP2(fileContents[i]);
        }
    }
    p2End = clock();

    double fileReadTime = ((double) fileReadEnd - fileReadStart) / CLOCKS_PER_SEC;
    double p1Time = ((double) p1End - p1Start) / CLOCKS_PER_SEC;
    double p2Time = ((double) p2End - p2Start) / CLOCKS_PER_SEC;

    printf("Input retrieval: %f sec\n", fileReadTime);
    printf("Part1: %d (%f sec)\n", safeLines, p1Time);
    printf("Part2: %d (%f sec)\n", safeLines2, p2Time);

    return 0;
}
