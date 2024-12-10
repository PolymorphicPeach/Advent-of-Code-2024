#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SEQUENCE 50
#define MAX_RULES 100

typedef struct{
    int before;
    int after;
}Rule;

Rule* populateRules(char** ordering, int numOrdering){
    Rule* rules = malloc(numOrdering * sizeof(Rule));

    for(int i = 0; i < numOrdering; ++i){
        char* token1 = strtok(ordering[i], "|");
        char* token2 = strtok(NULL, "|");
        char* endptr;
        int num1 = strtol(token1, &endptr, 10);
        int num2 = strtol(token2, &endptr, 10);

        rules[i].before = num1;
        rules[i].after = num2;
    }

    return rules;
}

int checkSequence(char* sequence, Rule* rules, int numRules){
    int seq[MAX_SEQUENCE];

    char* token = strtok(sequence, ",");
    int idx = 0;
    while(token != NULL){
        char* endptr;
        seq[idx] = strtol(token, &endptr, 10);
        ++idx;
        token = strtok(NULL, ",");
    }

    for(int i = 0; i < idx; ++i){
        for(int j = i; j < idx; ++j){
            for(int k = 0; k < numRules; ++k){
                if(rules[k].before == seq[j] && rules[k].after == seq[i]){
                    return 0;
                }
            }
        }
    }

    return seq[idx / 2];
}

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

void print2DArray(char** arr, int numLines){
    for(int i = 0; i < numLines; ++i){
        printf("%s\n", arr[i]);
    }
}

int part1 = 0;
int part2 = 0;

int fixSequence(char* sequence, Rule* rules, int numRules) {
    int seq[MAX_SEQUENCE];
    char sequenceCopy[strlen(sequence) + 1];
    strcpy(sequenceCopy, sequence);

    char* token = strtok(sequenceCopy, ",");
    int idx = 0;
    while (token != NULL) {
        char* endptr;
        seq[idx] = strtol(token, &endptr, 10);
        ++idx;
        token = strtok(NULL, ",");
    }

    printf("Incorrect Sequence: ");
    for(int i = 0; i < idx; ++i){
        printf("%d, ", seq[i]);
    }
    printf("\n");

    bool changed;
    do {
        changed = false;

        for (int i = 0; i < idx; ++i) {
            for (int j = 0; j < numRules; ++j) {
                for (int k = 0; k < idx - 1; ++k) {
                    if (rules[j].before == seq[k + 1] && rules[j].after == seq[k]) {
                        // Swap to fix ordering
                        int temp = seq[k];
                        seq[k] = seq[k + 1];
                        seq[k + 1] = temp;
                        changed = true;
                    }
                }
            }
        }

//        for (int i = 0; i < idx; ++i) {
//            for (int j = i + 1; j < idx; ++j) {
//                for (int k = 0; k < numRules; ++k) {
//                    if (rules[k].before == seq[j] && rules[k].after == seq[i]) {
//                        // Swap
//                        int temp = seq[i];
//                        seq[i] = seq[j];
//                        seq[j] = temp;
//                        changed = true;
//                    }
//                }
//            }
//        }
    } while (changed);

    printf("Correct Sequence:   ");
    for (int i = 0; i < idx; ++i) {
        printf("%d", seq[i]);
        if (i < idx - 1) {
            printf(", ");
        }
    }
    printf("\n");

    return seq[idx / 2];
}

int main(void) {
    clock_t start;
    clock_t end;

    start = clock();
    const char* orderingFile = "../input-ordering.txt";
    const char* sequencesFile = "../input-sequences.txt";
    int numOrdering = 0;
    int numSequences = 0;

    char** ordering = getFileContents(orderingFile, &numOrdering);
    char** sequences = getFileContents(sequencesFile, &numSequences);

    Rule* rules = populateRules(ordering, numOrdering);

    for(int i = 0; i < numSequences; ++i){
        //printf("Original Sequence: %s\n", sequences[i]);
        char* currentSequence = strdup(sequences[i]);
        int result = checkSequence(currentSequence, rules, numOrdering);

        if(result > 0){
            part1 += result;
        }
        else{
            //printf("Must fix: %s\n", sequences[i]);
            char* failedSequence = strdup(sequences[i]);
            int fixedResult = fixSequence(failedSequence, rules, numOrdering);
            part2 += fixedResult;
        }
    }
    end = clock();

    printf("Part1: %d, Part2: %d (%f sec)\n", part1, part2, (double) (end - start) / CLOCKS_PER_SEC);

    return 0;
}
