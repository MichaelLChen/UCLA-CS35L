//sfrob 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int frobcmp(char const* a, char const* b);
char decode(const char* c);
void checkAllocError(void* ptr);
int cmpWrapper(const void* a, const void* b);
int sfrob();

const int bias = 42;
const char spaceByte = ' ';
static int caseFlag = 0;

int main(int argc, char** argv){
  //argument check 
  if(argc > 2){
    fprintf(stderr, "Error in number of operands");
    exit(1);
  }
  if(argc == 2){
    char* flag = argv[1];
    if(flag[0]!='-' ||flag[1]!='f'){
      fprintf(stderr, "Invalid input");
      exit(1);
    }
    caseFlag = 1;
  }
  sfrob();
  return 0;
}


int sfrob(){
  //set up variables 
  char** buffer; 
  char* currentStr;
  char cur;
  size_t size_str, size_line, count_str, count_line;
  size_t count_file=0;
  int isEndOfFile;
  int (*cmp) (const void*, const void*);
  cmp = &cmpWrapper;

  //for file only
  ssize_t ret; 
  struct stat buf;
  ret = fstat(STDIN_FILENO, &buf);
  if(ret <0){
    fprintf(stderr, "file stats failed!");
    exit(1);
  }
  size_t size_file = buf. st_size + 1;
  //initialize
  size_str = size_file;
  size_line = size_file;
  count_str = 0;
  count_line = 0;
  
  ret = read(STDIN_FILENO, &cur, 1);
  //cur = getchar();
  if(ret < 0){
      fprintf(stderr, "Fail to read stdin");
      exit(1);
  }
  if(ret == 0)
    return 0;
  else 
    isEndOfFile=0;

  currentStr = (char*) malloc(size_str * sizeof(char));
  checkAllocError(currentStr);
  
  buffer = (char**) malloc(size_line * sizeof(char*));
  checkAllocError(buffer);
  
  while(!isEndOfFile){
    if(count_str >= size_str){
      //allocate new str buffer
      size_str = size_str * 2;
      currentStr = (char*)realloc(currentStr, size_str*sizeof(char));
      checkAllocError(currentStr);
    }
    currentStr[count_str] = cur;
    count_str++;

    //current line ends
    if(cur == spaceByte || cur == EOF){
      if(count_line >= size_line){
	//allocate new line 
	size_line = 2 * size_line;
	buffer = (char**)realloc(buffer, size_line*sizeof(char*));
      }
      buffer[count_line] = currentStr;
      size_str = size_file;
      count_str = 0; 
      count_line++;
      currentStr = malloc(size_str*sizeof(char));
      checkAllocError(currentStr);
    }

    isEndOfFile = (ret == 0);
    if(!isEndOfFile){ 
      ret = read(STDIN_FILENO, &cur, 1);
      if(ret < 0){
	fprintf(stderr, "Fail to read stdin");
	exit(1);
      }
      if(ret == 0)
	cur = EOF;
    }
  }

  //use frobcmp to qsort
  qsort(buffer, count_line, sizeof(char*), cmp);

  size_t i, j;
  //output 
  for (i=0; i< count_line; i++){
    char* outputLine = buffer[i];
    int j = 0;
    char c = outputLine[j];
    char space = spaceByte;
    while(c!=spaceByte && c!=EOF){
      ret = write(STDOUT_FILENO, &c, 1);
      if(ret<0){
	fprintf(stderr, "Fail to output");
	exit(1);
      }
      j++;
      c = outputLine[j];
    }
    if(j != 0)
      putchar(' ');
      if(ret<0){
	fprintf(stderr, "Fail to output");
	exit(1);
      }
  }
  //free all resources 
  free(buffer);
  free(currentStr);
  return 0;
}

void checkAllocError(void* ptr){
  if(ptr == 0){
      fprintf(stderr, "Fail to create memory buffer");
      exit(1);
  }
}

int cmpWrapper(const void* a, const void* b)
{
  return frobcmp(*((const char**) a), *((const char**) b));
}

int frobcmp(char const* a, char const* b){
  for(;*a==*b;a++, b++){
    if(*a == spaceByte){
      return 0;
    }
  }
  if(*a == spaceByte)
    return -1;
  if(*b == spaceByte)
    return 1;
  return (decode(a)<decode(b) ? -1:1);
}

char decode (const char* c){
  if(caseFlag == 0)
    return (*c)^bias;
  else{
    if((*c)>='a'&&(*c)<='z')
      return ((*c)-32)^bias;
    else
      return (*c)^bias;
  }
  /* if(caseFlag == 0) */
  /*   return (*c); */
  /* else{ */
  /*   if((*c)>='a'&&(*c)<='z') */
  /*     return ((*c)-32); */
  /*   else */
  /*     return (*c); */
  /* } */
}
