#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "consts.h"

int get_number(int min, int max){
	int answer;
	do{
		printf("input integer number from %d to %d:\t", min, max);
		if(scanf("%d", &answer) != 1){
			while (getchar() != '\n');
		} else if (answer < min || answer > max){
			printf("Number does not belong to interval needed");
		}
	}while(answer < min || answer > max);
	return answer;
}

const char* latin_to_morse(const Pair* alphabet, int size, char character){
	char upper = character;
	if (character == 0 || character == 10) return "";
	if(character >= 'a' && character <= 'z'){
		upper = character - 'a' + 'A';
	}
	for(int i = 0; i < size; i++){
		if(alphabet[i].latin==upper){
			return alphabet[i].morse;
		}
	}
	return "--------";
}

char morse_to_latin(const Pair* alphabet, int size, const char* token){
	if(strcmp(token, "--------")==0){return '$';}
        if(strcmp(token, "")==0){return ' ';}
	for (int i = 0; i < size; i++){
		if (strcmp(token, alphabet[i].morse) == 0) {
                   		return alphabet[i].latin;
               	}
	}
	return ' ';
}

void put_string_to_morse(char* str, FILE* mfw, const Pair* alphabet, int alphabet_size){
	char* token = strtok(str, " ");
	while (token!=NULL){
		int length = strlen(token);
		for (int i = 0; i < length; i++){
			char character = token[i];
			const char* code = latin_to_morse(alphabet, alphabet_size, character);
			fprintf(mfw, "%s ", code);
		}
		fprintf(mfw, "%s ", latin_to_morse(alphabet, alphabet_size, ' '));

		token = strtok(NULL, " ");
	}
}
