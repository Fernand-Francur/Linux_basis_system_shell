/*
Program to mimic linux shell interface
*/
#include "myshell_parser.h"
#include "stddef.h"

#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/fcntl.h>

#define MAX_LINE 512 /* The maximum length command */
#define MAX_CMDS 100 // The maximum pipelined commands
#define MAX_BACKGROUND_PIDS 100

// void add_pid(int pidlist[], int pid, int pid_num) {
//     if (pid_num < MAX_BACKGROUND_PIDS)
//         pidlist[pid_num] = pid;
//     else {
//         printf("Cannot create more that %d background processes.");
//         exit(1);
//     }
// }

// void remove_pid(int pidlist[], int pid, int pid_num) {
//     if(pid_num > 0) {
//         for (int i = 0, j = 0; i < pid_num; i++) {
//             if (pidlist[i] != pid) {
//                 pidlist[j++] = pidlist[i];
//             }
//         }
//     } else {
//         printf('There is no more registered background processes.');
//     }
// }


void sigchlHandler(int signal) {
    int status;
    pid_t child_Pid;

    while((child_Pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // printf("Background child %d terminated with status %d\n", 
        //                     child_Pid, status);

    }
}


int main(int argc, char *argv[]) {

    int num_cmds = 0;
    int num_pipes = 0;
    int pipe_idx = 0;
    char* fget_code;

    // int pidlist[MAX_BACKGROUND_PIDS];
    // int pid_num = 0;

    int should_run = 1; /* flag to determine when to exit program */
    char user_input[MAX_LINE];

    struct pipeline *cmdPipeline;
    struct pipeline_command *tmp;
    // struct pidlist * background_pid_list = NULL;
    struct sigaction sa;

    int f_in, f_out;
    pid_t pids[MAX_CMDS];
    int pid_status;

    // pipes
    int pipes[(MAX_CMDS - 1) * 2];

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchlHandler;		// address of handler function

    if(sigaction(SIGCHLD, &sa, NULL) == -1)
        fprintf(stderr, "ERROR: sigaction failure");


    while (should_run) {
       if (argc != 2) {
           printf("my_shell&");
       } else if (argc == 2) {
           if (strcmp(argv[1], "-n") != 0) {
               printf("my_shell&");
           }
       }
        fflush(stdout);

        memset(user_input, '\0', MAX_LINE);
        if ((fget_code = fgets(user_input, MAX_LINE, stdin)) != NULL) {
            // printf("fget_code = %s\n", fget_code);
            // printf("INITIAL INPUT: %s, size = %ld\n", user_input, strlen(user_input));
            // char stringInput[strlen(user_input)];
            // char stringInput[1024];
            // strcpy(stringInput, user_input); //, strlen(user_input));
            // stringInput[strlen(user_input)] = '\0';
            // printf("INPUT: %s\n", stringInput);
            cmdPipeline = pipeline_build(user_input);



            tmp = cmdPipeline->commands;
            num_cmds = 0;
            pipe_idx = 0;

            while (tmp != NULL) {
                // init pipe
                if (num_cmds > 0) {
                    if (pipe(pipes + (pipe_idx++ * 2)) == -1) {
                        return -1;
                    }
                }
                tmp = tmp->next;
                num_cmds++;
            }
            num_pipes = pipe_idx;

            pipe_idx = -1;
            for (int cmd_idx = 0; cmd_idx < num_cmds; cmd_idx++) {

                switch (cmd_idx) {
                    case 0:
                        tmp = cmdPipeline->commands;
                        break;
                    default:
                        tmp = tmp->next;
                }

                pipe_idx++; // set pipe pointer

                if (strcmp(tmp->command_args[0], "exit") == 0) /* exit command */
                    break;

                pid_t pid;
                pid = fork(); /* fork child process */

                pids[cmd_idx] = pid;

                switch (pid) {
                    case 0 :  /* child  */

                        // handle pipes
                        // INPUT
                        if (tmp->redirect_in_path != NULL) { // redirect input from file
                            f_in = open(tmp->redirect_in_path, O_RDONLY);
                            close(STDIN_FILENO);
                            dup(f_in);
                            close(f_in);
                        } else {
                            if (cmd_idx > 0) { // redirect input from previous command
                                dup2(pipes[(pipe_idx - 1) * 2], STDIN_FILENO);
                            }
                        }
                        // OUTPUT
                        if (tmp->redirect_out_path != NULL) { // redirect output to file
                            f_out = open(tmp->redirect_out_path, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IRGRP);
                            close(STDOUT_FILENO);
                            dup(f_out);
                            close(f_out);
                        } else {
                            if(tmp->next != NULL) { // redirect output to next command
                                dup2(pipes[pipe_idx * 2 + 1], STDOUT_FILENO);
                            }
                        }


//                    printf("close all pipe descriptors\n");
                        // close all pipe descriptors
                        for (int i = 0; i < num_pipes; i++) {
                            close(pipes[i * 2]); // close input pipe
                            close(pipes[i * 2 + 1]); // close output pipe
                        }
                    
                        if (execvp(tmp->command_args[0], tmp->command_args) < 0) {
                            printf("ERROR: Invalid command\n");
                            _exit(1);
                        }
                        
                        break;
                    case -1: // error
                        _exit(1);
                        break;
                    default: // parent
//                    printf("Parent cmd_idx = %d\n", cmd_idx);
                      break;
                }
            }


            // handle pipes
            // close all pipe descriptors
            for (int i = 0; i < num_pipes; i++) {
                close(pipes[i * 2]); // close input pipe
                close(pipes[i * 2 + 1]); // close output pipe
            }


            if (!cmdPipeline->is_background) {
                for (int i = 0; i < num_cmds; i++) {
                        // pid_t pid = 
                        waitpid(pids[i], &pid_status, 0);
                    // wait(NULL);
                        // printf("Child %d terminated with status %d\n", 
                        //     pid, pid_status);
                }
            } else {
                printf("[1] ");
                for (int i = 0; i < num_cmds; i++) {
                    printf("%d ", pids[i]);
                }
                printf("\n");
            }
            if (tmp != NULL) {
                if (strcmp(tmp->command_args[0], "exit") == 0) /* exit command */
                    break;
            }
            if (cmdPipeline->commands != NULL) {
                pipeline_free(cmdPipeline);
            }
        } else {
            if((fget_code == NULL) && feof(stdin)) {
                // printf("fget_code = %s\n", fget_code);
                // printf("INITIAL INPUT: %s, size = %ld\n", user_input, strlen(user_input));
                exit(0);
            }
        }
    }

    return 0;
}

//
// Created by Timo on 2/11/2022.
//#include "myshell_parser.h"
//#include "stddef.h"
//#include "regex.h"
//#include "stdio.h"
//#include "string.h"
//#include <stdlib.h>
//#include "myshell.h"



/*int main(int argc, char *argv[]) {
    struct pipeline *fullPipeline;
    char str[MAX_LINE];

    // function call prompt
    const char * nullPrompt = "-n";
//    if(argc == 2) {
//        if(strcmp(*argv[1], nullPrompt) != 0) {
//            printf("my_shell$");
//        }
//    }

    if( fgets(str,MAX_LINE, stdin) != NULL) {
        char stringInput[strlen(str)];
        strncpy(&stringInput[0], &str[0], strlen(str));
        fullPipeline = pipeline_build(stringInput);
    }
    pipeline_free(fullPipeline);
    return 0;*/