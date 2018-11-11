
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

int16_t value_from_binary_string() {

    const char* input = "1111111100000000";

    int16_t value = 0;
    for (signed i = 0; i < 16; i++){

        value <<= 1;
        value |= (input[i] == '1');

    }

    printf("\n%d\n", value);

}

int main(){

value_from_binary_string();

return 0;
}
