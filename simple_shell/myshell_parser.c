
#include "myshell_parser.h"
#include "stddef.h"
#include "regex.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

//First Try

//struct pipeline *pipeline_build(const char *command_line)
void  pipeline_build(const char* command_line)
{
  regex_t regex;
  int regexVal;

  struct pipeline_command *nextCommand;
  struct pipeline_command *tmp; // malloc(sizeof(struct pipeline_command));
  struct pipeline output;
  int k,r;
  char * it;
  
  // struct pipeline_command* commandInit = malloc(sizeof(struct pipeline_command));
  char symbolChar;
  
  size_t maxMatches = 10; //Trying some code I found in regex documentation
  size_t maxGroups = 4;
  unsigned int i,j,offset;
  
  regmatch_t groupArray[maxGroups];
  const char* regPattern = "[ \t\n]*([a-zA-Z0-9_.-]+)[ \t\n]*[|><&]?";
  char * cursor = command_line;

  // printf("%s\n", command_line);
  
  regexVal = regcomp(&regex, regPattern, REG_EXTENDED);
  if (regexVal) {
    fprintf(stderr, "Regular Expression compilation failed Compilation failed\n");
    exit(1);
  }
  
  for (i = 0; i <  maxMatches; i++) { 
    if ( regexec(&regex, cursor, maxGroups, groupArray, 0))
	break;

    offset = 0;
	for(j = 0; j <  maxGroups; j++) {
	  if (groupArray[j].rm_so == (size_t)-1)
	    break;

	  if (j == 0)
	    offset = groupArray[j].rm_eo;

	  char cursorCopy[strlen(cursor) + 1];
	  strcpy(cursorCopy, cursor);
	  cursorCopy[groupArray[j].rm_eo] = 0;
	  //printf("Match %u, Group %u: [%2u-%2u]: %s\n", i, j, groupArray[j].rm_so, groupArray[j].rm_eo, cursorCopy + groupArray[j].rm_so);
	  //printf("%s   %d     %d    %s\n", cursor, groupArray[j].rm_so, groupArray[j].rm_eo, cursorCopy);

	  char commandInput[strlen(cursorCopy + groupArray[j].rm_so)];
	  strcpy(commandInput, cursorCopy + groupArray[j].rm_so);
			    commandInput[strlen(commandInput)-1] = '\0';
	  if (j == 0) {
	    if ( i == 0 ) {
	      nextCommand = malloc(sizeof(struct pipeline_command));
	      nextCommand->command_args[0] = malloc(sizeof(commandInput));
	      strcpy(nextCommand->command_args[0], commandInput);
	      nextCommand->command_args[1] = NULL;
	      printf("%s\n", nextCommand->command_args[0]);
	      nextCommand->next = NULL;
	      output.commands = nextCommand;
	      tmp = nextCommand;


	    } else {
	    printf("%s\n", commandInput);  //cursorCopy + groupArray[j].rm_so);
	    switch (symbolChar) {
	    case '|': 
	      nextCommand = malloc(sizeof(struct pipeline_command));
	      strcpy(nextCommand->command_args[0], commandInput);
	      nextCommand->command_args[1] = NULL;
	      printf("%s\n", nextCommand->command_args[0]);
	      nextCommand->next = NULL;
	      output.commands = nextCommand;
	      tmp = nextCommand;
	    
	    printf("|\n");
	    break;
	  case ' ':
	    k = 0;
	    do {
	      it = next-command_args[k];
	      k++;
	      r = k-1;
	    } while (it != NULL)
	    nextCommand->command_args
	    printf("space\n");
	    break;
	  case '\t':
	    printf("space\n");
	    break;
	  case '\n':
	    printf("space\n");
	    break;
	  case '>':
	    printf(">\n");
	    break;
	  case '<':
	    printf("<\n");
	    break;
	  case '&':
	    printf("&\n");
	    break;
	  default:
	    printf("RegexFailed\n");
	    break;
	    }
	    }

	    
	  symbolChar = cursorCopy[groupArray[j].rm_eo - 1];
	  }
	}
	cursor += offset;
	//printf("%s\n", cursor - groupArray[j].rm_eo);
	//switch (cursor[groupArray[j].rm_eo]) {
	  //case '|':
	  //printf("|\n");
	  //break;
	  //case ' ':
	  //printf("space\n");
	  //break;
	  //case '\t':
	  //printf("space\n");
	  //break;
	  //case '\n':
	  //printf("space\n");
	  //break;
	  //case '>':
	  //printf(">\n");
	  //break;
	  //case '<':
	  //printf("<\n");
	  //break;
	  //case '&':
	  //printf("&\n");
	  //break;
	  //default:
	  // printf("RegexFailed\n");
	  // break;
	  //}

  }
  regfree(&regex);
  printf("%s", command_line);

  // TODO: Implement this function
  //	return NULL;
}

void pipeline_free(struct pipeline *pipeline)


{
	// TODO: Implement this function
}

int main() {
  const char* com = "ls|wc -l >counts.txt\n";
  // struct pipeline commandtest;
  pipeline_build(com);
  //printf("Hello\n");
  // printf("Testing\n");
  return 0;
}
