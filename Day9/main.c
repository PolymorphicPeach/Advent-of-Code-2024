#include <stdio.h>
#include <inttypes.h>
#include "Utility.h"
#include "Day9.h"

int main(void) {
    const char* filename = "../input.txt";
    int numLines = 0;
    char** fileContents = getFileContents(filename, &numLines);
    Memory memory = createMemory(fileContents[0]);
    packMemory(&memory);
    printf("Part1: %" PRIu64 "\n", getChecksum(memory));

    memory = createMemory(fileContents[0]);
    printMemory(memory);
    packMemory2(&memory);
    printMemory(memory);
    printf("Part2: %" PRIu64 "\n", getChecksum(memory));

    return 0;
}
