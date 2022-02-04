
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
  struct pipeline output;
  
  size_t maxMatches = 10; //Trying some code I found in regex documentation
  size_t maxGroups = 4;
  unsigned int i,j,offset;
  
  regmatch_t groupArray[maxGroups];
  const char* regPattern = "[ \t\n]*([a-zA-Z0-9_.-]+)[ \t\n]*[|><&]?";
  char * cursor = command_line;

  printf("%s\n", command_line);
  
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
	  printf("Match %u, Group %u: [%2u-%2u]: %s\n", i, j, groupArray[j].rm_so, groupArray[j].rm_eo, cursorCopy + groupArray[j].rm_so);
	  //printf("%s   %d     %d    %s\n", cursor, groupArray[j].rm_so, groupArray[j].rm_eo, cursorCopy);


	  if (j == 0) {
	    printf("%s\n", cursorCopy + groupArray[j].rm_so);
	  switch (cursorCopy[groupArray[j].rm_eo - 1]) {
	  case '|':
	    printf("|\n");
	    break;
	  case ' ':
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
