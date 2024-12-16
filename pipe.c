#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define COMMAND_SEPARATOR "///"

#define READ_SIDE 0
#define WRITE_SIDE 1

// adds NULLS at the end
void copy_ptrs_from_to(char** to, char** from, int from_ix, int to_ix){

    for(int dest_ix = 0; dest_ix < to_ix - from_ix + 1; dest_ix++){

        to[dest_ix] = from[from_ix + dest_ix];

    }
    to[to_ix - from_ix + 1] = NULL;

}

int main(int argc, char* argv[]){

    if(argc < 4) {

        fprintf(stderr, 
            "Usage: %s <command1> [<arg1> <arg2> ...] /// <command2> [<arg1> <arg2> ...]\n",
            argv[0]);

        return 1;

    }

    int commands_separator_ix;

    for(commands_separator_ix = 0;
        commands_separator_ix < argc &&
        strcmp(argv[commands_separator_ix], COMMAND_SEPARATOR);
        commands_separator_ix++);

    if (commands_separator_ix == argc){
        fprintf(stderr,
                "Command separator '%s' not found\b", COMMAND_SEPARATOR);

        return 1;
    }

    // +1 for NULL
    char** first_cmd = (char**) malloc(sizeof(char*) * argc + 1);
    char** second_cmd = (char**) malloc(sizeof(char*) * argc + 1);

    copy_ptrs_from_to(first_cmd, argv, 1, commands_separator_ix - 1);
    copy_ptrs_from_to(second_cmd, argv, commands_separator_ix + 2, argc - 2);

    int pipe_from_read_to_write[2];
    pipe(pipe_from_read_to_write);

    int command1_pid = fork();

    if(command1_pid == 0){
        close(pipe_from_read_to_write[READ_SIDE]);
        dup2(pipe_from_read_to_write[WRITE_SIDE], STDOUT_FILENO);

        execve(first_cmd[0], first_cmd, NULL);

        fprintf(stderr, "Failed to execute first: %s]n", first_cmd[0]);

        _exit(1);
    }

    int command2_pid = fork();
    if(command2_pid == 0) {
        close(pipe_from_read_to_write[WRITE_SIDE]);
        dup2(pipe_from_read_to_write[READ_SIDE], STDOUT_FILENO);

        execve(second_cmd[0], second_cmd, NULL);

        fprintf(stderr, "Failed to execute second: %s]n", second_cmd[0]);

        _exit(1);
    }

    #ifdef WAIT_FOR_CHILDREN
        int wait_status;
        waitpid(command1_pid, &wait_status, 0);
        printf("command 1 exited with status %d\n", WEXITSTATUS(wait_status))
        waitpid(command2_pid, &wait_status, WNOHANG);
        printf("command 2 exited with status %d\n", WEXITSTATUS(wait_status))
    #endif

    printf("parent %s pid is %d. forked %d and %d. Parent exiting\n", 
           argv[0], getpid(), command1_pid, command2_pid);

    

    return 0;

}