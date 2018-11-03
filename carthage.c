//The start of something special
#include <stdio.h>
#include <string.h>

//List of functions we need to implement: (Might need to make more helper functions; Return types and parameters not included) 
void load_default();
void read_console_input();
void read_file();
void fde();       // fetch-decode-execute  

//This part contains functions for the operations of our disassembler:
void load();
void store();
void subt();
void add();
void input();
void output();
void skipcond();
void jump();
void halt();

//List of 3 more functions that we can add to our disassembler:
void loadc();
void and();
void or();

//Helper functions:
char* cdtb(int p, char *binary);	//(Convert decimal to binary)   DONE 
int cbtd(char *str);	//(Convert binary to decimal)   DONE 
int opcodeM(char *str)	//gets the opcode DONE 
	
void display_memory_content();	//????
void convert_to_assembly();



int main(int argc, char* argv[]){ 

	if(argc<2){
		printf("Missing argument(s).\n");
		printf("Program terminated.\n");
		return 1;
	}
	
	if(!(strcmp(argv[1],"-d")))		
		load_default();		//load default content to memory 	
	else{
		if(!(strcmp(argv[1],"-c")))
			read_console_input();		//read user input from console into the memory 
		else{
			if(!(strcmp(argv[1],"-f")))
				read_file();		//read the contents of a file into memory 
			else{
				printf("Invalid argument.\n");
				printf("Program terminated.\n");
				return 1;
			}
		}
	}
			

	fde(); // fetch-decode-execute  	
	return 0;
	
}

void load_default(){
	printf("Loaded some stuff.\n");
}
void read_console_input(){
	printf("Read the console for input.\n");
}
void read_file(){
	printf("Read stuff from a file.\n");
}
void fde(){
	printf("Executed the stuff in memory.\n");
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
		for(j = 0; j<n; j++){
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
char* cdtb(int p, char *binary){
	int i;
	
	if(p<0){
		p += 2048;
		binary[0] = '1';
		
	}
	else{
		binary[0] = '0';
	}
	for(i=1; i<12; i++){
		if(p%2==0){
			binary[12-i] = '0';
		}
		else{
			binary[12-i] = '1';
		}
	p/=2;
	}
	binary[12] = '\0';
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
	
	
	return cbtd(opcode);
}
