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
void cdtb();	//(Convert decimal to binary)   DONE 
void cbtd();	//(Convert binary to decimal)   DONE 

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
