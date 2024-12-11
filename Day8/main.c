#include <stdio.h>
#include "Utility.h"

int main(void) {
    const char* filename = "../test.txt";
    int rows = 0;
    char** fileContents = getFileContents(filename, &rows);
    print2DArray(fileContents, rows);

    return 0;
}
