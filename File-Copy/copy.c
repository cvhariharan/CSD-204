/* 
   Name: C.V.Hariharan
   ID: 1610110147
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>

#define BUFFER 100

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
      int src_d = open(argv[1], O_RDONLY);
      DIR* dir;
if((dir = opendir(argv[1])) == NULL)
  {
      if(src_d == -1)
	{
	  printf("No such file exists.\n");
	  exit(0);
	}
      size = lseek(src_d, 0, SEEK_END);
lseek(src_d, 0, SEEK_SET);
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
	     char *loc[20];
	     char path[100];
	     strcpy(path,argv[2]);
	     //Create a path to the directory
	     int tokens = tokennize(argv[2],'/',loc); //Return a tokennized array of locations. The last element will be the file name.
	     char dirname[100];
	     //int start = (loc[0]==NULL)?1:0;
	     int start = 0;
	     strcpy(dirname,loc[start]);
	     for(int i=start+1;i < tokens-1;i++)
	       {
		 strcat(dirname,"/");
		 strcat(dirname,loc[i]);
	       }
	     printf("%s\n",dirname);
	     printf("%s\n",loc[tokens-1]);

	     mkdir(dirname, 0755);
	     if(strlen(loc[tokens-1]) != 0)
	       {
		 //Run only if the file name is not empty
	     int dest = open(path, O_RDONLY);
	     
	     if(dest == -1)
	       {
		 int i = 1;
		 dest = open(path, O_CREAT | O_WRONLY,766);
		 char *recv_content = malloc((size)*sizeof(char));
		 if(size > BUFFER)
		   {
		     int iterations = size/BUFFER;
		     int remaining = size - (BUFFER * iterations);
		     read(fd[0],recv_content,BUFFER);
		     write(dest,recv_content,BUFFER);
		     for( ; i < iterations ; i++)
		       {
			 lseek(fd[0],i*BUFFER, SEEK_SET);
			 read(fd[0],recv_content,BUFFER);
			 lseek(dest, i*BUFFER, SEEK_SET);
			 write(dest,recv_content,BUFFER);
			 
		       }
		     lseek(dest, i*BUFFER, SEEK_SET);
		     read(fd[0],recv_content,remaining);
		     lseek(dest, i*BUFFER, SEEK_SET);
		     write(dest,recv_content,remaining);
		   }
		 else
		   {
		     read(fd[0],recv_content,size);
		     write(dest,recv_content,size);
		   }
	       }
	     else
	       {
		 printf("A file with the same name exists. Do you want to overwrite? y/n: ");
		 char c;
		 scanf("%c", &c);
		 switch(c)
		   {
		   case 'Y':
		   case 'y': dest = open(path, O_CREAT | O_WRONLY | O_TRUNC,766);
		     char *recv_content = malloc((size)*sizeof(char));
		     read(fd[0],recv_content,size);
		     write(dest,recv_content,size);
		     break;
		   case 'N':
		   case 'n': exit(0);
		   }
	       }
	     close(dest);
	       }
	     else
	       printf("File name cannot be empty.\n");
	     //fprintf(dest,"%s",recv_content);

	    }
	 else
	   {
	    printf("Size: %d bytes\n", size);
	    close(fd[0]);
	    int src = open(argv[1], O_RDONLY);
	    if(src == -1)
	      {
		printf("No such file exists!");
		exit(0);
	      }
	    char *file_content;
	    file_content = (char *)malloc((size)*sizeof(char));
	   if(size > BUFFER)
		   {
		     int i = 1;
		     int iterations = size/BUFFER;
		     int remaining = size - (BUFFER * iterations);
		     read(src,file_content,BUFFER);
		     write(fd[1],file_content,BUFFER);
		     for( ; i < iterations ; i++)
		       {
			 lseek(src,i*BUFFER, SEEK_SET);
			 read(src,file_content,BUFFER);
			 lseek(fd[1], i*BUFFER, SEEK_SET);
			 write(fd[1],file_content,BUFFER);
			 
		       }
		      lseek(src,i*BUFFER, SEEK_SET);
		      read(src,file_content,BUFFER);
		      lseek(fd[1], i*BUFFER, SEEK_SET);
		      write(fd[1],file_content,BUFFER);
		   }
		 else
		   {
		     read(src,file_content,size);
		     write(fd[1],file_content,size);
		   }
	    close(src);
            wait(NULL);
	  
	   }
	}
}
 else
   printf("Trying to copy a directory.\n");
     
    }
  
  return 0;
}

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
      return ++m;
    }
  else
    {
      output[0] = input;
      return 1;
    }
}
