#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <sys/types.h>
#include <sys/mman.h>
#ifdef LINUX
#include <sys/sysmacros.h>
#include <sys/prctl.h>
#endif
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <pwd.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define _CONCAT(x, y) x##y
#define CONCAT(x, y) _CONCAT(x, y)

#define min(a, b)                                                              \
  ({                                                                           \
    typeof(a) _a = (a);                                                        \
    typeof(b) _b = (b);                                                        \
    _a < _b ? _a : _b;                                                         \
  })

#define max(a, b)                                                              \
  ({                                                                           \
    typeof(a) _a = (a);                                                        \
    typeof(b) _b = (b);                                                        \
    _a > _b ? _a : _b;                                                         \
  })

#ifndef powerof2
#define powerof2(x) (((x) & ((x)-1)) == 0)
#endif

#ifndef __unused
#define __unused __attribute__((unused))
#endif

extern char **environ;

/* Useful constants. */
#define MAXLINE 4096

/* Our own error-handling functions */
noreturn void unix_error(const char *fmt, ...)
  __attribute__((format(printf, 1, 2)));
noreturn void posix_error(int code, const char *fmt, ...)
  __attribute__((format(printf, 2, 3)));
noreturn void app_error(const char *fmt, ...)
  __attribute__((format(printf, 1, 2)));
noreturn void gai_error(int code, const char *fmt, ...)
  __attribute__((format(printf, 2, 3)));

/* Signal safe I/O functions */
void safe_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void safe_error(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

/* Decent hashing function. */
#define HASHINIT 5381

uint32_t jenkins_hash(const void *key, size_t length, uint32_t initval);

/* Memory allocation wrappers */
void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t nmemb, size_t size);

/* Process control wrappers */
pid_t Fork(void);
pid_t Waitpid(pid_t pid, int *iptr, int options);
#define Wait(iptr) Waitpid(-1, iptr, 0)
void Prctl(int option, long arg);

/* Process environment */
char *Getcwd(char *buf, size_t buflen);

/* Signal control wrappers */
void (*Signal(int sig, void (*func)(int)))(int);
void Kill(pid_t pid, int sig);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
void Sigaction(int signum, const struct sigaction *act,
               struct sigaction *oldact);
void Sigsuspend(const sigset_t *mask);

/* Process group control wrappers */
void Setpgid(pid_t pid, pid_t pgid);

/* Stdio wrappers */
char *Fgets(char *ptr, int n, FILE *stream);
void Fputs(const char *ptr, FILE *stream);

/* Unix I/O wrappers */
int Open(const char *pathname, int flags, mode_t mode);
size_t Read(int fd, void *buf, size_t count);
size_t Write(int fd, const void *buf, size_t count);
size_t Writev(int fd, const struct iovec *iov, int iovcnt);
off_t Lseek(int fildes, off_t offset, int whence);
void Close(int fd);
void Ftruncate(int fd, off_t length);
int Dup(int fd);
int Dup2(int oldfd, int newfd);
void Pipe(int fds[2]);
void Socketpair(int domain, int type, int protocol, int sv[2]);
int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout);
int Poll(struct pollfd *fds, nfds_t nfds, int timeout);

/* Directory access (Linux specific) */
struct linux_dirent {
  unsigned long d_ino;     /* Inode number */
  unsigned long d_off;     /* Offset to next linux_dirent */
  unsigned short d_reclen; /* Length of this linux_dirent */
  char d_name[];           /* Filename (null-terminated) */
};

int Getdents(int fd, struct linux_dirent *dirp, unsigned count);

/* Directory operations */
void Rename(const char *oldpath, const char *newpath);
void Unlink(const char *pathname);

/* File metadata access wrapper */
void Fstat(int fd, struct stat *statbuf);
void Fstatat(int dirfd, const char *pathname, struct stat *statbuf, int flags);
size_t Readlink(const char *pathname, char *buf, size_t bufsiz);
size_t Readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz);

/* Memory mapped files & anonymous memory */
void *Mmap(void *addr, size_t length, int prot, int flags, int fd,
           off_t offset);
void Mprotect(void *addr, size_t len, int prot);
void Munmap(void *addr, size_t len);
void Madvise(void *addr, size_t length, int advice);

/* Terminal control */
void Tcsetpgrp(int fd, pid_t pgrp);
pid_t Tcgetpgrp(int fd);
void Tcsetattr(int fd, int action, const struct termios *termios_p);
void Tcgetattr(int fd, struct termios *termios_p);

/* Setjmp & longjmp implementation without sigprocmask */
typedef struct {
  long rbx;
  long rbp;
  long r12;
  long r13;
  long r14;
  long r15;
  void *rsp;
  void *rip;
} Jmpbuf[1];

int Setjmp(Jmpbuf env);
noreturn void Longjmp(Jmpbuf env, int val);

/* Socket interface wrappers. */
typedef struct sockaddr SA;
int Socket(int domain, int type, int protocol);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
void Bind(int sockfd, struct sockaddr *my_addr, int addrlen);
void Listen(int s, int backlog);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

/* Protocol-independent wrappers. */
void Getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res);
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);
int open_clientfd(char *hostname, char *port);
int Open_clientfd(char *hostname, char *port);
int open_listenfd(char *port, int backlog);
int Open_listenfd(char *port, int backlog);

/* POSIX thread control wrappers. */

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                    void *(*routine)(void *), void *argp);
void Pthread_cancel(pthread_t tid);
void Pthread_join(pthread_t tid, void **thread_return);
void Pthread_detach(pthread_t tid);

/* POSIX semaphore wrappers. */
void Sem_init(sem_t *sem, int pshared, unsigned value);
void Sem_destroy(sem_t *sem);
void Sem_wait(sem_t *sem);
void Sem_getvalue(sem_t *sem, int *sval);
void Sem_post(sem_t *sem);

/* POSIX mutex wrappers. */
void Pthread_mutex_init(pthread_mutex_t *mutex,
                        const pthread_mutexattr_t *mutexattr);
void Pthread_mutex_destroy(pthread_mutex_t *mutex);
void Pthread_mutex_lock(pthread_mutex_t *mutex);
void Pthread_mutex_unlock(pthread_mutex_t *mutex);

/* POSIX conditional variable wrappers. */
void Pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
void Pthread_cond_destroy(pthread_cond_t *cond);
void Pthread_cond_signal(pthread_cond_t *cond);
void Pthread_cond_broadcast(pthread_cond_t *cond);
void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/* POSIX reader-writer lock wrappers. */
void Pthread_rwlock_init(pthread_rwlock_t *rwlock,
                         const pthread_rwlockattr_t *rwlockattr);
void Pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
void Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
void Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
void Pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

#endif /* __CSAPP_H__ */
