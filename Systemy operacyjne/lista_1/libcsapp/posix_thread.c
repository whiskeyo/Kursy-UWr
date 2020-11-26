/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

#include "csapp.h"

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                    void *(*routine)(void *), void *argp) {
  int rc = pthread_create(tidp, attrp, routine, argp);
  if (rc)
    posix_error(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid) {
  int rc = pthread_cancel(tid);
  if (rc)
    posix_error(rc, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void **thread_return) {
  int rc = pthread_join(tid, thread_return);
  if (rc)
    posix_error(rc, "Pthread_join error");
}

void Pthread_detach(pthread_t tid) {
  int rc = pthread_detach(tid);
  if (rc)
    posix_error(rc, "Pthread_detach error");
}
