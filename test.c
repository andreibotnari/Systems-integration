/*Systems Integration DT211C-4
Assignment 1: Networking Shell
Andrei Botnari C13383341

Documentation:How to install and operate
1.Downlad "test.c" file.
2.Open terminal. 
3.cd into the directory where the file is saved.
4.Compile the program "gcc -o test test.c".
5.Execute the program "./test".
6.Done.
*/
#include <stdio.h>
#include <stdlib.h>                                                                           
#include <unistd.h>                                                                         
#include <string.h>                                                                         
#include <sys/types.h>                                                                     
#include <signal.h>                                                                            
#include <sys/wait.h>                                                                       
#include <fcntl.h>                                                                         
#include <termios.h> 
#include <time.h> 
#include <limits.h> 
#include <grp.h>    
                                       
#define TRUE 1
#define STDIN 1
#define BUFFER_MAX_LENGTH 50                                                    
static char* currentDirectory;                
static char userInput = '\0';
static char buffer[BUFFER_MAX_LENGTH];                                       
static int bufferChars = 0;
static char *commandArgv[5]; 
static int cmdArgCount = 0;                                                

// gets line form user
void getTextLine();
// populates the getline
void populateCommand();
// clears the buffer
void destroyCommand();

void welcomeScreen();
//prompt display
void shPrompt();

// Shell Functions
void handleUserCommand();
// error check 
int checkBuiltInCommands();
//command excution
void executeCommand(char *command[], char *file, int newDescriptor);
// child process created
void launchJob(char *command[], char *file, int newDescriptor);
// commands built in 
void changeDirectory();
void internalDate();
void userDetails();
void pwd();
void ifconfig();
void help();
// initializes the shell 
void init();

//takes in the text from the user
 
void getTextLine(){
        destroyCommand();                                                                      
 // clean buffer and command line
        while ((userInput != '\n') && (bufferChars < BUFFER_MAX_LENGTH)) {
                buffer[bufferChars++] = userInput;
                userInput = getchar();
        }
        buffer[bufferChars] = 0x00;                                                            
// null terminate buffer
        populateCommand();
}

void populateCommand(){
        char* bufferPointer; 
        bufferPointer = strtok(buffer, " ");
        while (bufferPointer != NULL) { 
                commandArgv[cmdArgCount] = bufferPointer;  
                bufferPointer = strtok(NULL, " ");
                cmdArgCount++;
        }
}

//destroys chars, lets user to input the next command line
 
void destroyCommand(){
        while (cmdArgCount != 0) {
                commandArgv[cmdArgCount] = NULL;
                cmdArgCount--;                                                
        }
        bufferChars = 0;                                
}

void welcomeScreen(){
		printf("\n");
        printf("\t\t\tWelcome to My Custom Shell! :)\n");
	printf("\t\t\t\tLost? Press h");
        printf("\n\n");
}

// shell logname 
void shPrompt(){
        printf("sh_%s | ", getenv("LOGNAME"));
}

//execute built in commands then everything else
void handleUserCommand(){
        if ((checkBuiltInCommands(commandArgv[0])) == 0) {
                printf("This command is not available\n");
        }
}

// built-in commands: exit, cd, dt, ud, pw, ifc, help, h
int checkBuiltInCommands(){
        if (strcmp("exit", commandArgv[0]) == 0) {
                exit(EXIT_SUCCESS);
        }
        if (strcmp("cd", commandArgv[0]) == 0) {
                changeDirectory();
                return 1;
        }
		if (strcmp("dt", commandArgv[0]) == 0) {
		         internalDate();
		         return 1;
		}
		if (strcmp("ud", commandArgv[0]) == 0) {
				  userDetails();
				  return 1;
		}
		if (strcmp("pw", commandArgv[0]) == 0) {
					pwd();
					return 1;
		}
		if (strcmp("ifc", commandArgv[0]) == 0) {
					ifconfig();
					return 1;
		}
		if (strcmp("help", commandArgv[0]) == 0) {
					help();
					return 1;
		}
		if (strcmp("h", commandArgv[0]) == 0) {
					help();
					return 1;
		}
        return 0;
}

// change directory 
void changeDirectory(){
        if (commandArgv[1] == NULL) {
	                chdir(getenv("HOME"));
        } else {
                if (chdir(commandArgv[1]) == -1) {
                        printf(" %s: no such directory\n", commandArgv[1]);
                }
        }
}

//internalDate() prints out time and date
void internalDate() {
		printf("\n");
		
		time_t t;
		struct tm *tmp;
		char buf[64];
		time(&t);
		tmp = localtime(&t);
		strftime(buf, 64, "Time and Date:  %Y%m%d%H%M%S", tmp);
		printf("%s\n", buf);
		
		printf("\n");
}


// User details
 
void userDetails(){
		printf("\n");
		
		struct group *gr;
		int ngroups, i;
		gid_t groups[NGROUPS_MAX];

		ngroups = NGROUPS_MAX;
		if ( getgrouplist( getlogin(), getegid(), groups, &ngroups) == -1) {
			printf ("Groups array is too small: %d\n", ngroups);
		}
		
		printf("%d, %s," , geteuid(), getenv("LOGNAME"));
		for (i=0; i < ngroups; i++) {
			 gr = getgrgid(groups[i]);
			printf(" %s," ,  gr->gr_name);
		}
		printf(" %s", getcwd(getenv("HOME"), 1024));
		printf("\n");
		printf("\n");
}


//  pwd
 
void pwd() {
		printf("\n");
		system("pwd");
		printf("\n");
}


//  help menu
 
void help() {
		
		if (commandArgv[1] != NULL ) {
			if(strcmp(commandArgv[1], "ifc") == 0){
				printf("\n");
				printf("ifc: Prints out the ifconfig of the eth0 by default.\n");
				printf("\n");
			}
			else if (strcmp(commandArgv[1], "pw" ) == 0){
				printf("\n");
				printf("pw | Prints out the current  directory.\n");
				printf("\n");
			}
			else if (strcmp(commandArgv[1], "dt" ) == 0){
				printf("\n");
				printf("dt | Prints out the date.\n");
				printf("\n");
			}
			else if (strcmp(commandArgv[1], "ud" ) == 0){
				printf("\n");
				printf("ud | Prints out the user details .\n");
				printf("\n");
			}
			else if (strcmp(commandArgv[1], "exit" ) == 0){
				printf("\n");
				printf("exit | Exit test shell.\n");
				printf("\n");
			}
			else {
				printf("\n");
				printf("%s | has no help implemented.\n", commandArgv[1]);
				printf("\n");

			}
		}
		else {
				if (commandArgv[1] == NULL) {
					printf("\n");
					printf("1.-ud   | Prints out the userid, groupid, username and groupname.\n");
					printf("2.-dt   | Prints out the current date.\n");
					printf("3.-pw   | prints out the current working directory.\n");
					printf("4.-ifc  | Prints out the ifconfig of the eth0 by default.\n");
					printf("5.-exit | Exit test shell.\n");
					printf("\n");
				}
			}
}


// ifconfig ()
 
void ifconfig() {
	char command[100];
	if (commandArgv[1] == NULL) {
		printf("\n");
		system("ifconfig eth0");
		printf("\n");
	} else {
// go to the new directory
	    if (commandArgv[1] != NULL) {                                  
	       	printf("\n");
			sprintf(command, "ifconfig %s", commandArgv[1]);
			system(command);
			printf("\n");
	    }
	}
}

int main(int argc, char **argv, char **envp){
        //init();
        welcomeScreen();
        shPrompt();                                                                            
        while (TRUE) {
                userInput = getchar();
                switch (userInput) {
                case '\n':                                        
                        shPrompt();                                                                    
                        break;
                default:
                        getTextLine();                                                                  
                        handleUserCommand();                                                          
                        shPrompt();                                                                 	
                        break;
                }
        }
        printf("\n");
        return 0;
}
