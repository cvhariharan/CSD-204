#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define T_SIZE 15
#define ARG_SIZE 80
#define MAX_HISTORY 100

int storeCommand(char *);
int tokennize(char *, char, char *b[]);
void checkAmpersand(int *, char *);
void showLatestCommands(int);

int main(int argc, char *argv[])
{
  pid_t pid;
  char *allArgs;
  char *args[ARG_SIZE/2];
  char *token;
  int dontWait = 0;
  const char *delim = " ";
  allArgs = (char *)malloc(ARG_SIZE*sizeof(char));
  token  = (char *)malloc(T_SIZE*sizeof(char));

  while(1)
    {
      int i = 0;
      printf("Hari's Shell> ");
      gets(allArgs);
      storeCommand(allArgs);
      //Split the command line arguments
      i = tokennize(allArgs,' ',args); //Returns the index of last element + 1
      args[i] = NULL;
      checkAmpersand(&dontWait,args[--i]); //Sends the last argument to the function
      
      if(strcmp(args[0],"history") == 0)
	showLatestCommands(10); //Show the latest 10 commands

      else if(strcmp(args[0], "exit") == 0)
	exit(0);

      else
	{
	  pid = fork();

	  if(pid < 0)
	    fprintf(stderr, "Could not create a child process");

	  else if(pid == 0)
	    { //Code that will be executed by the child process
	      execvp(args[0],args);
	      exit(1);
	    }
	  else 
	    {
	      if(!dontWait)
		{
		  wait(NULL);
		}
	      else
		{
		  dontWait = 0;
		  continue;
		}
	    }
      	}
    }
  return 0;
}

int storeCommand(char *command)
{
  FILE *fp = fopen("commands.txt","a+");
  fprintf(fp,"%s\n",command);
  fclose(fp);
}

void showLatestCommands(int num)
{
  FILE *fp = fopen("commands.txt","r");
  ssize_t m;
  char *line = NULL, *comms[MAX_HISTORY];
  size_t n = 0;
  int i = 0,p = 0;
  while((m = getline(&line,&n,fp))!= -1)
    {
      comms[i] = (char *)malloc(n*sizeof(char));
      comms[i] = line;
      line = NULL;
      i++;
    }
  for(p = --i; p >= 0; p--)
    {
      if((i-p) == num)
	break;
      else
	printf("%s\n", comms[p]);
    }
  fclose(fp);
}

void checkAmpersand(int *dontWait, char *lastArg)
{
  //Checks the last character in the last argument for ampersand
  int lastIndex = strlen(lastArg);
  printf("%d %c\n",lastIndex,lastArg[--lastIndex]);
  if(lastArg[lastIndex] == '&')
    {
      printf("Don't wait.\n");
      *dontWait = 1;
    }
}

int tokennize(char *input,char delim, char *output[])
{
  int i = 0,m = 0,n = 0,delimFound = 0,previousDelim = 0; 
  char *temp;
  char *start = input;
  for(;i < strlen(input);i++)
    {
      if(*(input+i) == delim)
	{
	  delimFound = 1;
	  temp = (char *)malloc(i*sizeof(char));
	  temp = start;
	  temp[i] = '\0';
	  output[m] = temp;
	  start = input+1+i;
	  previousDelim = i+1; //The index just after the previous delim index
	  m++;
	}
    }
  if(delimFound)
    {
      temp = (char *)malloc((i-previousDelim)*sizeof(char));
      temp = input+previousDelim;
      output[m] = temp;
      return ++m;
    }
  else
    {
      output[0] = input;
      return 1;
    }
  }
