#include "csapp.h"

void Sem_init(sem_t *sem, int pshared, unsigned value) {
  if (sem_init(sem, pshared, value))
    unix_error("sem_init");
}

void Sem_destroy(sem_t *sem) {
  if (sem_destroy(sem))
    unix_error("sem_destroy");
}

void Sem_wait(sem_t *sem) {
  if (sem_wait(sem))
    unix_error("sem_wait");
}

void Sem_getvalue(sem_t *sem, int *sval) {
  if (sem_getvalue(sem, sval))
    unix_error("sem_getvalue");
}

void Sem_post(sem_t *sem) {
  if (sem_post(sem))
    unix_error("sem_post");
}
