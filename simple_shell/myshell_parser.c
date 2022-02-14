
#include "myshell_parser.h"
#include "stddef.h"
#include <regex.h>
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

//First Try

struct pipeline *pipeline_build(const char *command_line)
//struct pipeline pipeline_build(const char* command_line)
{
    regex_t regex_type;
    int regexVal;

    struct pipeline_command *currCommand;
    struct pipeline_command *prevCommand = NULL; // malloc(sizeof(struct pipeline_command));
    
    struct pipeline_command *begin;
    
    struct pipeline *result = malloc(sizeof(struct pipeline));
    result->commands = NULL;
    result->is_background = false;
    char background_key[2] = {'&', '\0'};
    
    char EOL_key[2] = {'\n', '\0'};
    //int k;
    //char *it;

    // struct pipeline_command* commandInit = malloc(sizeof(struct pipeline_command));
    // char symbolChar = "\0";

    //size_t maxMatches = 32; //Trying some code I found in regex_type documentation
    size_t maxGroups = 10;
    unsigned int cmd_offset = 0;

    regmatch_t groupArray[maxGroups];
    const char *regPattern = "[ \t]*([a-zA-Z0-9_\\.\\/\\-]+)[ \t]*([|><\\&\n]?)";
    // ([||\\>|\\<|\\&|\n]?)
    // char *cursor = malloc(sizeof(command_line));
    // strcpy(cursor, command_line);

    // printf("%s\n", command_line);

    regexVal = regcomp(&regex_type, regPattern, REG_EXTENDED);
    if (regexVal) {
        fprintf(stderr, "Regular Expression compilation failed Compilation failed\n");
        exit(1);
    }

    char curr_cmd[200];
    memset(curr_cmd, '\0', 200);
    char last_symbol[2];
    char prev_symbol[2] = {'|', '\0'};
    unsigned int curr_cmd_offset = 0;
    unsigned cmd_size;
    // printf("before parsing\n");
    while ((regexec(&regex_type, &command_line[cmd_offset], maxGroups, groupArray, 0) == 0)) {
        while ((groupArray[2].rm_so == groupArray[2].rm_eo) && (cmd_offset < strlen(command_line)) ) {
            // keep building command
            cmd_offset += groupArray[1].rm_so;
            cmd_size = groupArray[1].rm_eo - groupArray[1].rm_so;
            strncpy(&curr_cmd[curr_cmd_offset], &command_line[cmd_offset], cmd_size);
            cmd_offset += cmd_size;
            curr_cmd_offset += cmd_size;
            curr_cmd[curr_cmd_offset++] = ' ';
            regexec(&regex_type, &command_line[cmd_offset], maxGroups, groupArray, 0);
        }
        if (cmd_offset < strlen(command_line)) {
            // last part of command
            cmd_offset += groupArray[1].rm_so;
            cmd_size = groupArray[1].rm_eo - groupArray[1].rm_so;
            strncpy(&curr_cmd[curr_cmd_offset], &command_line[cmd_offset], cmd_size);
            curr_cmd_offset += cmd_size;

            // last symbol
            cmd_offset += groupArray[2].rm_so - groupArray[1].rm_so;
            cmd_size = groupArray[2].rm_eo - groupArray[2].rm_so;
            strncpy(last_symbol, &command_line[cmd_offset], cmd_size);
            last_symbol[cmd_size] = '\0';
            cmd_offset += cmd_size;
            curr_cmd_offset += cmd_size;
        }

        // create a pipeline command

        switch (prev_symbol[0]) {
            case '|':
                currCommand = malloc(sizeof(struct pipeline_command));

                char * token;
               // char * trycmd[100];
                token = strtok(curr_cmd, " ");
                int i = 0;
                while(token != NULL) {
//                    trycmd[i] = token;
                    currCommand->command_args[i] = malloc(sizeof(token));
                    strcpy(currCommand->command_args[i], token);
                    token = strtok(NULL, " ");
                    i++;
                }
                currCommand->command_args[i] = NULL;
//         
                currCommand->next = NULL;
                currCommand->redirect_in_path = NULL;
                currCommand->redirect_out_path = NULL;
                if (prevCommand == NULL) {
                    result->commands = currCommand;
                } else {
                    prevCommand->next = currCommand;
                }
                prevCommand = currCommand;
                begin = result->commands;

                // Convert string to array of pointers


                break;
            case '<':
                if(begin != prevCommand) {
                    fprintf(stderr, "ERROR: Redirecting out of file not in first pipe");
                    break;
                }
                prevCommand->redirect_in_path = malloc(sizeof(curr_cmd));
                strcpy(prevCommand->redirect_in_path, curr_cmd);
                break;
            case '>':
                if(((strcmp(last_symbol, background_key) != 0) && (strcmp(last_symbol, EOL_key) != 0))) {
                    fprintf(stderr, "ERROR: Redirecting out of a pipeline command which is not last");
                    break;
                }
                prevCommand->redirect_out_path = malloc(sizeof(curr_cmd));
                strcpy(prevCommand->redirect_out_path, curr_cmd);
                break;
            case '&':
                fprintf(stderr, "ERROR: Background operator not at end of pipeline" );
                exit(1);
            case '\n':
                break;
            default:
                fprintf(stderr, "ERROR: Unknown command separator: %s", prev_symbol);
                 exit(1);
            break;
        }

        if (strcmp(last_symbol, background_key) == 0) {
            result->is_background = true;
        };

        memset(curr_cmd, '\0', 80);
        // printf("Current command: %s; with last symbol: %s; with prev_symbol: %s\n", curr_cmd, last_symbol, prev_symbol);
        curr_cmd_offset = 0;
        prev_symbol[0] = last_symbol[0];
        last_symbol[0] = '\0';
    };

    regfree(&regex_type);

    return result;

}

void command_free(struct pipeline_command * cmd) {

    if (cmd->next != NULL) {
        command_free(cmd->next);
        free(cmd->next);
    }
    if(cmd->redirect_in_path != NULL) free(cmd->redirect_in_path);
    if(cmd->redirect_out_path != NULL) free(cmd->redirect_out_path);
    int i = 0;
    while(cmd->command_args[i] != NULL) {
        free(cmd->command_args[i]);
        i++;
    }

}

void pipeline_free(struct pipeline *pipeline) {
    command_free(pipeline->commands);
    free(pipeline->commands);
    free(pipeline);
}
