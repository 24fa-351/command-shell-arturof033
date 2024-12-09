#include "redir_func.h"

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

    printf("we're are looking for %s \n", no_path);

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
    if (*words[2] == '-'){

        *output_source = STDOUT_FILENO;

    }
    else{
        
        *output_source = open(words[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (*output_source == -1) {
            fprintf(stderr, "Failed to open %s\n", words[2]);
            return false;
        }

    }

    return true;

}

void print_char(char *char_arr[], int arr_len){
    
    for(int ix = 0; ix < arr_len; ix++){
        if(char_arr[ix] == NULL){
            break;
        }
        printf("%s\n", char_arr[ix]);
    }

}
