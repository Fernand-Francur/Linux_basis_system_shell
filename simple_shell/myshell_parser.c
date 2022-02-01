
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
  const char* pattern = "[ \t\n]*([[a-zA-Z0-9_.-]]+)[ \t\n]*[|><&]?";
  
  regexVal = regcomp(&regex, pattern, REG_EXTENDED);
  if (regexVal) {
    fprintf(stderr, "Regular Expression compilation failed Compilation failed\n");
    exit(1);
  }

  regexVal = regexec(&regex, command_line, 0, NULL, 0);

  
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
