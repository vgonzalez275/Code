#include <stdlib.h> /* required for rand_r(...) */
#include <stdio.h>
#include "buffer.h"  //buffer
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "stdint.h"
 

//**************************Initialization*****************************//
buffer_item buffer[BUFFER_SIZE];

//create semaphore
sem_t empty, full;

 // create the mutex lock
pthread_mutex_t mutex;
 
//function prototypes
int rand_r(unsigned int *seed);
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *param);
void *consumer(void *param);

//******************************BUFFER IN BUFFER OUT*******************//
int insert_item(buffer_item item) {
// insert item into buffer
 
 if(counter!=BUFFER_SIZE){
     buffer[in] = item;
     in =(in+1)%BUFFER_SIZE;
     counter++;
     return 0;
 }
 else{
     printf("Error, buffer full");
     return -1;
 } 
}

int remove_item(buffer_item *item) {
//remove an object from buffer
 if(counter!=0){
    *item = buffer[out];
    out=(out+1)%BUFFER_SIZE;
    counter--;
    return 0;
 }
 else{
    printf("error, buffer empty\n");
    return -1;
 } 
}
//*********************************CREATE PRODUCER/CONSUMER THREADS********************************//
void *producer(void *param) {
 int t=0;
 unsigned int seed=time(NULL);
 buffer_item item;
 
 while (1) {
  t=rand()%4;
  /* sleep for a random period of time */
  sleep(t);
  //produce an item in next_produced
  /* generate a random number */
  item = rand_r(&seed);
  sem_wait(&empty);
  pthread_mutex_lock(&mutex);
  //add next_produced to the buffer
  insert_item(item);
  pthread_mutex_unlock(&mutex);
  sem_post(&full);

  if (insert_item(item))
     printf("report error condition");
  else
     printf("producer produced %d\n", item);
 }
}
//*********************************CONSUMER*************************************
void *consumer(void *param) {
  buffer_item item;
  unsigned int seed=time(NULL);
  int t=0;
  
  while (1) {
     t=rand()%4;
    /* sleep for a random period of time */
    sleep(t);
    item= rand_r(&seed);
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    
    //remove an item from buffer to next_consumed
    remove_item(&item);
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    if (remove_item(&item))
       printf("report error condition");
    else
       printf("consumer consumed %d\n", item);
  }
}

//****************************THREAD CREATION USIGN PTHREAD*****************************

int main(int argc, char *argv[]) {
    int time;
    int produce;
    int consume;
/* 1. Get command line arguments argv[1],argv[2],argv[3] */
    if (argc > 3){
       time= strtol(argv[1], NULL, 0);
       produce = strtol(argv[2], NULL, 0);
       consume = strtol(argv[3], NULL, 0);
    }
    else{
	fprintf(stderr, "usage: buffer [sleep time] [# to produce] [# to consume]\n");
    }
/* 2. Initialize buffer */
    int i,j; 

    // create the semaphore and initialize it to 5 
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);
    // 3. Create producer thread(s) 
    pthread_t prod_thread[produce];
    for(i=0;i<produce;i++){
      pthread_create(&prod_thread[i], NULL, producer, NULL);    
    }
/* 4. Create consumer thread(s) */
    pthread_t cons_thread[consume];
    for(j=0;j<consume; j++){
       pthread_create(&cons_thread[j], NULL, consumer, NULL);
       
    }
/* 5. Sleep */
    sleep(time);
/* 6. Exit */
    return 0;
}

