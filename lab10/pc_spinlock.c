#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "spinlock.h"


#define MAX_ITEMS 10

int items = 0;



const int NUM_ITERATIONS = 200;
const int NUM_CONSUMERS  = 2;
const int NUM_PRODUCERS  = 2;

int producer_wait_count;     // # of times producer had to wait
int consumer_wait_count;     // # of times consumer had to wait
int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items


spinlock_t lock;

	
void produce() {
  // TODO ensure proper synchronization
  while (items >= MAX_ITEMS) {
	 assert ((items >= 0) && (items <= MAX_ITEMS));
  }
  spinlock_lock(&lock);

  if (items >= MAX_ITEMS) {
	  spinlock_unlock(&lock);
	  producer_wait_count++;
	  assert ((items >= 0) && (items <= MAX_ITEMS));
	  produce();
  } else {
  items++;
  assert ((items >= 0) && (items <= MAX_ITEMS));
  histogram [items] += 1;
  
  spinlock_unlock(&lock);
  }
}

void consume() {
  // TODO ensure proper synchronization
  while (items == 0) {
	  assert ((items >= 0) && (items <= MAX_ITEMS));
  }
  spinlock_lock(&lock);
  if (items == 0) {
	  spinlock_unlock(&lock);
	  consumer_wait_count++;
	   assert ((items >= 0) && (items <= MAX_ITEMS));
	  consume();
  } else{
  items--;
   assert ((items >= 0) && (items <= MAX_ITEMS));
  histogram [items] += 1;
  spinlock_unlock(&lock);
  }
  

}

void* producer() {
  for (int i=0; i < NUM_ITERATIONS; i++)
    produce();
}

void* consumer() {
  for (int i=0; i< NUM_ITERATIONS; i++)
    consume();
}

int main (int argc, char** argv) {
  // TODO create threads to run the producers and consumers
  spinlock_create(&lock);
  uthread_init(4);
  uthread_t producerThread1;
  uthread_t producerThread2;
  uthread_t consumerThread1;
  uthread_t consumerThread2;
  producerThread1 = uthread_create(producer, 0);
  producerThread2 = uthread_create(producer, 0);
  consumerThread1 = uthread_create(consumer, 0);
  consumerThread2 = uthread_create(consumer, 0);
  uthread_join(producerThread1, 0);
  uthread_join(producerThread2, 0);
  uthread_join(consumerThread1, 0);
  uthread_join(consumerThread2, 0);
  printf("Producer wait: %d\nConsumer wait: %d\n",
         producer_wait_count, consumer_wait_count);
  for(int i=0;i<MAX_ITEMS+1;i++)
    printf("items %d count %d\n", i, histogram[i]);
/*   uthread_join(producerThread1, 0);
  uthread_join(producerThread2, 0);
  uthread_join(consumerThread1, 0);
  uthread_join(consumerThread2, 0); */
}