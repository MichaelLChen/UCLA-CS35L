//tr2b.c buffered

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int isByteinString(const char* str, char c);
void errorChecking(int argc, const char* argv[]);
int IO(char* from, char* to);

int main(int argc, const char* argv[]){  
  errorChecking(argc, argv);
  char* from = argv[1];
  char* to = argv[2];
  int result = IO(from, to);
  return result;
}

int IO(char* from, char* to){
  size_t i;
  size_t length_from = strlen(from);
  while(1){
    char c = getchar();
    if(ferror(stdin)){
      fprintf(stderr, "Error input string");
      exit(1);
    }
    if(feof(stdin))
      break;

    int printed = 0;
    for (i=0; i<length_from; i++){
      if(from[i]==c){
	putchar(to[i]);
	printed = 1;
      }
    }
    if(printed == 0)
      putchar(c);

    if(ferror(stdout)){
      fprintf(stderr, "Error output string");
      exit(1);
    }
  }
  return 0;
}
  
void errorChecking(int argc, const char* argv[]){
  char* from, * to;
  int i;
  if(argc != 3){
    fprintf(stderr, "Error in number of operands\n");
    exit(1);
  }
  from = argv[1];
  to = argv[2];
  int len_from = strlen(from);
  int len_to = strlen(to);
  if(len_from == 0){
    fprintf(stderr, "Missing operand\n");
    exit(1);
  }
  if(len_from != len_to){
    fprintf(stderr, "Different Length of input and output\n");
    exit(1);
  }
  
  for(i=0; i<len_from; i++){
    if(isByteinString(from, from[i])){
      fprintf(stderr, "Duplicate bytes in FROM string\n");
      exit(1);
    }
  }
}

int isByteinString(const char* str, char c){
  int count = 0;
  while(*str!='\0'){
    if(*str == c){
      count++;
      if(count > 1)
	return 1;
    }
    str++;
  }
  return 0;
}
