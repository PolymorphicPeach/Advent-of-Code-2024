#ifndef DAY9_DAY9_H
#define DAY9_DAY9_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct{
    int id;
}Block;

typedef struct{
    Block* memory;
    int size;
    int capacity;
} Memory;

Memory createMemory(char* str);
Block createBlock(int id);
void printMemory(Memory memory);
void packMemory(Memory* memory);
uint64_t getChecksum(Memory memory);
void packMemory2(Memory* memory);
#endif
