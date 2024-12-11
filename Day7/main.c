#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

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
    *numLines = lineCount;
    return contents;
}

typedef struct Node{
    uint64_t value;
    struct Node* plus;
    struct Node* times;
    struct Node* concat;
}Node;

typedef struct Tree{
    struct Node* root;
}Tree;

Tree* createTree(uint64_t rootValue){
    Tree* tree = malloc(sizeof(Tree));
    Node* root = malloc(sizeof(Node));
    root->value = rootValue;
    root->plus = NULL;
    root->times = NULL;
    root->concat = NULL;
    tree->root = root;
    return tree;
}

void fillTreeRecursive(Node* node, uint64_t* nums, int currentIdx, int totalCount, uint64_t goal){
    if (currentIdx >= totalCount || node->value > goal) {
        return;
    }

    node->plus = malloc(sizeof(Node));
    node->plus->value = node->value + nums[currentIdx];
    node->plus->plus = NULL;
    node->plus->times = NULL;

    node->times = malloc(sizeof(Node));
    node->times->value = node->value * nums[currentIdx];
    node->times->plus = NULL;
    node->times->times = NULL;

    fillTreeRecursive(node->plus, nums, currentIdx + 1, totalCount, goal);
    fillTreeRecursive(node->times, nums, currentIdx + 1, totalCount, goal);
}

void fillTree(Tree* tree, uint64_t* nums, int totalCount, uint64_t goal){
    fillTreeRecursive(tree->root, nums, 1, totalCount, goal);
}

bool checkGoal(Node* node, uint64_t goal){
    if(!node){
        //printf("NULL Node. Returning false\n");
        return false;
    }
    //printf("Visiting Node: %llu\n",node->value);

    if(!node->plus && !node->times){
        //printf("Leaf Node: value=%llu, goal=%llu. Result=%s\n", node->value, goal, (node->value == goal ? "true" : "false"));
        return node->value == goal;
    }
    //printf("Checking children of Node with value=%llu for goal=%llu\n", node->value, goal);

    bool result = checkGoal(node->plus, goal) ||
                  checkGoal(node->times, goal) ||
                  checkGoal(node->concat, goal);
    //printf("Result for Node with value=%llu: %s\n", node->value, (result ? "true" : "false"));
    return result;
}

void freeTree(Node* node){
    if(!node){
        return;
    }
    freeTree(node->plus);
    freeTree(node->times);
    free(node);
}

uint64_t p1ProcessLine(char* line){
    char lineCopy[100];
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';

    char* token = strtok(lineCopy, ":");
    uint64_t goal = strtoull(token, NULL, 10);

    token = strtok(NULL, " ");
    uint64_t nums[50];
    int count = 0;

    while(token != NULL){
        nums[count] = strtoull(token, NULL, 10);
        ++count;
        token = strtok(NULL, " ");
    }

    Tree* tree = createTree(nums[0]);
    fillTree(tree, nums, count, goal);
    uint64_t result = 0;
    if(checkGoal(tree->root, goal)){
        result = goal;
    }
    freeTree(tree->root);
    free(tree);
    return result;
}


uint64_t concatNumbers(uint64_t num1, uint64_t num2){
    char num1Buffer[21];
    char num2Buffer[21];
    char concatBuffer[41];

    snprintf(num1Buffer, sizeof(num1Buffer), "%" PRIu64, num1);
    snprintf(num2Buffer, sizeof(num2Buffer), "%" PRIu64, num2);
    //printf("num1Buffer: %s, num2Buffer: %s\n", num1Buffer, num2Buffer);

    int totalDigits = 0;
    for(int i = 0; num1Buffer[i] != '\0'; ++i){
        ++totalDigits;
    }
    for(int i = 0; num2Buffer[i] != '\0'; ++i){
        ++totalDigits;
    }

    if(totalDigits > 20){
        return 0;
    }

    snprintf(concatBuffer, sizeof(concatBuffer), "%s%s", num1Buffer, num2Buffer);
    //printf("concat to: %" PRIu64 "\n", strtoull(concatBuffer, NULL, 10));

    return strtoull(concatBuffer, NULL, 10);
}

void fillTreeRecursiveP2(Node* node, uint64_t* nums, int currentIdx, int totalCount, uint64_t goal){
    if (!node || currentIdx >= totalCount || node->value > goal) {
        return;
    }

    node->plus = malloc(sizeof(Node));
    node->plus->value = node->value + nums[currentIdx];
    node->plus->plus = NULL;
    node->plus->times = NULL;
    node->plus->concat = NULL;

    node->times = malloc(sizeof(Node));
    node->times->value = node->value * nums[currentIdx];
    node->times->plus = NULL;
    node->times->times = NULL;
    node->times->concat = NULL;

    uint64_t concatNum = concatNumbers(node->value, nums[currentIdx]);
    if(concatNum > 0 && concatNum <= goal && node->value > 0){
        node->concat = malloc(sizeof(Node));
        node->concat->value = concatNum;
        node->concat->plus = NULL;
        node->concat->times = NULL;
        node->concat->concat = NULL;
    }

    fillTreeRecursiveP2(node->concat, nums, currentIdx + 1, totalCount, goal);
    fillTreeRecursiveP2(node->plus, nums, currentIdx + 1, totalCount, goal);
    fillTreeRecursiveP2(node->times, nums, currentIdx + 1, totalCount, goal);
}

void fillTreeP2(Tree* tree, uint64_t* nums, int totalCount, uint64_t goal){
    fillTreeRecursiveP2(tree->root, nums, 1, totalCount, goal);
}

uint64_t p2ProcessLine(char* line){
    char lineCopy[100];
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';

    char* token = strtok(lineCopy, ":");
    uint64_t goal = strtoull(token, NULL, 10);

    token = strtok(NULL, " ");
    uint64_t nums[50];
    int count = 0;

    while(token != NULL){
        nums[count] = strtoull(token, NULL, 10);
        ++count;
        token = strtok(NULL, " ");
    }

    Tree* tree = createTree(nums[0]);
    fillTreeP2(tree, nums, count, goal);
    uint64_t result = 0;
    if(checkGoal(tree->root, goal)){
        result = goal;
    }
    freeTree(tree->root);
    free(tree);
    return result;
}

int main(void) {
    const char* filename = "../input.txt";
    int numLines = 0;
    char** fileContents = getFileContents(filename, &numLines);

    uint64_t part1 = 0;
    uint64_t part2 = 0;
    for(int i = 0; i < numLines; ++i){
        uint64_t p1Result = p1ProcessLine(fileContents[i]);
        if(p1Result > 0){
            part1 += p1Result;
        }
        else{
            part2 += p2ProcessLine(fileContents[i]);
        }
    }
    printf("Part1: %" PRIu64 "\n", part1);
    printf("Part2: %" PRIu64 "\n", part1 + part2);

    return 0;
}
