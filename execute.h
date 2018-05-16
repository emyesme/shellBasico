#ifndef __EXECUTE__
#define __EXECUTE__

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

int execute (char * comando, char * archivo);
void conPipes (char **arreglo[]);
void sinPipes (char **arreglo[]);
int countDiv(char *text, char *sep);
void divStr(char *array[],char *text, char *sep);
int validateEntry(char *wDiv[],int size);
void fillMatrix(char **array[], char *text, int *sizes, int  index);

#endif