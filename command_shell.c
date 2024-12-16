#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include "parse_func.h"


#define MAX_LINE 1000

int main(int argc, char* argv[]){

    char* line[MAX_LINE];

    while(1){
        printf("xsh# ");
        fgets(*line, MAX_LINE, stdin);
        if(!strcmp(*line, "quit\n")){
            break;
        }
        printf("You entered %s", *line);

        // testing potential path to evaluate 
        evaluate_input(*line);
    }

    // ****************************************
    // CONNECTION OF SEGMENTS STILL IN PROGRESS
    // ****************************************


    return 0;

}

