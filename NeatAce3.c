#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define TRUE 1
#define FALSE 0

typedef struct {
	short AC;		//Acumulator						16 bits
	short IR;		//Instruction Register 					16 bits
	short MBR; 		//Memory Buffer Register				16 bits
	short PC; 		//Program Counter					12 bits
	short MAR;		//Memory Address Register				12 bits
	short InREG;		//Input Register					16 bits
	short OutREG;		//Output Register					16 bits
	short FR;		//Flag Register						16 bits
}Registers;

//Input Functions
void load_default(short memory[4096]);
void read_console_input(short memory[4096]);
void read_file(short memory[4096]);

//Fetch-Decode-Execute
short fde(short memory[4096]);

//Functions for operations
void halt(Registers *reg);
void load(short memory[4096],Registers *reg);
void store(short memory[4096],Registers *reg);
void sub(short memory[4096], Registers *reg);
void add(short memory[4096], Registers *reg);
void input(Registers *reg);

void output(Registers *reg);
void skipcond(Registers *reg);
void jump(Registers *reg);
void and(short memory[4096], Registers *reg);
void or(short memory[4096], Registers *reg);

//Display memory functions
void display_memory_contents(short memory[4096]);
void display_assembly(short memory[4096]);

//Helper Functions
short cbtd(char input[], int comp);
char* cdtb(int p, char *binary, int bits);
short opcode(char *str);
short operand(char *str);

/*


void loadc(struct registers reg);
*/

int main(int argc, char* argv[]){

	short memory[4096] = {0};

	if(argc<2){
		printf("Missing argument(s).\n");
		printf("Program terminated.\n");
		return 1;
	}

	if(!(strcmp(argv[1],"-d")))
		load_default(memory);		//load default content to memory
	else{
		if(!(strcmp(argv[1],"-c")))
			read_console_input(memory);		//read user input from console into the memory
		else{
			if(!(strcmp(argv[1],"-f")))
				read_file(memory);		//read the contents of a file into memory
			else{
				printf("Invalid argument.\n");
				printf("Program terminated.\n");
				return 1;
			}
		}
	}
	
	


	//display_assembly(memory);
	fde(memory); // fetch-decode-execute
	//display_memory_contents(memory);
	
	return 0;

} 

//*******************************Get Input Functions*****************************
//*******************************************************************************

void load_default(short memory[4096]){
	printf("Print default content\n");
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
}

void read_console_input(short memory[4096]){

	for(int i=0;i<4096;i++){
    	char str[17];

		while(TRUE){												//This will keep on looping until a valid input is found;
		
			scanf("%s",str);										//Get the input;
										
            if (strcmp(str, "stop") == 0) {							//If the input was 'stop' then stop reading inputs;
                printf("stopped\n");	
                return;
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
        printf("Succesfully wrote to memory.\n");
    }
    
    printf("You have reached the end of the memory.\n");
	
}


void read_file(short memory[4096]){

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
			line[16] = '\0';
			memory[index] = cbtd(line,2);
			index++;
	}

	fclose(filePointer);

}

//***********************FDE and Operation functions*************************
//***************************************************************************

short fde(short memory[4096]){
	
	Registers reg = {0};
	char bin16[17];
	reg.AC = 0;
	
	reg.FR = 1; 	//Program is running
	reg.PC = 0;
	while(reg.FR==1){
		reg.MAR = reg.PC;
		reg.IR = memory[reg.MAR];
		reg.PC++;
		cdtb(reg.IR, bin16, 16);
		reg.MAR = operand(bin16);  //MAR refers to an address 
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

			        printf("Input.\n");
			        break;
			case 6:
			        output(&reg);
			        break;
			case 7:
			        skipcond(&reg);
			        break;
			case 8:
			        jump(&reg);
			        printf("Jump.\n");
			        break;
			case 9:
			        //loadc(reg);
			        printf("LoadC.\n");
			        break;
			case 10:
			        and(memory, &reg);
			        break;
			case 11:
			       	or(memory, &reg);
			        break;
			default:
			        printf("Invalid Opcode.\n");
			        break;
		}
	}
	
	return reg.AC;
}

void halt(Registers *reg){
	reg->FR = 0;
}

void load(short memory[4096],Registers *reg) {
	reg->AC = memory[reg->MAR];
}

void store(short memory[4096],Registers *reg){
	memory[reg->MAR] = reg->AC;
}

void sub(short memory[4096], Registers *reg){
	if(reg->AC - memory[reg->MAR]>32767 || reg->AC - memory[reg->MAR]<-32768)
		reg->FR = 2; 
	else
		reg->AC -= memory[reg->MAR];
}

void add(short memory[4096], Registers *reg){
	if(reg->AC + memory[reg->MAR]>32767 || reg->AC + memory[reg->MAR]<-32768)
		reg->FR = 2; 
	else
		reg->AC += memory[reg->MAR];
}

void output(Registers *reg){
    reg->OutREG = reg->AC;
    printf ("The value of register AC is: %04x \n", (unsigned short)reg->OutREG);
}

void skipcond(Registers *reg){ //!!!!! Might cause Out of bounds error
    if (reg->AC == 0) 
        reg->PC++;
}

void jump(Registers *reg){
    reg->PC = reg->MAR;
}

void and(short memory[4096], Registers *reg) {
    reg->AC = memory[reg->MAR] & reg->AC;
}

void or(short memory[4096], Registers *reg) {
    reg->AC = memory[reg->MAR] | reg->AC;
}

void input(Registers *reg){
	int valid = FALSE;
	char inputVal[17];
	while(!valid){
		valid = TRUE;
		printf("Enter new AC in binary (16 bit): ");
		scanf(" %17s", inputVal);

		if (strlen(inputVal) != 16){
			valid = FALSE;
			continue;
		}
		
		for(int i = 0; i<16; i++){
			if(!(inputVal[i] == '0' || inputVal[i] == '1')){
				valid = FALSE;
				break;
			}
		}	
	}
	reg->AC = cbtd(inputVal, 0);
}
//***********************Display Memory functions*************************
//************************************************************************

void display_memory_contents(short memory[4096]){

	FILE *OutFile = fopen("C:\\Location\\memoryContents.txt", "w");

    	for(int i=0;i<4096;i++){

        fprintf(OutFile, "%d: \t", i);
        fprintf(OutFile, "%04x", (unsigned short)memory[i]);
        fprintf(OutFile, "\n");

    }
    fclose(OutFile);
}

void display_assembly(short memory[]){	
	int address;
	char *instruction = " ";
	int operating_on;
	char binary[17];
	int jump = 0;
	printf("ADDRESS |INSTRUCTION    |ON\n");
	printf("________+_______________+_____\n");
	for(address = 0; address+jump < 4096; address++) {
		operating_on = operand(cdtb(memory[address+jump], binary, 16));
        	switch(opcode(cdtb(memory[address+jump], binary, 16))){
			case 11:
				instruction = "OR";
				break;
			case 10:
				instruction = "AND";
				break;
			case 9:
				instruction = "LOADC";
				operating_on = operand(cdtb(memory[address+jump], binary, 16));
				break;
			case 8:
				instruction = "JUMPX";
				break;
			case 7:
				instruction = "SKIP";
				operating_on = 0;
				break;
			case 6:
				instruction = "OUTPT";
				operating_on = 0;
				break;
			case 5:
				instruction = "INPUT";
				operating_on = 0;
				break;
			case 4:
				instruction = "ADD";
				break;
			case 3:
				instruction = "SUB";
				break;
			case 2:
				instruction = "STORE";
				break;
			case 1:
				instruction = "LOAD";
				break;
			case 0:
				instruction = "HALT";
				operating_on = 0;
				break;
			default:
				instruction = "NULL";
				operating_on = 0;
				break;
				
			}
		printf(" %d\t| %s\t\t| %d\n", address+jump, instruction, operating_on);

		if (strcmp(instruction, "HALT") == 0) {
			break;
		}
		
		else if(strcmp(instruction, "JUMPX") == 0){
			
			jump = operating_on;
			continue;
		}
		else{
			continue;
		}
	
    }
}

//***********************Helper functions*********************************
//************************************************************************
short cbtd(char input[], int comp){
	
	
	short result = 0;
	int power = 1;
	
	for(int i=2;i<strlen(input);i++)
			power = power * 2;
			
	switch(comp){
		case 0:{
			power = power * 2;
			for(int i=0; i<strlen(input); i++){
				if(input[i] == '1')
					result = result + power;
				power = power/2;
			}
			break;
		}
		
		case 2:{
			if(input[0] == '0'){
				result = 0;
			for(int i=1; i<strlen(input); i++){
				if(input[i] == '1')
					result = result + power;
				power = power/2;
			}
			}
			else{
				result = -power*2;
				for(int i=1; i<strlen(input); i++){
					if(input[i] == '1')
						result = result + power;
					power = power/2;
				}
			}
			break;
		}
	}

	return result;
}

//takes in a decimal number and the address of a string to return the binary value as a string
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
//this gets the opcode from an instruction- decided to return it as an int bc it means we can use a switch on it later
*/

short opcode(char *str){
	
	char opcode[5];

	for(int i=0; i<4; i++){
		opcode[i] = str[i];
	}
	opcode[4] = '\0';
	
	return cbtd(opcode,0);
}

short operand(char *str){
	char operand[13];

	for(int i=4; i<16; i++){
		operand[i-4] = str[i];
	}
	operand[16] = '\0';
	
	return cbtd(operand,0);
}
