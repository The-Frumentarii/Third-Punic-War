#include <stdio.h>
#include <string.h>

int cbtd(char *str);
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
        short to_memory = cbtd(&str[16]);
        memory[loopcount] = to_memory;
    }
    printf("You have reached the end of the memory.\n");
    memory[4095] = 0;
    return 0;
}


int cbtd(char *str){
    int p, i, j, k, n;
    p=0;
    n = (int)strlen(str);
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

