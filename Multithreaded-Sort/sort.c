#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

int sort_and_merge();
void *sort(void *);

typedef struct
{
  pthread_t tid;
  int start;
  int end;
}parameters;

int arr[] = {1,23,53,16,75,43,89,21,77,88,90,91,26,77,15,211,48};
int arr_size = 0;
int main()
{
  sort_and_merge();
  return 0;
}


int sort_and_merge()
{
  int arr_size = sizeof(arr)/sizeof(int);
  int sorted[arr_size];
  int i=0,j=0,k=0;
  void **sort_arr1, **sort_arr2;
  
  pthread_t sort_thread1, sort_thread2;
  pthread_attr_t attr1, attr2;

  pthread_attr_init(&attr1);
  pthread_attr_init(&attr2);

  parameters *data1 = (parameters *)malloc(sizeof(parameters));
  parameters *data2 = (parameters *)malloc(sizeof(parameters));

  data1->start = 0;
  data1->end = 8;
  data2->start = 8;
  data2->end = arr_size-1;

  pthread_create(&sort_thread1, &attr1, sort, data1);
  pthread_create(&sort_thread2, &attr2, sort, data2);

  pthread_join(sort_thread1, NULL);
  pthread_join(sort_thread2, NULL);

  i = data1->start;
  j = data2->start;
  while(i < data1->end && j < data2->end)
    {
      if(arr[i] <= arr[j])
	{
	  sorted[k] = arr[i];
	  i++;
	}
      else
	{
	  sorted[k] = arr[j];
	  j++;
	}
      k++;
    }

  printf("Sorted Array...\n");
  for(i=0; i < arr_size; i++)
    printf("%d\n",sorted[i]);
}

void *sort(void *param)
{
  parameters *data = (parameters *) param;
  int *temp_arr;
  int swap, i;
  do
    {
      swap = 0;
      i = data->start;
      while(i <= data->end - 1)
	{
	  if(arr[i] > arr[i+1])
	    {
	      int temp = arr[i];
	      arr[i] = arr[i+1];
	      arr[i+1] = temp;
	      swap = 1;
	    }
	  i++;
	}

    }while(swap != 0);

  temp_arr = (int *)malloc((data->end - data->start)*sizeof(int));
  
  printf("Thread ID: %d\n", pthread_self());
  for(i = data->start; i < data->end; i++)
    printf("%d\n",arr[i]);
  
}

