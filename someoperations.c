#include <stdio.h>

void halt(struct registers reg) {
    reg.FR = 0;
}

void skipcond(struct registers reg) {
    if (reg.AC == 0) {
        reg.PC++
    }
}

void and(short memory[4096], struct registers reg) {
    reg.AC = reg.MAR & reg.AC;
}

void or(short memory[4096], struct registers reg) {
    reg.AC = reg.MAR | reg.AC;
}

void output(struct registers reg) {
    reg.OutReg = reg.AC;
    printf ("The value of register AC is: %X \n", reg.OutReg);
}

void add(short memory[4096], struct registers reg){
	if(reg.AC + memory[reg.MAR]>32767 || reg.AC + memory[reg.MAR]<-32768)
		reg.FR = 2; 
	else{
		reg.AC += memory[reg.MAR];
	}
		
}

void sub(short memory[4096], struct registers reg){
	if(reg.AC - memory[reg.MAR]>32767 || reg.AC - memory[reg.MAR]<-32768)
		reg.FR = 2; 
	else{
		reg.AC -= memory[reg.MAR];
	}
		
}

void load(short memory[4096], struct registers reg) {
    reg.AC = memory[reg.MAR];
}

void store(short memory[4096], struct registers reg) {
    memory[reg.MAR] = reg.AC;
}

void jump(struct registers reg) {
    reg.PC = reg.MAR;
}


void shiftright(struct registers reg) {
    reg.AC = (reg.MAR / 2);
}

void shiftleft(struct registers reg) {
    reg.AC = (reg.MAR * 2);
}

