#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void printRules();
void help();

// Path to Host File
char path[] = "/etc/hosts";
int c;

int main(int argc, char *argv[])
{
  // Open File
  FILE * fp;
  fp = fopen(path, "a+");
  // Add 
  if (argc == 1){
    printf("Invalid Usage: firewall --protocol --srcadd --desadd --srcport --desport --action \n");
    printf("Use --help for extra information\n");
    return 0;
  } else {
    if(strncmp(argv[1],"--print", 7) == 0){
      printRules();
      return 0;
    } else if (strncmp(argv[1],"--help", 6) == 0){
      help();
      return 0;
    } else {
      for(int i = 1; i < argc;i++){
        fprintf(fp, "%s \n", argv[i]);
      }
    }
  }
  fseek(fp, 0, SEEK_SET);
  fclose(fp);
  return 0;
}

void printRules() {
  FILE * _fp;
  int _c;

  _fp = fopen(path, "r");
  while(1){
    _c = fgetc(_fp);
    if( feof(_fp) ) {
      break;
    }
    printf("%c", _c);
  }
  printf("");
  fclose(_fp);
}

void help(){
  printf("Help!\n");
}
