#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include "pagealgos.h"

#define OPTIMAL 0
#define L_R_U 1
#define F_I_F_O 2

void retrieve(Frame *, int *, int, int);
int hit(Frame *, int);

int clock = 0; //For LRU. Incremented with each memory access and added to the corresponding frame.
int fn = 0;
int MAX_PAGES = 20;
int hit_counter[3];
int miss_counter[3];
int next_frame = 0;
int head_pointer; //For FIFO
struct stat st;

void main(int argc, char *argv[])
{
  int frame_sizes[] = {1,4,6,10};
  int frame_tests = 0;
  while(frame_tests < 4)
    {
      fn = frame_sizes[frame_tests];
      int i = 0;
      Frame *framesOptimal;
      Frame *framesLRU;
      Frame *framesFIFO;
      //int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,7,0,1}; //{1,4,4,3,8,10,14,19,4,6,8,15,16,2,15,19,0,2,9,3};
      framesOptimal = (Frame *)malloc(fn*sizeof(Frame *));
      framesLRU = (Frame *)malloc(fn*sizeof(Frame *));
      framesFIFO = (Frame *)malloc(fn*sizeof(Frame *));
  

      //Reading from the input file
      if(argc >= 2)
	{
	  FILE *fp;
	  char *data,*token;
	  int arr_size;
	  fp = fopen(argv[1],"r+");
	  stat(argv[1], &st);
	  arr_size = st.st_size;
	  int pages[arr_size];
	  //printf("%d A\n", arr_size);
	  data = (char *)malloc(arr_size*sizeof(char));
	  fscanf(fp, "%s", data);
	  //printf("Data: %s\n", data);
	  token = (char *)malloc(8*sizeof(char));
	  token = strtok(data, ",");
	  while(token != NULL)
	    {
	      pages[i] = atoi(token);
	      token = strtok(NULL, ",");
	      // arr[i] = atoi(token);
	      i++;
	    }
	  MAX_PAGES = i;
  
	  //Initialization
	  head_pointer = 0;
	  for(i=0;i<3;i++)
	    {
	      hit_counter[i] = 0;
	      miss_counter[i] = 0;
	    }
	  for(i=0;i<fn;i++)
	    {
	      framesOptimal[i].number = MAX_PAGES+1;
	      framesLRU[i].number = MAX_PAGES+1;
	      framesFIFO[i].number = MAX_PAGES+1;
	    }
  
	  for(i=0;i<MAX_PAGES;i++)
	    {
	      retrieve(framesOptimal,pages,i,OPTIMAL);
	    }
	  next_frame = 0;
	  clock = 0;
	  for(i=0;i<MAX_PAGES;i++)
	    {
	      retrieve(framesLRU,pages,i,L_R_U);
	    }
	  next_frame = 0;
	  clock = 0;
	  for(i=0;i<MAX_PAGES;i++)
	    {
	      retrieve(framesFIFO,pages,i,F_I_F_O);
	    }
	  printf("Optimal - Frame Size: %d\n",fn);
	  printf("Hits: %d, Misses: %d\n",hit_counter[OPTIMAL],miss_counter[OPTIMAL]);
	  printf("\n");
	  printf("LRU - Frame Size: %d\n",fn);
	  printf("Hits: %d, Misses: %d\n",hit_counter[L_R_U],miss_counter[L_R_U]);
	  printf("\n");
	  printf("FIFO - Frame Size: %d\n",fn);
	  printf("Hits: %d, Misses: %d\n",hit_counter[F_I_F_O],miss_counter[F_I_F_O]);
	  printf("--------------------------------------------------------------------\n\n");
	}
      else
	printf("No input file given.");
      frame_tests++;
    }
}

void retrieve(Frame frames[], int pages[],int index, int algo)
{
  int victim = 0;
  if(hit(frames,pages[index]))
    {
      hit_counter[algo]++;
      //printf("Hit %d\n",pages[index]);
    }
  else
    {
      miss_counter[algo]++;
      if(next_frame < fn)
	{
	  frames[next_frame].number = pages[index];
	  //printf("Victimq: %d\n",next_frame);
	  frames[next_frame].priority = clock;
	  next_frame++;
	  
	}
      else
	{
	  if(algo == OPTIMAL)
	    victim = optimal(frames,pages,index);//LRU(frames,pages,index);
	  else if(algo == L_R_U)
	    victim = LRU(frames,pages,index);
	  else if(algo == F_I_F_O)
	    victim = FIFO();
	  //printf("Victim: %d\n",victim);
	  frames[victim].number = pages[index];
	  frames[victim].priority = clock;
	}
      
      
      clock++;
    }
 
}


int hit(Frame frames[], int p)
{
  //int fn = sizeof(frames)/sizeof(int),i;
  int i;
  for(i=0;i<fn;i++)
    {
      if(frames[i].number == p)
	return 1;
    }
  return 0;
}

int optimal(Frame frames[], int pages[],int index)
{
  //int fn = sizeof(frames)/sizeof(int),i,j;
  int pn = MAX_PAGES,i,j;
  int greatest=0,victim=0;
  for(i=0;i<fn;i++)
    {
      for(j=index;j<pn;j++)
	{
	  if(frames[i].number == pages[j])
	    {
	      if(j > greatest)
		{
		  greatest = j;
		  victim = i;
		}
	      break;
	    }
	}
      //No hit for the page
      if(j == pn)
	{
	  victim = i;
	  break;
	}
    }
  return victim;
}

int LRU(Frame frames[], int pages[], int index)
{
  int pn = MAX_PAGES,i,j,victim = 0;
  int smallest = INT_MAX;
  //printf("MAX INT %d\n",smallest);
  for(i=0;i<fn;i++)
    {
      if(frames[i].priority < smallest)
	{
	  smallest = frames[i].priority;
	  victim = i;
	}
    }
  return i;
}

int FIFO()
{
  int i = head_pointer;
  head_pointer++;
  if(head_pointer >= fn)
    head_pointer = 0;
  return i;
}
