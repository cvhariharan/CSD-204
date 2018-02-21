/* Name: C.V.Hariharan
ID: 1610110147
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int tokennize(char *, char, char **);

size_t size = 0;

int main(int argc, char *argv[])
{
  int fd[2];
  pid_t cpid;

  if(argc < 2)
    printf("Insufficient Arguments. Need src and dest.");
  else
    {
      pipe(fd); /* create pipe */

      FILE *fp = fopen(argv[1],"rb");
      fseek(fp,0,SEEK_END);
      size = ftell(fp);
      fseek(fp,0,SEEK_SET);
      fclose(fp);
      if(size > 0)
	{
	 cpid = fork();
	 if(cpid == -1)
	   {
	     perror("fork");
	     exit(1);
	   }
	 if(cpid == 0)
	   {
	     /*Close the write end*/
	     close(fd[1]);
	     int dest = open(argv[2], O_CREAT | O_WRONLY,766);
	     char *recv_content = malloc((size)*sizeof(char));
	     read(fd[0],recv_content,size);
	     write(dest,recv_content,size);
	     close(dest);
	     //fprintf(dest,"%s",recv_content);

	    }
	 else
	   {
	    printf("Size: %d bytes\n", size);
	    close(fd[0]);
	    int src = open(argv[1], O_RDONLY);
	    char *file_content;
	    file_content = (char *)malloc((size)*sizeof(char));
	    read(src,file_content, size);
	    // printf("%s\n", file_content);
	 
	    
	    write(fd[1], file_content, size);
	    close(src);
	    wait(NULL);
	   }
	}
     
    }
  
  return 0;
}
/*
int tokennize(char *input,char delim, char *output[])
{
  //Tokennizes the input using the delim and stores the tokens in output
  int i = 0,m = 0,n = 0,delimFound = 0,previousDelim = 0; 
  char *temp;
  char *start = input;
  int inputLen = strlen(input);
  for(;i < inputLen;i++)
    {
      if(*(input+i) == delim)
	{
	  //printf("%s\n",start);
	  delimFound = 1;
	  *(input+i) = '\0';
	  output[m] = (char *)malloc((i-previousDelim)*sizeof(char));
	  strcpy(output[m],start);
	  previousDelim = i+1;
	  start = input+i+1;
	  //printf("%s\n",output[m]);
	  m++;
	}
    }
  if(delimFound)
    {
      temp = (char *)malloc((i-previousDelim)*sizeof(char));
      temp = input+previousDelim;
      output[m] = temp;
      return m;
    }
  else
    {
      output[0] = input;
      return 0;
    }
}
*/
