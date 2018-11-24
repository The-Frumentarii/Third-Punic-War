/**************************************************************************
 * Assessment Title: ACE#3
 *
 * Number of Submitted C Files: 1
 *
 * Date: 25th of November 2018
 *
 * 
 * Authors:	
 *	1. Mateusz Dluzniewski, Reg no: 201727840;
 *	2. Rares Lungu, Reg no: 201749583
 *	3. Aoife Keane, Reg no: 201715226
 *	4. Sam Montgomery Allam, Reg no: 201708617
 *	5. Umer Ahmed, Reg no: 201707142
 *
 * 	 Statement: We confirm that this submission is all our own work.
 *
 *          	(Signed) Mateusz Dluzniewski
 *		(Signed) Rares Lungu
 *		(Signed) Aoife Keane
 *		(Signed) Umer Ahmed
 *		(Signed)
 *
 * 
 *
 **************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TRUE 1
#define FALSE 0

/*
	Structure that holds the values of the registers.
*/
typedef struct {
	short AC;		//Accumulator						16 bits
	short IR;		//Instruction Register 				16 bits
	short MBR; 		//Memory Buffer Register			16 bits
	short PC; 		//Program Counter					12 bits
	short MAR;		//Memory Address Register			12 bits
	short InREG;	//Input Register					16 bits
	short OutREG;	//Output Register					16 bits
	short FR;		//Flag Register						16 bits
}Registers;

//Input Functions
int load_default(short memory[4096]);
int read_console_input(short memory[4096]);
int read_file(short memory[4096]);

//Fetch-Decode-Execute
void fde(short memory[4096]);

//Functions for operations
void halt(Registers *reg);
void load(short memory[4096],Registers *reg);
void store(short memory[4096],Registers *reg);
void sub(short memory[4096], Registers *reg);
void add(short memory[4096], Registers *reg);
void input(Registers *reg);
void loadC(Registers *reg);
void output(Registers *reg);
void skipcond(Registers *reg);
void jump(Registers *reg);
void and(short memory[4096], Registers *reg);
void or(short memory[4096], Registers *reg);
void not(Registers *reg);
void xor(short memory[4096], Registers *reg);
void shiftright(Registers *reg);
void shiftleft(Registers *reg);

//Display memory functions
void display_memory_contents(short memory[4096]);
void display_assembly(short memory[4096],int no_instructions);

//Helper Functions
short cbtd(char input[], int comp);
char* cdtb(int p, char *binary, int bits);
short opcode(char *str);
short operand(char *str,int comp);
void display_registers(Registers *reg);


/*
	Simulates an disassembler.
*/
int main(int argc, char* argv[]){
	int no_instructions =0;
	short memory[4096] = {0};

	if(argc<2){
		printf("Missing argument(s).\n");
		printf("Program terminated.\n");
		return 1;
	}

	if(!(strcmp(argv[1],"-d")))
		no_instructions =load_default(memory);		//load default content to memory
	else{
		if(!(strcmp(argv[1],"-c")))
			no_instructions =read_console_input(memory);		//read user input from console into the memory
		else{
			if(!(strcmp(argv[1],"-f")))
				no_instructions =read_file(memory);		//read the contents of a file into memory
			else{
				printf("Invalid argument.\n");
				printf("Program terminated.\n");
				return 1;
			}
		}
	}
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\t\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\t\n");
	display_assembly(memory, no_instructions);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\t\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\t\n");
	fde(memory); 
	display_memory_contents(memory);
	
	return 0;

} 

//*******************************Get Input Functions*****************************
//*******************************************************************************

/*
	Loads some default content into memory.
*/
int load_default(short memory[4096]){
	memory[0] = 4096;		//Load	
	memory[1] = 8192;		//Store
	memory[2] = 12288;		//Sub
	memory[3] = 16384;		//Add
	memory[4] = 20480;		//Input
	memory[5] = 24576;		//Output
	memory[6] = 28672;		//SkipCond
	memory[7] = -32768;		//Jump
	memory[8] = -28672;		//LoadC
	memory[9] = -24576;		//And
	memory[10] = -20480;		//Or
	memory[11] = 0;			//Halt
	printf("Loaded default content.\n");
	return 11;
}

/*
	Loads the memory with instructions written by the user in the console.
*/
int read_console_input(short memory[4096]){
	int i;
	printf("Enter instructions in binary (2's complement). Write 'stop' to exit.\n");
	for(i=0;i<4096;i++){
    	char str[17];

		while(TRUE){												//This will keep on looping until a valid input is found;
		
			scanf("%s",str);										//Get the input;
										
            if (strcmp(str, "stop") == 0) {							//If the input was 'stop' then stop reading inputs;
                printf("stopped\n");	
                return i;
			}
			
			if (strlen(str) != 16) {								//Check if the input has the correct size;
				printf("Error: Invalid size.\n");
				continue;											//If not, continue with the while loop and try with a new input;
			}
			
			int valid = 1;											//If we get to this point => input != 'stop' & input has size 16;
			for(int index = 0; index<16; index++) {
                if (!(str[index] == '0' || str[index] == '1')) {	//Check if the characters in the input are valid;
                	printf("Error: Invalid character(s).\n");
                	valid = 0;										//If not, input is not valid;
                    break;
				}
			}
			if(valid)												//If the input was valid, break  out of the while loop;
				break;
  		}
  		
  		// Put the input in memory
  		short to_memory = cbtd(str,2);			
        memory[i] = to_memory;
        printf("Successfully wrote to memory.\n");
    }
    
    printf("You have reached the end of the memory.\n");
	return i;
}

/*
	Loads the memory with instructions from a file.
*/
int read_file(short memory[4096]){

	FILE *filePointer;
	char line[16];
	char file[255];
	int index = 0;

	printf("Please enter a full file name with the extension.\n");
	printf("(Up to 255 chars in length)\n");
	scanf(" %255s", file);
	filePointer = fopen(file, "r");

	if (filePointer == NULL){
		printf("File not found: %s \n", file);
		exit(EXIT_FAILURE);
	}
	printf("File read successful.\n");
	
	while (fgets(line, 18, (FILE*)filePointer)){
		if(index>4095){
			printf("Error: Input file was too large. Reading stopped.\n");
			break;
		}
		line[16] = '\0';
		memory[index] = cbtd(line,2);
		index++;
	}

	fclose(filePointer);
	return index;
}

//***********************FDE and Operation functions*************************
//***************************************************************************

/*
	Performs the fetch-decode-execute cycle and prints some useful messages when an error has occurred. 
*/
void fde(short memory[4096]){
	
	Registers reg = {0};
	char bin16[17];
	
	reg.FR = 1; 	//Program is running
	while(reg.FR==1){
		if(reg.PC>4095){
			reg.FR = 4;
			break;
		}
		reg.MAR = reg.PC;
		reg.IR = memory[reg.MAR];
		reg.PC++;
		cdtb(reg.IR, bin16, 16);
		reg.MAR = operand(bin16,0);  //MAR refers to an address 
		switch (opcode(bin16)){
			
			case 0:
			        halt(&reg);
			        break;
			case 1:
			      	load(memory,&reg);
			        break;
			case 2:
			        store(memory,&reg);
			        break;
			case 3:
			        sub(memory,&reg);
			        break;
			case 4:
			        add(memory,&reg);
			        break;
			case 5:
			        input(&reg);
					break;
			case 6:
			        output(&reg);
			        break;
			case 7:
			        skipcond(&reg);
			        break;
			case 8:
			        jump(&reg);
			        break;
			case 9:
			        loadC(&reg);
			        break;
			case 10:
			        and(memory, &reg);
			        break;
			case 11:
			       	or(memory, &reg);
			        break;
			case 12:
					not(&reg);
					break;
			case 13:
					xor(memory, &reg);
					break;
			case 14:
					shiftleft(&reg);
					break;
			case 15:
					shiftright(&reg);
					break;
		}
	}
	
	switch(reg.FR){
		case 0:
		        printf("Program halted.\n");
				printf("The value of the AC is %d.\n\n",reg.AC);
		        break;
		case 2:
		        printf("Error: Overflow.\n\n");
				display_registers(&reg);;
		        break;
		case 3:
				printf("Error: Underflow.\n\n");
				display_registers(&reg);
		        break;
		case 4:
		        printf("Error: Memory out of bounds.\n\n");
				display_registers(&reg);
		        break;
		default:
				printf("\n");
			break;
	}
	
}

/*
	Performs the halt operation.
*/
void halt(Registers *reg){
	reg->FR = 0;
}

/*
	Performs the load operation.
*/
void load(short memory[4096],Registers *reg) {
	reg->MBR = memory[reg->MAR];
	reg->AC = reg->MBR;
}

/*
	Performs the store operation.
*/
void store(short memory[4096],Registers *reg){
	reg->MBR = reg->AC;
	memory[reg->MAR] = reg->MBR;
}

/*
	Performs the subtract operation.
*/
void sub(short memory[4096], Registers *reg){
	reg->MBR = memory[reg->MAR];
	if(reg->AC - reg->MBR>32767)
		reg->FR = 2; 
	else if(reg->AC - reg->MBR<-32768)
		reg->FR = 3; 
	else
		reg->AC -= reg->MBR;
}

/*
	Performs the add operation.
*/
void add(short memory[4096], Registers *reg){
	reg->MBR = memory[reg->MAR];
	if(reg->AC + reg->MBR>32767)
		reg->FR = 2; 
	else if(reg->AC + reg->MBR<-32768)
		reg->FR = 3; 
	else
		reg->AC += reg->MBR;
}

/*
	Performs the input operation.
*/
void input(Registers *reg){
	int valid = FALSE;
	char inputVal[17];
	while(!valid){
		valid = TRUE;
		printf("Enter new AC in binary (16 bit): ");
		scanf(" %17s", inputVal);

		if (strlen(inputVal) != 16){
			valid = FALSE;
			printf("Error: Invalid size.\n");
			continue;
		}
		
		for(int i = 0; i<16; i++){
			if(!(inputVal[i] == '0' || inputVal[i] == '1')){
				valid = FALSE;
				printf("Error: Invalid character(s).\n");
				break;
			}
		}	
	}
	printf("Input read successfully.\n\n");
	reg->InREG = cbtd(inputVal, 0);
	reg->AC = reg->InREG;
}

/*
	Performs the Output operation.
*/
void output(Registers *reg){
    reg->OutREG = reg->AC;
    printf ("The value of register AC is: %04x \n", (unsigned short)reg->OutREG);
}

/*
	Performs the Skip on Condition operation.
*/
void skipcond(Registers *reg){
    if (reg->AC == 0) 
        reg->PC++;
}

/*
	Performs the Jump operation.
*/
void jump(Registers *reg){
    reg->PC = reg->MAR;
}

/*
	Performs the Load Constant operation.
*/
void loadC(Registers *reg){
	char bin16[17];
	cdtb(reg->IR, bin16, 16);
	reg->AC = operand(bin16,2);;
}

/*
	Performs the biwise AND operation.
*/
void and(short memory[4096], Registers *reg) {
	reg->MBR = memory[reg->MAR];
    reg->AC = reg->MBR & reg->AC;
}

/*
	Performs the bitwise OR operation.
*/
void or(short memory[4096], Registers *reg) {
    reg->MBR = memory[reg->MAR];
    reg->AC = reg->MBR | reg->AC;
}

/*
	Performs the bitwise NOT operation.
*/
void not(Registers *reg) {
    reg->AC = ~reg->AC;
}

/*
	Performs the bitwise XOR operation.
*/
void xor(short memory[4096], Registers *reg) {
    reg->MBR = memory[reg->MAR];
    reg->AC = reg->MBR ^ reg->AC;
}

/*
	Performs the Shift Right Logic operation.
*/
void shiftright(Registers *reg) {
    reg->AC = (reg->AC >> 1);
}

/*
	Performs the Shift Left Logic operation.
*/
void shiftleft(Registers *reg) {
    reg->AC = (reg->AC << 1);
}

//***********************Display Memory functions*************************
//************************************************************************

/*
	Prints out the contents of the memory in a file called "memoryContents.txt".
*/
void display_memory_contents(short memory[4096]){

	FILE *OutFile = fopen("memoryContents.txt", "w");

    	for(int i=0;i<4096;i++){

        fprintf(OutFile, "%d: \t", i);
        fprintf(OutFile, "%04x", (unsigned short)memory[i]);
        fprintf(OutFile, "\n");

    }
    fclose(OutFile);
}

/*
	Converts the contents of the memory into assembly and prints the assembly code on the screen.
*/
void display_assembly(short memory[], int no_instructions){	
	int address;
	short operating_on;
	char binary[17] = "0000000000000000\n";

	printf("ADDRESS |INSTRUCTION    |ON\n");
	printf("________+_______________+_____\n");
	address = 0;
	while(address<no_instructions && address<4096) {
		operating_on = operand(cdtb(memory[address], binary, 16),0);
        switch(opcode(cdtb(memory[address], binary, 16))){
			case 0:
				printf(" %d\t| HALT\t\t| X\n", address);
				operating_on = 0;
				break;
			case 1:
				printf(" %d\t| LOAD\t\t| %d\n", address, operating_on);
				break;
			case 2:
				printf(" %d\t| STORE\t\t| %d\n", address, operating_on);
				break;
			case 3:
				printf(" %d\t| SUB\t\t| %d\n", address, operating_on);
				break;
			case 4:
				printf(" %d\t| ADD\t\t| %d\n", address, operating_on);
				break;
			case 5:
				printf(" %d\t| INPUT\t\t| X\n", address);
				break;
			case 6:
				printf(" %d\t| OUTPT\t\t| X\n", address);
				break;
			case 7:
				printf(" %d\t| SKIP\t\t| X\n", address);
				break;
			case 8:
				printf(" %d\t| JUMPX\t\t| %d\n", address, operating_on);
				break;
			case 9:
				operating_on = operand(cdtb(memory[address], binary, 16),2);
				printf(" %d\t| LOADC\t\t| %d\n", address, operating_on);
				break;
			case 10:
				printf(" %d\t| AND\t\t| %d\n", address, operating_on);
				break;
			case 11:
				printf(" %d\t| OR\t\t| %d\n", address, operating_on);
				break;
			case 12:
				printf(" %d\t| NOT\t\t| %d\n", address, operating_on);
				break;
			case 13:
				printf(" %d\t| XOR\t\t| %d\n", address, operating_on);
				break;
			case 14:
				printf(" %d\t| SLL\t\t| %d\n", address, operating_on);
				break;
			case 15:
				printf(" %d\t| SRL\t\t| %d\n", address, operating_on);
				break;
			}
		address++;
	
    }
}

//***********************Helper functions*********************************
//************************************************************************

/*
	Converts a binary number (represented by the string input) to decimal and returns the value. The comp parameter is used to convert 		from either normal binary or two's complement.
*/
short cbtd(char *input, int comp){
	
	short result = 0;
	int power = 1;
	
	for(int i=2;i<strlen(input);i++)
			power = power * 2;
			
	switch(comp){
		case 0:
				power = power * 2;
				for(int i=0; i<strlen(input); i++){
					if(input[i] == '1')
						result = result + power;
					power = power/2;
				}
				break;
		
		case 2:
				if(input[0] == '1')
					result = -power*2;
				
				for(int i=1; i<strlen(input); i++){
					if(input[i] == '1')
						result = result + power;
					power = power/2;
				}
				break;
	}
	return result;
}

/*
	Converts a decimal number (p) to binary and returns a pointer to the binary representation (two's complement).
*/
char* cdtb(int p, char *binary, int bits){
	int i, n, k;
	n = bits;
	k = 1;
		
	if(p<0){
		for(i = 0; i<n-1; i++){
			k*=2;
		}
		p += k;
		binary[0] = '1';
	}
	else{
		binary[0] = '0';
		}
	for(i=1; i<n; i++){
		if(p%2==0){
			binary[n-i] = '0';
		}
		else{
			binary[n-i] = '1';
		}
	p/=2;
	}
	binary[n] = '\0';
	return binary;
	
}


/*
  	Extracts the opcode from an string representing an instruction.
*/
short opcode(char *str){
	
	char opcode[5];

	for(int i=0; i<4; i++){
		opcode[i] = str[i];
	}
	opcode[4] = '\0';
	
	return cbtd(opcode,0);
}

/*
	Extracts the operand from an string representing a binary number in either normal binary or two's complment.
*/
short operand(char *str,int comp){
	char operand[13];
	for(int i=4; i<16; i++){
		operand[i-4] = str[i];
	}
	operand[13] = '\0';
	
	return cbtd(operand,comp);
}

/*
	Prints the values of the registers on the screen.
*/
void display_registers(Registers *reg){

	printf ("The value of register AC is: %04x \n", (unsigned short)reg->AC);
	printf ("The value of register IR is: %04x \n", (unsigned short)reg->IR);
	printf ("The value of register MBR is: %04x \n", (unsigned short)reg->MBR);
	printf ("The value of register PC is: %04x \n", (unsigned short)reg->PC);
	printf ("The value of register InREG is: %04x \n", (unsigned short)reg->InREG);
	printf ("The value of register OutREG is: %04x \n", (unsigned short)reg->OutREG);
	printf ("The value of register FR is: %04x \n", (unsigned short)reg->FR);
}
