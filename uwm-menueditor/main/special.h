#ifndef SPECIAL_H_
#define SPECIAL_H_
#include "stdio.h"

char *ReadQuoted(FILE *f); //Copied from UDE
void string_print (FILE* out,char *str);
char *MyOpen(char *name);
void *MyCalloc(size_t n,size_t s);

#endif
