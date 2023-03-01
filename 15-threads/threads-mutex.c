#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <memory.h>

struct thread_arg_t {
  pthread_mutex_t * mutex;
  double * doubles;
  size_t thread_no;
  size_t N; size_t k;
};

void* thread_func(void * arg) {
  struct thread_arg_t * thread_arg = (struct thread_arg_t * ) arg;

  pthread_mutex_lock(thread_arg->mutex);
  size_t thread_no = thread_arg->thread_no;
  size_t N = thread_arg->N;
  double * doubles = thread_arg->doubles;
  size_t k = thread_arg->k;

  for (size_t i = 0 ;i < N; ++i) {
    doubles[(thread_no - 1 + k) % k] += .99;
    doubles[thread_no] += 1;
    doubles[(thread_no + 1) % k] += 1.01;
  }
  pthread_mutex_unlock(thread_arg->mutex);
  return NULL;
}


int main(int argc, char * argv[]) {
  size_t N = strtol(argv[1], NULL, 10);
  size_t k = strtol(argv[2], NULL, 10);

  double * doubles = malloc(sizeof(double) * k);
  memset(doubles, 0, k*sizeof(double));
  pthread_t * threads = malloc(sizeof(pthread_t) * k);

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  struct thread_arg_t *thread_args = malloc(sizeof(struct thread_arg_t) * k);

  for (size_t i = 0; i < k; ++i) {
    thread_args[i].mutex = &mutex;
    thread_args[i].doubles = doubles;
    thread_args[i].thread_no = i;
    thread_args[i].k = k;
    thread_args[i].N = N;
    pthread_create(&threads[i], NULL, thread_func, &thread_args[i]);
  }

  for (size_t i = 0; i < k; ++i ){
    pthread_join(threads[i], NULL);
  }
  for (size_t i = 0; i < k; ++i) {
    printf("%.10g\n", doubles[i]);
  }

  fflush(stdout);

  free(threads); free(doubles); free(thread_args);
  return 0;
}