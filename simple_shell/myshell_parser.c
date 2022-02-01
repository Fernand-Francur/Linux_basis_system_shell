
#include "myshell_parser.h"
#include "stddef.h"
#include "regex.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

//First Try

struct pipeline *pipeline_build(const char *command_line)
{
  regex_t regex;
  int regexVal;
  size_t maxMatches = 5; //Trying some code I found in regex documentation
  size_t maxGroups = 5;
  unsigned int i,j,offset;
  
  regmatch_t groupArray[maxGroups];
  const char* regPattern = "[ \t\n]*([[a-zA-Z0-9_.-]]+)[ \t\n]*[|><&]?";
  char * cursor = command_line;
  
  regexVal = regcomp(&regex, regPattern, REG_EXTENDED);
  if (regexVal) {
    fprintf(stderr, "Regular Expression compilation failed Compilation failed\n");
    exit(1);
  }
  
  for (i = 0; i <  maxMatches; i++) { 
    if ( regexec(&regex, command_line, maxGroups, groupArray, 0))
	break;
	
	for(j = 0; j <  maxGroups; j++) {
	  if (groupArray[j].rm_so == (size_t)-1)
	    break;

	  if (j == 0)
	    offset = groupArray[j].rm_eo;

	  //char cursorCopy


	}
    

  }
  
  printf("%s", command_line);

  // TODO: Implement this function
	return NULL;
}

void pipeline_free(struct pipeline *pipeline)


{
	// TODO: Implement this function
}

int main() {
  const char* com = "ls|wc -l >counts.txt\n";
  struct pipeline commandtest;
  commandtest = *pipeline_build(com);
  printf("Hello\n");
  printf("Testing\n");
  return 0;
}
