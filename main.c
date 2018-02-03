/*
 Assignment Part B -
	 Write a program to Display the memory contents in a tabular format
	 with ASCII conversion between two given memory addresses. Your
	 program should use page breaks and full error trapping on data entry,
	 order of addresses.
 
 Assignment Part C -
	Write a program to modify the memory contents in a tabular format.
	You may assume the data entry is hexadecimal, your program should
	use command breaks (Suggest '.' Or '#'character) and full error trapping
    on data entry, start addresses (Done forget illegal address ranges -
	only RAM) etc. 

 Assignment Part D -
	Write a program to execute a program stored in memory. You may assume
	the data entry is hexadecimal, full error trapping on data entry, start
	addresses (Done forget illegal address ranges - only RAM) etc.
	
Assignment Part E -
	Combine Part B, C, D  with a full error trapped menu program with
	additional  commands  'help', 'demo'  and 'load file -lf'as per the
	existing  68hc11 monitor. Your solution should be ported to EPROM and
	submitted to your tutor directly during the demo. 
 
 Created by Aaron Nebbs on 23/01/2018.
 Copyright © 2018 Aaron Nebbs. All rights reserved.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BOARD_MODE 0
#define INPUTSIZE 200
#define SCREEN_HEIGHT 31

void waitForInput(void);
int checkValidInput(char*);
int isValidHexChar(char);
int isValidHexString(int, char*);
int isNumber(char);
int isInRange(long, long, long);
char isValidAscii(char);
int modifyMemory(char[]);
void displayMemory(char[]);
void clearScreen(void);
void runProgram(void);
void testFunction(void);


int main() {
	clearScreen();
    waitForInput();
    return 0;
}

/* User input loop */
void waitForInput(){
    char func[INPUTSIZE];
    char data;
    int quit = 0;
    int ret;

    while (quit == 0) {
        printf("Command --- :> ");
        gets(func);
        ret = checkValidInput((char*)&func);
        
        /* Quits program */
        if(ret == -1){
            quit = 1;
        }
    }
}

/* Allows modification of memory based on user input */
int modifyMemory(char hex[5]){
	long *hexData; /* Used to store hex number from user */
	char *p; /* Pointer to memory location */
	char input[5]; /* User input for hex data */
	int counter = 0;

	/* Check if in valid RAM range */
	if(isInRange((long)hexData, 0x000, 0x3FF) == 0){
		printf("\n	 Invalid Memory Address! Try between 0x000 - 0x3FF\n\n");
		return 0;
	}
	
	sscanf(hex,"%lx",&hexData);
	p = (char*)hexData;
	
	printf("\n	 %s |   %s\n", "Address", "Hex		<.> to stop modifiying");
	printf("      ======================\n");
	
	/* Keep asking for user input, until <.> is entered */
	while(1){
		printf("	  %04x   |   ", hexData); /* Replace hexData with p[counter] */
		gets(input);
		
		/* Check if user wants to stop */
		if(input[0] == '.'){
			printf("\n");
			return 1;
		}
		
		/* Check if input is valid */
		if(isValidHexString(0, input) == 1 && strlen(input) < 3){
			/*Modify Memory and Increment Memory address*/
			counter++;
			/* Check if still in valid memory location*/
			if(isInRange((long)hexData, 0x000, 0x3FF) == 0){
				printf("\n	 Invalid Memory Address!\n");
				return 0;
			}
		}
	}
	return 0;
}

/* Displays memory based on the users input */
void displayMemory(char hex[5]){
	long *hexData; /* Used to store hex number from user */
	char *p; /* Pointer to memory location */
    int i;
    int counter = 0;
    
    /* Only run in on the microcontroller */
    if(BOARD_MODE){
    	/* Convert the user inputed hex into the hexData long */
	    sscanf(hex,"%lx",&hexData);
	    /* Set the pointer to look at the memory address the user input */
		p = (char*)hexData;
		
		printf("HEX: %s", hex);
		printf("\n	 %s %15s %26s\n", "Address", "Hex", "Ascii");
		printf("      ==============================================================\n");
		
		/* Print a set amount of memory addresses 20*8 = 160 addresses */
		for(i = 0; i < 20; i++){
			/* Checks if ascii is valid if not set the char to a .*/
			char c1 = isValidAscii(p[counter]); /* Replace n with p[counter] */
			char c2 = isValidAscii(p[counter]+1);
			char c3 = isValidAscii(p[counter]+2);
			char c4 = isValidAscii(p[counter]+3);
			char c5 = isValidAscii(p[counter]+4);
			char c6 = isValidAscii(p[counter]+5);
			char c7 = isValidAscii(p[counter]+6);
			char c8 = isValidAscii(p[counter]);
			/* Print out the memory address and first 4 hex values */
	        printf("	  %s      %s %s %s %s", "040a","FF","FF","FF","FF" );
	        /* Print out the last 4 memory addresses */
	        printf(" %s %s %s %s","FF","FF","FF","FF");
	        /* Print out the 8 ASCII characters */
	        printf("	 %c %c %c %c %c %c %c %c \n",c1,c2,c3,c4,c5,c6,c7,c8);
	        /* Increment the memory location */
	        counter += 8;
		}
	}else{
		printf("HEX: %s", hex);
		printf("\n	 %s %15s %26s\n", "Address", "Hex", "Ascii");
		printf("      ==============================================================\n");
		/* Print a set amount of memory addresses 20*8 = 160 addresses */
		for(i = 0; i < 20; i++){
			/* Checks if ascii is valid if not set the char to a .*/
			char c1 = isValidAscii('n'); /* Replace n with p[counter] */
			char c2 = isValidAscii('n');
			char c3 = isValidAscii('n');
			char c4 = isValidAscii('n');
			char c5 = isValidAscii('n');
			char c6 = isValidAscii('n');
			char c7 = isValidAscii('n');
			char c8 = isValidAscii('n');
			/* Print out the memory address and first 4 hex values */
	        printf("	  %s      %s %s %s %s", "040a","FF","FF","FF","FF" );
	        /* Print out the last 4 memory addresses */
	        printf(" %s %s %s %s","FF","FF","FF","FF");
	        /* Print out the 8 ASCII characters */
	        printf("	 %c %c %c %c %c %c %c %c \n",c1,c2,c3,c4,c5,c6,c7,c8);
	        /* Increment the memory location */
	        counter += 8;
		}
	}
	printf("\n");
}

/* Clears the screen by printing new lines */
void clearScreen(){
	int i = 0;
	for(i; i < SCREEN_HEIGHT; i++){
		printf("\n");
	}
}

/* Test function to be replaced with loaded program */
void testFunction(){
	printf("\n	 FROM POINTER FUNCTION\n\n");
}

/* Runs a function pointer */
void runProgram(){
	void (*ptr)();
	ptr = testFunction;
	ptr();
}

/* Checks if the users input is valid, if not ask for more input */
int checkValidInput(char *_func){
    char functionToTest[3];
    char hex[20];
    
    /* Takes the first 2 chars from the input and stores in string */
    if(sprintf(functionToTest,"%c%c", _func[0], _func[1])==0){
    	return 0;
	}
    
    /* Check if quit has been called */
    if(strcmp(functionToTest, "q") == 0){
        printf("\n");
        return -1;
    }
    /* Check if clear screen has been called */
	else if(strcmp(functionToTest, "cl") == 0){
    	clearScreen();
    	return 0;
    }
    /* Check if run program has been called */
    else if(strcmp(functionToTest, "go") == 0){
    	runProgram();
    }

    /* Check if the data entry follows the <FUNCTION> <DATA> format */
    if((_func[2] != ' ') || isNumber(_func[0]) || isNumber(_func[1])){
        return 0;
    }

    /* Check if a valid input */
    if (isValidHexString(3, _func) == 0 || _func[4] == ' ') {
        return 0;
    }
    
    /* Copy valid hex into hex string */
    strncpy(hex, _func + 3, 4);
    
    /* Do function based on input */
    if(strcmp(functionToTest, "dm") == 0){
        displayMemory(hex);
    }else if(strcmp(functionToTest, "mm") == 0){
    	modifyMemory(hex);
	}
    
    /* Invalid function, ask again */
    return 0; 
}

/* Check if the char is a valid ascii code */
char isValidAscii(char c){
	if(c > 32 && c < 126){
		return c;
	}else{
		return '.';
	}
}

/* Checks if given char is valid*/
int isValidHexChar(char c){
    /* Check if char is number */
    if(c > 47 && c < 58) return 1; 
	/* Check if char is abcdef/ABCDEF*/
	if(c > 64 && c < 71 || c > 96 && c < 103)return 1;
    return 0;
}

/* Checks if the whole string is a valid hex number */
int isValidHexString(int i, char *_func){
    for (i; i < strlen(_func); i++) {
        if(isValidHexChar(_func[i]) == 0){
            return 0;
        }
    }
    return 1;
}

/* Checks if the char given is a number */
int isNumber(char _checkChar){
    if(_checkChar >= 48  && _checkChar <= 57){
        return 1;
    }else{
        return 0;
    }
}

/* Checks if a number is in a range */
int isInRange(long in, long low, long high){
	if(in >= low && in <= high) return 1;
	return 0;
}




