//The start of something special
#include <stdio.h>
#include <string.h>

//Structure used to hold the values of the registers 
struct registers{
	short AC;		//Acumulator						16 bits
	short IR;		//Instruction Register 					16 bits
	short MBR; 		//Memory Buffer Register				16 bits
	short PC; 		//Program Counter					12 bits
	short MAR;		//Memory Address Register				12 bits
	short InREG;		//Input Register					16 bits
	short OutREG;		//Output Register					16 bits
	short FR;		//Flag Register						16 bits
};

//List of functions we need to implement: (Might need to make more helper functions; Return types and parameters not included)
void load_default(short memory[4096]);
void read_console_input(short memory[4096]);
void read_file(short memory[4096]);
short fde(short memory[4096]);       // fetch-decode-execute

//This part contains functions for the operations of our disassembler:
void load(short memory[4096], struct registers reg);
void store(short memory[4096], struct registers reg);
void sub(short memory[4096], struct registers reg);
void add(short memory[4096], struct registers reg);
void input(struct registers reg);
void output(struct registers reg);
void skipcond(struct registers reg);
void jump(struct registers reg);
void halt(struct registers reg);

//List of 3 more functions that we can add to our disassembler:
void loadc(struct registers reg);
void and(short memory[4096], struct registers reg);
void or(short memory[4096], struct registers reg);

//Helper functions:
char* cdtb(int p, char *binary, int bits);	//(Convert decimal to binary)   DONE
int cbtd(char *str);	//(Convert binary to decimal)   DONE
int opcodeM(char *str)	//gets the opcode DONE

void display_memory_content();	//????
void convert_to_assembly(shor memory); //DONE



int main(int argc, char* argv[]){

	short memory[4096];

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


	fde(memory); // fetch-decode-execute
	return 0;

}

void load_default(short memory[4096]){
	printf("Loaded some stuff.\n");
}
void read_console_input(short memory[4096]){
	printf("Read the console for input.\n");
}
void read_file(short memory[4096]){
	printf("Read stuff from a file.\n");
}
short fde(short memory[4096]){
	struct registers reg;
	char bin12[13];	//Binary Representation of 12 bit Register
	char bin16[17];	//Binary Representation of 16 bit Register
	
	reg.FR = 1; 	//Program is running
	reg.PC = 0;
	while(reg.FR==1){
		reg.MAR = reg.PC;
		reg.IR = memory[reg.MAR];
		reg.PC++;
		cdtb(reg.IR, bin16);
		printf("%s\n", bin16);
        //Copy bits 11-0 from IR to MAR
        printf(" %d\n", opcodeM(bin16));
		switch (opcodeM(bin16)){
			
			case 0:
			        halt(reg);
			        break;
			case 1:
			        load(memory,reg);
			        break;
			case 2:
			        store(memory,reg);
			        break;
			case 3:
			        sub(memory,reg);
			        break;
			case 4:
			        add(memory,reg);
			        break;
			case 5:
			        input(reg);
			        break;
			case 6:
			        output(reg);
			        break;
			case 7:
			        skipcond(reg);
			        break;
			case 8:
			        jump(reg);
			        break;
			case 9:
			        loadc(reg);
			        break;
			case 10:
			        and(reg,memory);
			        break;
			case 11:
			        or(reg,memory);
			        break;
			default:
			        printf("Invalid Opcode.\n");
			        break;
		}
	}
	
	return reg.AC;
}

//takes in a binary number as a string and returns the decimal value as an int
int cbtd(char *str){
	int p, i, j, k, n;
	n = strlen(str);
	int binary[n];
	for(i = 0; i<n; i++){
		binary[i] = str[i] - '0';
	}
	k = 1;
	if(binary[0] == 1){
		for(j = 0; j<n-1; j++){
			k*=2;
		}
		p -= k;

	}
	else{
		p =0;
	}
	for(i=1; i<n; i++){
		k=1;
		if(binary[i] ==1){

			for(j = 0; j<n-(1+i); j++){
				k*=2;
			}
		}
		else{
			k = 0;
		}
	p+=k;
	}

	return p;
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


//this gets the opcode from an instruction- decided to return it as an int bc it means we can use a switch on it later
int opcodeM(char *str){
	int i = 0;
	char opcode[5];
	for(i=0; i<4; i++){
		opcode[i] = str[i];
	}
	opcode[4] = '\0';
	
	int value = 0;
    int power = 8;
    for(int i=0;i<5; i++){
    	if(opcode[i] == '1')
    		value = value + power;
    	power = power/2;
    } 
    
	return value;
}

int operandM(char *str){
	int i = 0;
	char operand[13];
	for(i=0; i<13; i++){
		operand[i] = str[i+4];
	}
	operand[13] = '\0';
	
	
	return cbtd(operand);
}


//converts to assembly
void display_assembly(short memory[]){	
	int address;
	char *instruction = " ";
	int operating_on;
	char binary[17];
	int jump = 0;
	printf("ADDRESS |INSTRUCTION    |ON\n");
	printf("________+_______________+_____\n");
	for(address = 0; address+jump < 4096; address++) {
		operating_on = operandM(cdtb(memory[address+jump], binary, 16));
        	switch(opcodeM(cdtb(memory[address+jump], binary, 16))){
			case 11:
				instruction = "OR";
				break;
			case 10:
				instruction = "AND";
				break;
			case 9:
				instruction = "LOADC";
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
