#ifndef DATA_H
#define DATA_H

#include <stdio.h>
//#include <string.h>
//#include <unistd.h>

typedef struct{
	char latin;
	char morse[10];
}Pair;


int get_number(int min, int max);
const char* latin_to_morse(const Pair* alphabet, int size, char character);
char morse_to_latin(const Pair* alphabet, int size, const char* token);
void put_string_to_morse(char* str, FILE* mfw, const Pair* alphabet, int alphabet_size);

#endif
