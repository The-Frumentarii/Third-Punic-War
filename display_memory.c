#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/*

---------NOTES---------

Here I used Sam's code (added/changed about 2 lines and added
the display memory function, and changed the cbtd function to
one I found easier.

My function kept returning 0, spent ages trying to fix it.
With the last convert bin 2 dec function, 0 kept being returned,
this is the same with the new one and I think it is because of
the way the variable is being passed to it.

I still need to work on printing the memory address in hex

Anything else in this which is crap, let me know and I'll try fix it

There are a couple more comments below

*/

int16_t convert_bin2dec(char *str);
void display_memory_contents(int loopcount);
short memory[4096];

int main(){
    int loopcount;
    printf("Be aware there is a halt at the end of memory");
    for(loopcount = 0; loopcount < 4096; loopcount++) {
        char str[16];
        char firstfour[4];
        int bool = 0;
        while (bool == 0) {
            printf("Please enter a 16 bit binary number or 'stop'. You will see this message again if you don't\n");
            scanf( "%s" , str );
            strncpy(firstfour, str, 4);
            if (strcmp(firstfour, "stop") == 0) {
                printf("stopped\n");
                return 1;
            }
            if (strlen(&str[16]) != 16) {
                continue;
            }
            int valid = 1;
            for(int index = 0; index<16; index++) {
                if (!(str[index] == '0' || str[index] == '1')) {
                    valid = 0;
                    break;
                }
            }
            if(valid)
                break;
        }
        printf("accepted\n");
        int16_t to_memory = convert_bin2dec(&str[16]); // changed this line to call the new function
        memory[loopcount] = to_memory;
        display_memory_contents(loopcount); // I am calling my function every iteration of the for loop
    }
    printf("You have reached the end of the memory.\n");
    memory[4095] = 0;
    return 0;
}

void display_memory_contents(int loopcount){

    printf("Memory contents at position: %d ", loopcount); // prints a heading

      printf("%04" PRIx16, memory[loopcount]); // prints content in hex

      printf(":%p\n", &memory[loopcount]); //prints address not in hex


}

int16_t convert_bin2dec(char *str){

    char* input = str;

    int16_t value = 0; //int16 is a signed int

    for (signed i = 0; i < 16; i++){

        value <<= 1;        //<<= this shifts the bits to the left
        value |= (input[i] == '1'); //constructs a bitwise or on the two variables, then stores the result in valuee

    }

    printf("\n%d\n", value);

    return value;

    }
