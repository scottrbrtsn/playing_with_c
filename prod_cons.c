#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int c = 0;
bool consumer_ready = false;
bool done = false;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *unused) {
  int i;
  while (!consumer_ready) {
    // no-op
    printf("consumer not ready\n");
  }
  printf("start producing\n");
  for (i = 0; i < 1000; i++) {
    /* produce something */
    /* append it to a list */
    pthread_mutex_lock(&m);
    c++;
    printf("cP: %d\n", c);
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&m);
  }
  printf("fin: %d\n", c);
  done = true;
  pthread_cond_signal(&cond_var);
  return NULL;
}

void *consumer(void *unused) {
  printf("cons: %d\n", c);
  consumer_ready = true;
  printf("not done\n");
  printf("start consuming\n");
  while (c == 0) {
    printf("waiting for producer\n");
  }
  while (c > 0) {
    /* remove something from list */
    if (!done) {
      pthread_mutex_lock(&m);
      pthread_cond_wait(&cond_var, &m);
      c--;
      printf("cC: %d\n", c);
      pthread_mutex_unlock(&m);
    } else {
      c--;
      printf("cC: %d\n", c);
    }
  }
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t prod, con;

  pthread_create(&con, NULL, consumer, NULL);
  pthread_create(&prod, NULL, producer, NULL);

  void *unused;
  pthread_join(prod, &unused);
  pthread_join(con, &unused);

  printf("Net: %d\n", c);
}
