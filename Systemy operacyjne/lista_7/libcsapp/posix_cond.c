#include "csapp.h"

void Pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr) {
  if (pthread_cond_init(cond, cond_attr))
    unix_error("Pthread_cond_init");
}

void Pthread_cond_destroy(pthread_cond_t *cond) {
  if (pthread_cond_destroy(cond))
    unix_error("Pthread_cond_destroy");
}

void Pthread_cond_signal(pthread_cond_t *cond) {
  if (pthread_cond_signal(cond))
    unix_error("Pthread_cond_signal");
}

void Pthread_cond_broadcast(pthread_cond_t *cond) {
  if (pthread_cond_broadcast(cond))
    unix_error("Pthread_cond_broadcast");
}

void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
  if (pthread_cond_wait(cond, mutex))
    unix_error("Pthread_cond_wait");
}
