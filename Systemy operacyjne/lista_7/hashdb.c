#include "csapp.h"

#define ENT_LENGTH 48

#define DB_MODE (S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)
#define DB_PROT (PROT_READ | PROT_WRITE)

typedef enum { DB_QUERY, DB_INSERT } op_t;
typedef char entry_t[ENT_LENGTH];

typedef struct db {
  entry_t *entry;
  ssize_t size;
  char *name;
} db_t;

static bool db_action(db_t *db, const char *str, op_t op) {
  /* Do not insert empty strings. Database doesn't store them. */
  if (*str == '\0')
    return op == DB_INSERT;
  size_t len = strnlen(str, ENT_LENGTH);
  int chain_len = max((db->size >> 14) - 1, 3);
  uint32_t chain = jenkins_hash(str, len, HASHINIT);
  for (int c = 0; c < chain_len; c++) {
    size_t idx = (chain + c) % db->size;
    if (!db->entry[idx][0] && op == DB_INSERT) {
      strncpy(db->entry[idx], str, ENT_LENGTH);
      return true;
    }
    if (strncmp(str, db->entry[idx], ENT_LENGTH) == 0)
      return true;
  }
  return false;
}

#define db_query(db, key) db_action(db, key, DB_QUERY)
#define db_maybe_insert(db, key) db_action(db, key, DB_INSERT)

/* Open (`size` = 0) or create (`size` > 0) database from `name` file. */
static void db_open(db_t *db, const char *name, size_t size) {
  assert(powerof2(size));

  int fd = Open(name, O_RDWR | O_CREAT | (size ? O_EXCL : 0), DB_MODE);

  if (size == 0) {
    struct stat sb;
    Fstat(fd, &sb);
    size = sb.st_size / sizeof(entry_t);
    if (size == 0)
      size = 1;
  }

  /* TODO: Setup DB structure, set file size and map the file into memory.
           Inform OS that we're going to read DB in random order. */
  Close(fd);
}

/* Remove DB memory mapping and release associated memory. */
static void db_close(db_t *db) {
  Munmap(db->entry, db->size * sizeof(entry_t));
  free(db->name);
}

/* Attempt to increase size of database. */
static bool db_rehash(db_t *db, size_t new_size) {
  assert(powerof2(new_size));

  /* Create new database. */
  db_t new[1];

  char *name = alloca(strlen(db->name) + sizeof(".reshash") + 1);
  strcpy(name, db->name);
  strcat(name, ".rehash");
  db_open(new, name, new_size);

  /* Copy everything from old database to new database. */
  /* TODO: Inform OS that we're going to read DB sequentially. */
  for (size_t i = 0; i < db->size; i++) {
    if (!db_maybe_insert(new, db->entry[i])) {
      /* Oops... rehashing failed. Need to increase db size and try again. */
      /* TODO: Remove new database, since rehashing failed. */
      return false;
    }
  }

  /* TODO Replace old database with new one, remove old database. */
  db->entry = new->entry;
  db->size = new->size;
  free(new->name);
  return true;
}

/* Insert key into database and possibly increase DB size. */
static void db_insert(db_t *db, const char *str) {
  while (!db_maybe_insert(db, str)) {
    size_t size = db->size;
    do {
      size *= 2;
      fprintf(stderr, "db_rehash: %ld -> %ld\n", db->size, size);
    } while (!db_rehash(db, size));
  }
}

/* Read a key from buffer and perform `mode` operation of the database. */
static char *consume_line(char *buf, db_t *db, op_t mode) {
  /* Terminate string at new line character. */
  char *end = strchr(buf, '\n');
  if (end) {
    if (buf < end && end[-1] == '\r')
      end[-1] = '\0';
    *end++ = '\0';
  }

  /* Skip if line is empty. */
  if (*buf) {
    if (mode == DB_INSERT)
      db_insert(db, buf);
    else if (mode == DB_QUERY)
      puts(db_query(db, buf) ? "YES" : "NO");
  }

  /* Return pointer to next line or NULL. */
  return end;
}

static void doit(const char *path, op_t mode) {
  db_t db;
  db_open(&db, path, 0);

  /* If input file is a terminal device then use standard reading technique. */
  /* TODO: Use fstat instead to handle pipes correctly. */
  if (isatty(STDIN_FILENO)) {
    char buf[ENT_LENGTH + 1];
    while (fgets(buf, ENT_LENGTH + 1, stdin))
      consume_line(buf, &db, mode);
  } else {
    /* TODO: Map stdin into memory, and read it line by line. */
  }

  db_close(&db);
}

static noreturn void usage(int argc, char **argv) {
  app_error("Usage: %s [-i|-q] [FILE]", argv[0]);
}

int main(int argc, char **argv) {
  op_t mode = -1;
  int opt;

  while ((opt = getopt(argc, argv, "iq")) != -1) {
    if (opt == 'i')
      mode = DB_INSERT;
    else if (opt == 'q')
      mode = DB_QUERY;
    else
      usage(argc, argv);
  }

  if (optind >= argc || mode == -1)
    usage(argc, argv);

  doit(argv[optind], mode);

  return 0;
}
