#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

int sum = 0;
queue_t rhbq;

void interruptServiceRoutine () {
  // TODO
  uthread_t dequeueThread = queue_dequeue(&rhbq);
  uthread_unblock(dequeueThread);
}

void blockUntilComplete() {
  // TODO
  uthread_block();
}

void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
  sum += *(((int*) buf) + 1);
}

/**
 * Struct provided as argument to readAndHandleBlock
 */
struct readAndHandleBlockArgs {
  char* buf;
  int   nbytes;
  int   blockno;
};

void* readAndHandleBlock (void* args_voidstar) {
  // TODO
  // Synchronously:
  //   (1) call disk_scheduleRead to request the block
  //   (2) wait read for it to complete
  //   (3) call handleRead
  struct readAndHandleBlockArgs* argrhb = args_voidstar;
  disk_scheduleRead(argrhb->buf, argrhb->nbytes, argrhb->blockno);
  queue_enqueue(&rhbq, uthread_self());
  blockUntilComplete();
  handleRead(argrhb->buf, argrhb->nbytes, argrhb->blockno);
  free(argrhb);
  return NULL;
}

void run (int numBlocks) {
  uthread_t thread [numBlocks];
  for (int blockno = 0; blockno < numBlocks; blockno++) {
    // TODO
    // call readAndHandleBlock in a way that allows this
    // operation to be synchronous without stalling the CPU
	char buf[4096];
	struct readAndHandleBlockArgs* rhb = malloc (sizeof (*rhb));
	rhb->buf = malloc (sizeof(buf));
	rhb->nbytes = (sizeof (buf));
	rhb->blockno = blockno;
	thread[blockno] = uthread_create(readAndHandleBlock, rhb);

	
  }
  for (int i=0; i<numBlocks; i++)
    uthread_join (thread [i], 0);
}

int main (int argc, char** argv) {
  static const char* usage = "usage: aRead numBlocks";
  int numBlocks = 0;
  queue_init(&rhbq);
  if (argc == 2)
    numBlocks = strtol (argv [1], NULL, 10);
  if (argc != 2 || (numBlocks == 0 && errno == EINVAL)) {
    printf ("%s\n", usage);
    return EXIT_FAILURE;
  }
  
  uthread_init (1);
  disk_start   (interruptServiceRoutine);
  
  run (numBlocks);
  
  printf ("%d\n", sum);
}