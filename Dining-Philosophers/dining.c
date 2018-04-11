#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

void *philosopher(void *);

int *forks; //1 if used for each fork
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t *lock;
pthread_cond_t *cond;
int n; //Number of philosophers

void main()
{
  
  int i,j;
  scanf("%d",&n);
  pthread_t philosophers[n];
  forks = (int *)malloc(n*sizeof(int));
  cond = (pthread_cond_t *)malloc(n*sizeof(pthread_cond_t));
  lock = (pthread_mutex_t *)malloc(n*sizeof(pthread_mutex_t));
  for(i=0;i<n;i++)
    {
       pthread_create(&philosophers[i],NULL,philosopher,&i);
       forks[i] = 0;
       pthread_mutex_init(&lock[i], NULL);
       pthread_cond_init(&cond[i], NULL);
       
    }
  for(i=0;i<n;i++)
    {
      pthread_join(philosophers[i], NULL);
    }
}

void pickup_forks(int philosopher_id)
{
  int fork1 = philosopher_id;
  int fork2 = (philosopher_id + n-1)%n;
  int small_fork = (fork1 < fork2)?fork1:fork2;
  int large_fork = (fork1 > fork2)?fork1:fork2;
  pthread_mutex_lock(&lock[small_fork]);
  
  while(forks[small_fork] != 0)
    pthread_cond_wait(&cond[small_fork],&lock[small_fork]);
  forks[small_fork] = 1;
  printf("Philosopher %d got fork %d\n",philosopher_id,small_fork);
  pthread_mutex_lock(&lock[large_fork]);
  
  while(forks[large_fork] != 0)
    pthread_cond_wait(&cond[large_fork],&lock[large_fork]);
  forks[large_fork] = 1;
  printf("Philosopher %d got fork %d\n",philosopher_id,large_fork);
  pthread_mutex_unlock(&lock[large_fork]);
    
  pthread_mutex_unlock(&lock[small_fork]);

  
}

void return_forks(int philosopher_id)
{
  int fork1 = philosopher_id;
  int fork2 = (philosopher_id + n-1)%n;
  int small_fork = (fork1 < fork2)?fork1:fork2;
  int large_fork = (fork1 > fork2)?fork1:fork2;
  pthread_mutex_lock(&lock[small_fork]);
  
  forks[small_fork] = 0;
    pthread_cond_signal(&cond[small_fork]);
  printf("Philosopher %d released fork %d\n",philosopher_id,small_fork);
  pthread_mutex_lock(&lock[large_fork]);
  
  forks[large_fork] = 0;
    pthread_cond_signal(&cond[large_fork]);
  printf("Philosopher %d released fork %d\n",philosopher_id,large_fork);
  pthread_mutex_unlock(&lock[large_fork]);
    
  pthread_mutex_unlock(&lock[small_fork]);
}

void *philosopher(void *p)
{
  int *id = (int *)p;
  int p_id = *id;
  //printf("Philosopher %d\n",*id);
   srand(time(NULL));
  while(1)
    {
      int r = (1 + rand()) % 4;
      printf("Philosopher %d is thinking\n",p_id);
      sleep(r);
      pickup_forks(p_id);
      printf("Philosopher %d is eating\n",p_id);
      return_forks(p_id);
    }
}
