#include "csapp.h"

void Pthread_mutex_init(pthread_mutex_t *mutex,
                        const pthread_mutexattr_t *mutexattr) {
  if (pthread_mutex_init(mutex, mutexattr))
    unix_error("Pthread_mutex_init");
}

void Pthread_mutex_destroy(pthread_mutex_t *mutex) {
  if (pthread_mutex_destroy(mutex))
    unix_error("Pthread_mutex_destroy");
}

void Pthread_mutex_lock(pthread_mutex_t *mutex) {
  if (pthread_mutex_lock(mutex))
    unix_error("Pthread_mutex_lock");
}

void Pthread_mutex_unlock(pthread_mutex_t *mutex) {
  if (pthread_mutex_unlock(mutex))
    unix_error("Pthread_mutex_unlock");
}
