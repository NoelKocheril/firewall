#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FALSE 0
#define TRUE  1

// Use Proper Clear Screen Function
#if defined WIN32
#define CLEAR_SCREEN system("cls");
#else
#define CLEAR_SCREEN system("clear");
#endif

// Implicit Declaration of Functions
void usage();
void printRules();
void help();
void pause();
int  validIP(char* ip);
int isValueInArray(char* val, char* arr[], int size);
void upper(char* arr);

// Constants for Comparing Entries in Command Line to Valid Options
char* PROTO_OPTIONS[] = {"ALL", "TCP", "UDP", "ICMP"};
int PROTO_SIZE = 4;

char* ACTION_OPTIONS[] = {"BLOCK", "UNBLOCK"};
int ACTION_SIZE = 2;

// Path to Host File
char PATH[] = "/etc/hosts";

int main(int argc, char *argv[])
{
  CLEAR_SCREEN
  // Command Line Vaildation
  // No Possible uses with argc == 1
  if (argc == 1){
  // Provide Usage for Functions
    usage();
    return 0;
  /*
     Two Possible Command Line Inputs with argc == 2
     --print: Prints all Rules using printRules()
     --help: Provides interactive Helper Function to detail
             possible command line inputs
  */
  } else if(argc == 2){
  if(strcmp(argv[1],"--print") == 0){
      printRules(); // Print All Rules in use
      return 0;
    } else if (strcmp(argv[1],"--help") == 0){
      help();       // Run Help Function to provide information on Options
      return 0;
    } else {
      usage();
      return 0;
    }

    /*
       One Possible Command Line Input with argc == 3
       --delete num: Deletes the num-th rule
       i.e.
       Rules (Before):
         firewall --in --proto ALL --action BLOCK
         firewall --in --srcip 127.0.0.1 --proto TCP --action UNBLOCK

       $ firewall --delete 1

       Rules (After):
         firewall --in --srcip 127.0.0.1 --proto TCP --action UNBLOCK
    */

  } else if (argc == 3){
    if (strcmp(argv[1],"--delete") == 0){
      int num;
      char ans;
      num = atoi(argv[2]);
      // TODO - Read Rule from rule file
      printf("Are you sure you want to delete Rule %d? (Y or N)\n", num);
      scanf(" %c", &ans);
      if(toupper(ans) == 'Y'){
        printf("Rule %d deleted.\n\n", num);
      } else {
        printf("Rule %d was NOT deleted.\n\n", num);
      }
    } else {
      usage();
      return 0;
    }
    /*
       All Other Combinations of Command Line Inputs
    */
  } else if(argc >= 6){
    // Blocks All incoming Traffic
    // Most Minimal Command: firewall --in    --proto ALL     --action BLOCK
    //                       1        2       3       4       5        6
    //                       argv[0]  argv[1] argv[2] argv[3] argv[4]  argv[5]
    if(((strcmp(argv[1],"--in") == 0) || (strcmp(argv[1],"--out") == 0))){
      int i = 2;
      // Loop through
      while(i < argc){
        if(strcmp(argv[i],"--proto") == 0){
          if(i+1 < argc){
            upper(argv[i+1]); // Ensures Option is Capitalized
            if(isValueInArray(argv[i+1], PROTO_OPTIONS, PROTO_SIZE)){
              printf("%s %s\n", argv[i], argv[i+1]);
            } else {
              printf("Invalid --proto\n");
            }
            i++;
          } else {
            usage();
          }
        } else if(strcmp(argv[i],"--action") == 0){
          if(i+1 < argc){
            upper(argv[i+1]); // Ensures Option is Capitalized
            if(isValueInArray(argv[i+1], ACTION_OPTIONS, ACTION_SIZE)){
              printf("%s %s\n", argv[i], argv[i+1]);
            } else {
              printf("Invalid --action\n");
            }
            i++;
          } else {
            usage();
          }
        }
      i++;
      }
    } else {
      usage();
    }
  } else {
    usage();
  }

  return 0;
}

void usage(){
  printf("\nUsage: firewall --in --proto ALL --action BLOCK \n");
  printf("Use --help for extra information\n\n");
  pause();
}

void printRules() {
  FILE * _fp;
  int _c;

  _fp = fopen(PATH, "r");
  while(1){
    _c = fgetc(_fp);
    if( feof(_fp) ) {
      break;
    }
    printf("%c", _c);
  }
  printf("\n");
  fclose(_fp);
}

//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Utility Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~

void help(){
  int entry;
  // Print Possible Options\n
  do{
    printf("\n");
    printf("1:  --in | --out\n");
    printf("2:  --srcip src_ip [OPTIONAL]\n");
    printf("3:  --destip dest_ip [OPTIONAL]\n");
    printf("4:  --srcnetmask src_mask [OPTIONAL]\n");
    printf("5:  --destnetmask dest_mask [OPTIONAL]\n");
    printf("6:  --srcport src_port [OPTIONAL]\n");
    printf("7:  --destport dest_port [OPTIONAL]\n");
    printf("8:  --proto [ALL|TCP|UDP|ICMP]\n");
    printf("9:  --action [BLOCK|UNBLOCK]\n");
    printf("10: --print\n");
    printf("11: --delete num\n");
    printf("99: Exit\n\n");

    printf("Choose an Option: ");
    scanf("%d", &entry);
    printf("\n");

    switch(entry){
      case 1:
        printf("MANDATORY! \n");
        printf("--in | --out \n");
        printf("  --in: Inward Traffic\n");
        printf("  --out: Outward Traffic\n\n");
      break;

      case 2:
        printf("--srcip src_ip\n");
        printf("  src_ip format: *.*.*.*\n");
        printf("  Source IP Address to apply rule to.\n\n");
      break;

      case 3:
        printf("--destip dest_ip\n");
        printf("  dest_ip format: *.*.*.*\n");
        printf("  Destination IP Address to apply rule to.\n\n");
      break;

      case 4:
        printf("--srcnetmask src_mask\n");
        printf("  src_mask format: *.*.*.*\n");
        printf("  Source Netmask Address to apply rule to.\n\n");
      break;

      case 5:
        printf("--destnetmask dest_mask\n");
        printf("  dest_mask format: *.*.*.*\n");
        printf("  Destination Netmask Address to apply rule to.\n\n");
      break;

      case 6:
        printf("--srcport src_port\n");
        printf("  Source Port number to apply rule to.\n\n");
      break;

      case 7:
        printf("--destport dest_port\n");
        printf("  Destination Port number to apply rule to.\n\n");
      break;

      case 8:
        printf("MANDATORY! \n");
        printf("--proto [ALL|TCP|UDP|ICMP]\n");
        printf("  Which protocol(s) to block.\n\n");
      break;

      case 9:
        printf("MANDATORY! \n");
        printf("--action [BLOCK|UNBLOCK]\n");
        printf("  BLOCK: Block Traffic\n");
        printf("  UNBLOCK: Unblock Traffic\n\n");
      break;

      case 10:
        printf("--print: Print out All Rules\n");
        printf("  Usage: ./firewall --print\n\n");
      break;

      case 11:
        printf("--delete num: Delete num-th rule\n");
        printf("  Usage: ./firewall --delete 3\n");
        printf("  Deletes Third Rule.\n\n");
      break;

      case 99:
        // In to Avoid Default
      break;

      default:
        printf("Invalid Option, Please Try Again.\n\n");
      break;
    }
    pause();
  } while(entry != 99);
}

//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Helper Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~

int validIP( char* ip){
  int num;
  int flag = 1;
  int counter=0;
  char* p = strtok(ip,".");

  while (p && flag ){
    num = atoi(p);
    if (num>=0 && num<=255 && (counter++<4)){
      flag=1;
      p=strtok(NULL,".");
    } else {
      flag=0;
      break;
    }
  }
  return flag && (counter==4);
}

int isValueInArray(char* val, char* arr[], int size){
    int i;
    for (i=0; i < size; i++) {
        if (strcmp(arr[i], val) == 0)
            return TRUE;
    }
    return FALSE;
}

void upper(char* arr){
  for(int i=0;i<strlen(arr);i++){
    arr[i] = toupper(arr[i]);
  }
}

void pause(){
  char ch;
  printf("Press Enter to Continue. ");
  fseek(stdin,0,SEEK_END);
  getchar();
  //scanf(" %c",&ch);
    CLEAR_SCREEN
}
