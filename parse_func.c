#include "parse_func.h"
#include <stdio.h>

void evaluate_input(char *line_input){

    char* words[MAX_CMD_INPUT];
    int number_of_words = 0;
    char absolue_path[1000];

    // "wc -l" -> "/usr/bin/wc"
    // "wc -l" -> "wc" "-l"
    break_into_words(line_input, words, &number_of_words, ' ');

    // accounts for number of arguments + NULL
    int number_of_arguments = number_of_words - 1;
    char *arguments[number_of_arguments];
    isolate_arguments_in(words, number_of_words, arguments);

    if(find_absolute_path(words[1], absolue_path) == false){
        printf("Could not find %s\n", words[1]);
        exit(0);
    }

    int fd_in;
    int fd_out;

    if(find_file_directories(&fd_in, 
                            &fd_out, 
                            words, 
                            number_of_words) == false){

        exit(0);

    }


    int pid = fork();
    if (pid == 0) {
        
        dup2(fd_in, STDIN_FILENO); 
        dup2(fd_out, STDOUT_FILENO);

        execve(absolue_path, arguments, NULL);
        printf("execve failed\n");
       
    }
    else if(pid != 0){

        int status;
        pid_t child_pid = waitpid(pid, &status, 0);

        if (child_pid == -1) {
            perror("waitpid");
            exit(1);
        }

        wait(NULL);
        close(fd_in);
        close(fd_out);

        printf("pid is %d. forked %d. "
           "Parent exiting\n",
             getpid(), child_pid);

    }
    else {
        perror("Fork failed");
        exit(1);
    }
    
}








void add_char_to_word(char* word, char c){
    int len = strlen(word);
    word[len] = c;
    word[len + 1] = '\0'; 
}

void break_into_words(char* input, char* words[], int *number_of_elems, char break_on){

    int word_count = 0;
    char* current_char = input;

    char word_so_far[1000];
    word_so_far[0] = '\0';

    while(*current_char != '\0'){
        if(*current_char == break_on){

            //malloc and strcpy in one line:
            words[word_count++] = strdup(word_so_far);
            strcpy(word_so_far, "");

        }
        else{

            add_char_to_word(word_so_far, *current_char);

        }
        current_char++;
    }

    words[word_count++] = strdup(word_so_far);
    words[word_count] = NULL;
    *number_of_elems = word_count;

}

bool find_absolute_path(char *no_path, char* with_path){

    char *directories[MAX_DIR_LENGTH];
    int number_of_directories = 0;

    break_into_words(getenv("PATH"), directories, &number_of_directories, ':');
    
    for(int ix = 0; ix < MAX_DIR_LENGTH && directories[ix] != NULL; ix++){
        strcpy(with_path, directories[ix]);
        strcat(with_path, "/");
        strcat(with_path, no_path);
        
        if(access(with_path, X_OK) == 0){
            return true;
        }
    }

    return false;

}

bool find_file_directories(int *input_source, int *output_source, char *words[], int word_count){

    if (word_count < 3) {
        fprintf(stderr,
            "Usage: <inputfile> <command> <outputfile>\n");
        return false;
    }

    if (*words[0] == '-'){
        
        *input_source = STDIN_FILENO;

    }
    else{

        *input_source = open(words[0], O_RDONLY);
        if (*input_source == -1) {
        fprintf(stderr, "Failed to open %s\n", words[0]);
        return false;
        }

    }
    if (*words[word_count - 1] == '-'){

        *output_source = STDOUT_FILENO;

    }
    else{
        
        *output_source = open(words[word_count - 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (*output_source == -1) {
            fprintf(stderr, "Failed to open %s\n", words[2]);
            return false;
        }

    }

    return true;

}

void isolate_arguments_in(char *words[], int number_of_words, char *arguments[]){

    for(int ix = 1; ix < number_of_words; ix++){

        // terminating index
        if(ix == number_of_words - 1){
            arguments[ix - 1] = NULL;
            break;
        }

        arguments[ix - 1] = words[ix];

    }

}


void print_char(char *char_arr[], int arr_len){
    
    for(int ix = 0; ix < arr_len; ix++){
        if(char_arr[ix] == NULL){
            break;
        }
        printf("%s\n", char_arr[ix]);
    }

}
