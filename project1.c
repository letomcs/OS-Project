/*README README README README
The focus of this part of the project is to use fork, PATH variables, and execv.
A lot of this comes from the skeleton given in the prompt from the assignment, 
the rest is just attempting to figure out what connects with what I am given at the start.
Overall, what it does can be diffcult to see originally, but over time, it can be self explanatory.
//Prologue
The vague runthrough or what main method does is grabs and parses every path in PATH into the array dir or directory.
That is where we will attempt to find commands.
Then, it sticks us in a while loop and uses readcommand to grab the input for the shell.
Parsecommand breaks it down by whitespace in minishell.h to individual pieces which go into argv[0].
Then, using lookup path along with the argv and dir arrays, 
then concats the string of argv[0] or the main command with other directories to see if it can be accessed/ if it exists in that directory
If it can be accessed in a certain directory, it allocates more space for the path and replace the original string in argv[0].
Unless the input is quit, a child process is ran that uses execv to run the command, where the first param is the command path, and argv entries until NULL are params.
Once the command completes, child informs us/the parent process that the process finished and is terminated.
Finally, once you type quit or enter ctrl+c, main method ends the loop and prints that it is terminating.
//WARNING WARNING WARNING WARNING
Ensure you have minishell.h as it has important values defined that are needed
Avoid using directory changing commands or text document commands like vi. the former is true, because it can't interpret '.' as it sees it as WHITESPACE
if you want to risk working with directory commands, remove '.' in whitespaces but i cant guarentee your safety.
The latter should be avoided, since vi and the gets command become finnicky and fight for input. You wont be able to enter :wq or :q
to leave, so you are forced to ctrl+c.
*/
//#include ...
#include <string.h> //necessary for strcopy, strcat
#include <stdlib.h> //necessary for malloc
#include <unistd.h> //necessary for access, execv, fork etc
#include <stdio.h>
#include "minishell.h" //contains defined values

char *lookupPath(char **argv,char **dir){
//This function searches directories identified by the dir arg to see if argv[0] or file name is there
//Allocate a new string, place full path name in it, then return it
char pName[MAX_PATHS_LEN];
//check to see if file name is already an absolute path name
if(*argv[0]=='/'){
if(access(argv[0],0)==0){
printf("argv[0] = %s\n",argv[0]);
return argv[0];
}
fprintf(stderr,"%s : command not found\n",argv[0]);
return NULL;
}
//look in PATH directories
//use access() to see if file is in dir
for(int i=0;i<MAX_PATHS;i++){
char tpath[MAX_PATHS_LEN];//temporary absolute path
if(dir[i]==NULL){
        break;
}
//concats file name to directories and checks if it can be accessed.
strcpy(tpath,dir[i]);//copy dir[i] to tpath
strcat(tpath,"/");//append '/' to tpath
strcat(tpath,argv[0]);
strcpy(pName,tpath);
if(access(pName,0)==0){
printf("lookupPath: Found %s in path %s (full path is %s )\n",argv[0],dir[i],pName);
//stores command path to argv[0]
argv[0]=(char *) malloc(MAX_PATHS_LEN);
strcpy(argv[0],pName);
return argv[0];
}
}//file name not found
fprintf(stderr, "%s: command not found\n", argv[0]);
return NULL;
}

int parseCommand(char *cmdL, struct command_t *command){
char **cmdptr;
int argc = 0;
//creates pointer and number of arguments
cmdptr = &cmdL; //getting command line
command->argv[argc] = (char *) malloc(MAX_ARGS_LEN); //used arrow to access structure
while((command->argv[argc]=strsep(cmdptr, WHITESPACE))!=NULL){
//basically saying while the function isnt having issues running, continue with the other function
command->argv[++argc]=(char *)malloc(MAX_ARGS_LEN);
//allocates memory to argv for as long as there are directories
}
//honestly at this part I played with the arrows with pointer issues and hoped it worked
command->argc=argc-1;
command->name= (char *) malloc(sizeof(command->argv[0]));
//moves arguments down one and allocates main command directory at argv[0]
strcpy(command->name,command->argv[0]);//copies strings from pointer 2 to 1
return 0;//tells whoever ran this that it actually worked
}

int parsePath(char **dir){
//This function reads PATH variable for this envir, then builds an array, dir[]
char *pathEnvVar;
char *thePath;
for(int i=0;i<MAX_PATHS;i++){
	dir[i]=NULL;
}
//empties dir
pathEnvVar=(char *) getenv("PATH");
thePath=(char *) malloc(strlen(pathEnvVar)+1);
strcpy(thePath, pathEnvVar);
// Loop to parse thePath. Look for a ':' or the delimiter between each path name
for(int i=0;i<MAX_PATHS;i++){
	dir[i]=strsep(&thePath,":");
	if(dir[i]==NULL){
		break;
	}
}
//parses path based off of ':' then stores in dir
printf("Directories being searched in: \n");
for(int i=0;i<MAX_PATHS;i++){
	if(dir[i]==NULL){
	break;
	}
printf("directory[%d]: %s\n",i,dir[i]);
}
//prints directories for myself
return 0;
}

void printPrompt(){
//build the prompt string to have the machine name along with current directory
char promptstring[] = ">> ";
printf("%s ", promptstring);
}

void readCommand(char *buffer){
gets(buffer);//Im praying its this simple like the skeleton says
}

int main(int argc, char *argv[]){ //required to actually get and use the struct
char *pathv[MAX_PATHS];
struct command_t command;
//Shell Initialization
printf("Running background methods...\nType quit to terminate shell...\n");
parsePath(pathv); //Get directory paths from PATH
while(1==1){
char* commandLine=(char*)malloc(LINE_LEN);
	printPrompt();
//read command line and parse it
	readCommand(commandLine);
	parseCommand(commandLine, &command);
	if(strcasecmp(commandLine,"quit")==0){
	break;
}
//Grabs command and parses
//get the full pathname for the file
	command.name = lookupPath(command.argv, pathv);

//gets directory for command
	if(command.name==NULL){
	fprintf(stderr, "%s: command unknown", command.name);
	continue;
	}
//Create child and execute command
int childPid=fork();
if(childPid==0){
//runs command with child process
	execv(command.name,command.argv);
	exit(0);
}else{
	//Wait for the child to terminate
	if(wait(NULL) > 0) /* Child terminating */
//tells parent/us that child process terminated
	printf("fork terminated\n");
}
}
//Shell terminates
printf("Terminal closed. Goodbye.\n");
return 0;
}
