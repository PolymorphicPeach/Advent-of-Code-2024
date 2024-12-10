#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

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

bool isNumber(char* str){
    while(*str){
        if(!isdigit(*str)){
            return false;
        }
        ++str;
    }
    return true;
}


uint64_t part1(char* content) {
    uint64_t result = 0;
    uint64_t products[1024] = {0};
    int numProducts = 0;
    char* itr = content;

    while ((itr = strstr(itr, "mul(")) != NULL) {
        // Move by "mul("
        itr += 4;

        char* commaIdx = strchr(itr, ',');
        if (!commaIdx) {
            itr++;
            continue;
        }
        size_t len1 = commaIdx - itr;
        if (len1 > 3) {
            ++itr;
            continue;
        }
        char int1[3 + 1] = {0};
        strncpy(int1, itr, len1);

        if (!isdigit(int1[0])) {
            ++itr;
            continue;
        }

        // Parse second number
        char* closeParenIdx = strchr(commaIdx + 1, ')');
        if (!closeParenIdx) {
            ++itr;
            continue;
        }
        size_t len2 = closeParenIdx - (commaIdx + 1);
        if (len2 > 3) {
            ++itr;
            continue;
        }
        char int2[3 + 1] = {0};
        strncpy(int2, commaIdx + 1, len2);

        if (!isdigit(int2[0])) {
           ++itr;
            continue;
        }


        uint64_t product = strtoull(int1, NULL, 10) * strtoull(int2, NULL, 10);

        products[numProducts++] = product;
        ++itr;
    }

    for (int i = 0; i < numProducts; i++) {
        result += products[i];
    }

    return result;
}


uint64_t part2(char* content){
    uint64_t result = 0;
    uint64_t products[1024] = {0};
    int numProducts = 0;
    char* itr = content;
    bool doMul = true;

    while(*itr != '\0'){
        if(strncmp(itr, "do()", 4) == 0){
            doMul = true;
            itr += 4;
            continue;
        }
        if(strncmp(itr, "don't()", 7) == 0){
            doMul = false;
            itr += 7;
            continue;
        }

        // Basically the same as Part1
        if(doMul && strncmp(itr, "mul(", 4) == 0){
            itr += 4;

            char* commaIdx = strchr(itr, ',');
            if (!commaIdx) {
                ++itr;
                continue;
            }
            size_t len1 = commaIdx - itr;
            if (len1 > 3) {
                ++itr;
                continue;
            }
            char int1[3 + 1] = {0};
            strncpy(int1, itr, len1);

            if (!isdigit(int1[0])) {
                ++itr;
                continue;
            }

            // Parse second number
            char* closeParenIdx = strchr(commaIdx + 1, ')');
            if (!closeParenIdx) {
                ++itr;
                continue;
            }
            size_t len2 = closeParenIdx - (commaIdx + 1);
            if (len2 > 3) {
                ++itr;
                continue;
            }
            char int2[3 + 1] = {0};
            strncpy(int2, commaIdx + 1, len2);

            if (!isdigit(int2[0])) {
                ++itr;
                continue;
            }

            uint64_t product = strtoull(int1, NULL, 10) * strtoull(int2, NULL, 10);
            products[numProducts++] = product;
        }

        ++itr;
    }

    for (int i = 0; i < numProducts; i++) {
        result += products[i];
    }

    return result;
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
    fileReadEnd = clock();
    double fileReadTime = ((double)fileReadEnd - fileReadStart) / CLOCKS_PER_SEC;
    printf("Input retrieval: %f sec\n", fileReadTime);

    p1Start = clock();
    size_t fullLen = 0;
    for (int i = 0; i < numLines; ++i) {
        fullLen += strlen(fileContents[i]);
    }

    // +1 for null terminator at the end
    char* fullContents = malloc((fullLen + 1) * sizeof(char));

    size_t pos = 0;
    for (int i = 0; i < numLines; ++i) {
        size_t len = strlen(fileContents[i]);
        memcpy(&fullContents[pos], fileContents[i], len);
        pos += len;
    }
    fullContents[pos] = '\0';

    int result = part1(fullContents);
    p1End = clock();
    double p1Time = ((double) p1End - p1Start) / CLOCKS_PER_SEC;
    printf("Part1: %d (%f sec)\n", result, p1Time);


    p2Start = clock();
    int result2 = part2(fullContents);
    p2End = clock();
    double p2Time = ((double) p2End - p2Start) / CLOCKS_PER_SEC;
    printf("Part2: %d (%f sec)\n", result2, p2Time);

    return 0;
}