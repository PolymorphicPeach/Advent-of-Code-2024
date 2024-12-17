#ifndef DAY9_UTILITY_H
#define DAY9_UTILITY_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char** getFileContents(const char *path, int *numLines);
void free2DArray(char **arr, int numLines);
void print2DArray(char **arr, int numLines);

#endif
