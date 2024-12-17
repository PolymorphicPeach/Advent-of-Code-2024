#include "Day9.h"

Block createBlock(int id){
    Block block;
    block.id = id;
    return block;
}

Memory createMemory(char* str){
    Memory mem;
    mem.size = 0;
    mem.capacity = 50;
    mem.memory = malloc(mem.capacity * sizeof(Block));

    bool inMemory = true;
    int strIdx = 0;
    int nextId = 0;

    while(str[strIdx] != '\0'){
        int blockSize = str[strIdx] - '0';

        if(inMemory){
            for(int i = 0; i < blockSize; ++i){
                if(mem.size >= mem.capacity){
                    int newCapacity = mem.capacity += 50;
                    mem.memory = realloc(mem.memory, newCapacity * sizeof(Block));
                }
                mem.memory[mem.size] = createBlock(nextId);
                ++mem.size;
            }
            ++nextId;
        }
        else{
            for(int i = 0; i < blockSize; ++i){
                if(mem.size >= mem.capacity){
                    int newCapacity = mem.capacity += 50;
                    mem.memory = realloc(mem.memory, newCapacity * sizeof(Block));
                }
                mem.memory[mem.size] = createBlock(-1);
                ++mem.size;
            }
        }

        ++strIdx;
        inMemory = !inMemory;
    }
    //printf("Returning Memory: size = %d, memory = %d\n", mem.size, mem.capacity);
    return mem;
}

void printMemory(Memory memory){
    for(int i = 0; i < memory.size; ++i){
        if(memory.memory[i].id >= 0){
            printf("%d", memory.memory[i].id);
        }
        else{
            printf(".");
        }
    }
    printf("\n");
}

void packMemory(Memory* memory){
    int freeBlockIdx = 0;
    int validBlockIdx = memory->size - 1;

    while(freeBlockIdx < validBlockIdx){
        while(memory->memory[freeBlockIdx].id != -1){
            ++freeBlockIdx;
        }
        while(memory->memory[validBlockIdx].id == -1){
            --validBlockIdx;
        }
        memory->memory[freeBlockIdx].id = memory->memory[validBlockIdx].id;
        memory->memory[validBlockIdx].id = -1;
    }
    //printf("freeBlockIdx = %d, validBlockIdx = %d\n", freeBlockIdx, validBlockIdx);

    if(memory->memory[freeBlockIdx].id != -1 && memory->memory[validBlockIdx].id == -1){
        memory->memory[validBlockIdx].id = memory->memory[freeBlockIdx].id;
        memory->memory[freeBlockIdx].id = -1;
    }

}

uint64_t getChecksum(Memory memory){
    uint64_t checksum = 0;
    for(int i = 0; i < memory.size; ++i){
        if(memory.memory[i].id != -1){
            uint64_t temp = i * memory.memory[i].id;
            checksum += temp;
        }
    }
    return checksum;
}
int* getLeftFreeBlock(Memory* memory, int end, int size){
    //printf("getLeftFreeBlock: end = %d, size = %d\n", end, size);
    int* indexes = malloc(2 * sizeof(int));
    for(int i = 0; i < end; ++i){
        if(memory->memory[i].id == -1){
            int currStart = i;
            int currEnd = i + 1;
            //printf("getLeftFreeBlock: [%d, %d)\n", currStart, currEnd);
            while(memory->memory[currEnd].id == -1){
                ++currEnd;
            }

            int freeSize = currEnd - currStart;

            if(freeSize >= size){
                indexes[0] = currStart;
                indexes[1] = currEnd;
                //printf("Returning...\n");
                //printf("getLeftFreeBlock: [%d, %d)\n", currStart, currEnd);
                return indexes;
            }
        }
    }
    indexes[0] = -1;
    indexes[1] = -1;
    return indexes;
}

int* getFileBlock(Memory* memory, int id){
    int* indexes = malloc(2 * sizeof(int));
    int start = 0;
    while(memory->memory[start].id != id){
        ++start;
    }
    int end = start;
    while(memory->memory[end].id == id){
        ++end;
    }
    indexes[0] = start;
    indexes[1] = end;
    return indexes;
}

void packMemory2(Memory* memory){
    int maxId = memory->memory[memory->size - 1].id;
    //printf("max id: %d\n", maxId);

    for (int currId = maxId; currId > 0; --currId) {
        int* currBlock = getFileBlock(memory, currId);
        if (currBlock == NULL){
            continue;
        }
        //printf("currBlock: [%d, %d)\n", currBlock[0], currBlock[1]);
        int* freeBlock = getLeftFreeBlock(memory, currBlock[0], currBlock[1] - currBlock[0]);
        if (freeBlock == NULL) {
            continue;
        }
        //printf("freeBlocks: [%d, %d)\n", freeBlock[0], freeBlock[1]);
        if (freeBlock[0] != -1) {
            int fileSize = currBlock[1] - currBlock[0];
            for (int i = 0; i < fileSize; ++i) {
                memory->memory[freeBlock[0] + i].id = currId;
                memory->memory[currBlock[0] + i].id = -1;
            }
        }

        free(currBlock);
        free(freeBlock);
    }
}