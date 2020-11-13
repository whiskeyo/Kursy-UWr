#include "csapp.h"

void Pthread_rwlock_init(pthread_rwlock_t *rwlock,
                         const pthread_rwlockattr_t *rwlockattr) {
  if (pthread_rwlock_init(rwlock, rwlockattr))
    unix_error("Pthread_rwlock_init");
}

void Pthread_rwlock_destroy(pthread_rwlock_t *rwlock) {
  if (pthread_rwlock_destroy(rwlock))
    unix_error("Pthread_rwlock_destroy");
}

void Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) {
  if (pthread_rwlock_rdlock(rwlock))
    unix_error("Pthread_rwlock_rdlock");
}

void Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) {
  if (pthread_rwlock_wrlock(rwlock))
    unix_error("Pthread_rwlock_wrlock");
}

void Pthread_rwlock_unlock(pthread_rwlock_t *rwlock) {
  if (pthread_rwlock_unlock(rwlock))
    unix_error("Pthread_rwlock_unlock");
}
