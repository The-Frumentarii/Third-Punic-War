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
    return(reg.MAR & reg.AC);
}

void or(short memory[4096], struct registers reg) {
    return(reg.MAR | reg.AC);
}

void output(struct registers reg) {
    reg.OutReg = reg.AC;
    printf ("The value of register AC is: %X \n", reg.OutReg);
}
