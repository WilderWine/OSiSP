#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "consts.h"



Pair alphabet[] = {{'A', ".-"},{'B', "-..."},{'C', "-.-."},{'D', "-.."},{'E', "."},{'F', "..-."},
	{'G', "--."},{'H', "...."},{'I', ".."},{'J', ".---"},{'K', "-.-"},{'L', ".-.."},{'M', "--"},
	{'N', "-."},{'O', "---"},{'P', ".--."},{'Q', "--.-"},{'R', ".-."},{'S', "..."},{'T', "-"},
	{'U', "..-"},{'V', "...-"},{'W', ".--"},{'X', "-..-"},{'Y', "-.--"},{'Z', "--.."},

	{' ', "-...-"},{'.', "......"},{',', ".-.-.-"},{'!', "--..--"},{'?', "..--.."},
	{':', "---..."},{';', "-.-.-."},{'-', "-....-"},{'(', "-.--.-"},{')', "-.--.-"},
	{'`', ".----."},{'_', ".-..-."},{'@', ".--.-."},{'/', "--..-."},

	{'0', "-----"},{'1', ".----"},{'2', "..---"},{'3', "...--"},{'4', "....-"},
	{'5', "....."},{'6', "-...."},{'7', "--..."},{'8', "---.."},{'9', "----."},
	{'+', ".-.-"},{'*', "..-.."},{'\\', "--.--"}
};
int alphabet_size = sizeof(alphabet)/sizeof(alphabet[0]);
const char* latin_file = "latin.txt";
const char* morse_file = "morse.txt";



int main(){
while(1==1){
	printf("Choose what to do:\n1. Convert latin text to morse\n2. Convert morse to latin text.\n3. Exit\n");
	int option = get_number(1,3);
	if (option == 1){
		printf("1. Input from keyboard\n2. Input from file\n");
		int way = get_number(1,2);
		if (way==1){
			char input[256];
			printf("Input your text:\n");
			getchar();
			fgets(input, sizeof(input), stdin);
			
//			scanf("%99[^\n]", input);
			FILE* mfw = fopen(morse_file, "w");
			put_string_to_morse(input, mfw, alphabet, alphabet_size);
			fclose(mfw);
		} else if (way==2){
			FILE* lfr = fopen(latin_file, "r");
			if(lfr == NULL){
				printf("file does not exist");
				continue;
			}
			char buffer[256];
			FILE* mfw = fopen(morse_file, "w");
			while((fgets(buffer, 256, lfr))!=NULL){
				put_string_to_morse(buffer, mfw, alphabet, alphabet_size);
				fprintf(mfw, "\n");
			}
			fclose(mfw);
			fclose(lfr);
		}
	}if (option == 2) {
    		if (access(morse_file, F_OK) != -1) {
        		FILE* mfr = fopen(morse_file, "r");
        		FILE* lfw = fopen(latin_file, "w");
        		if (mfr == NULL || lfw == NULL) {
            			printf("Error while opening file(s)");
        		}
        		char buffer[256 * 7];
        		while (fgets(buffer, 256 * 7, mfr) != NULL) {
            			char* token = strtok(buffer, " ");
            			printf("%s", token);
            			while (token != NULL) {
                			char symbol = morse_to_latin(alphabet, alphabet_size, token);
                			fprintf(lfw, "%c", symbol);
                			token = strtok(NULL, " "); // Исправленное обновление значения token
            			}
 	           		fprintf(lfw, "\n");
        		}
        		fclose(mfr);
        		fclose(lfw);
    		}
	} else{break;}
}
}
