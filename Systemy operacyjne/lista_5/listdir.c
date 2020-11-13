#include "csapp.h"

#define DIRBUFSZ 256

static void print_mode(mode_t m) {
    char t;

    if (S_ISDIR(m))
        t = 'd';
    else if (S_ISCHR(m))
        t = 'c';
    else if (S_ISBLK(m))
        t = 'b';
    else if (S_ISREG(m))
        t = '-';
    else if (S_ISFIFO(m))
        t = 'f';
    else if (S_ISLNK(m))
        t = 'l';
    else if (S_ISSOCK(m))
        t = 's';
    else
        t = '?';

    char ur = (m & S_IRUSR) ? 'r' : '-';
    char uw = (m & S_IWUSR) ? 'w' : '-';
    char ux = (m & S_IXUSR) ? 'x' : '-';
    char gr = (m & S_IRGRP) ? 'r' : '-';
    char gw = (m & S_IWGRP) ? 'w' : '-';
    char gx = (m & S_IXGRP) ? 'x' : '-';
    char or = (m & S_IROTH) ? 'r' : '-';
    char ow = (m & S_IWOTH) ? 'w' : '-';
    char ox = (m & S_IXOTH) ? 'x' : '-';

    /* TODO: Fix code to report set-uid/set-gid/sticky bit as 'ls' does. */
    // 'setuid' bit can be identified easily when there is 's' in place of
    // 'x' of the executable bit. The 's' implies that te executable bit is
    // set, otherwise it would be set to 'S'. 'setgid' bit is similar to 
    // 'setuid' bit, but in this case 's' is present in group sector (there
    // are three sectors: owner, group, other, before them there is one bit
    // saying if it is regular file). The 'sticky' bit is meant to forbid
    // modifying files in that directory by users who are not owners. It is
    // identifiable by a 't' in the place of 'x', also 'T' applies that the
    // executable bit is not present.

    // If m has set 'setuid'/'setgid'/'sticky', check if m is executable,
    // then set the bit accordingly, otherwise permissions are not changed.
    ux = (m & S_ISUID) ? ((m & S_IXUSR) ? 's' : 'S') : ux;
    gx = (m & S_ISGID) ? ((m & S_IXGRP) ? 's' : 'S') : gx;
    ox = (m & S_ISVTX) ? ((m & S_IXOTH) ? 't' : 'T') : ox;

    printf("%c%c%c%c%c%c%c%c%c%c", t, ur, uw, ux, gr, gw, gx, or, ow, ox);
}

static void print_uid(uid_t uid) {
    struct passwd *pw = getpwuid(uid);
    if (pw)
        printf(" %10s", pw->pw_name);
    else
        printf(" %10d", uid);
}

static void print_gid(gid_t gid) {
    struct group *gr = getgrgid(gid);
    if (gr)
        printf(" %10s", gr->gr_name);
    else
        printf(" %10d", gid);
}

static void file_info(int dirfd, const char *name) {
    struct stat sb[1];

    /* TODO: Read file metadata. */
    // int fstatat(int dirfd, const char *pathname, 
    //             struct stat *statbuf, int flags);
    // AT_SYMLINK_NOFOLLOW flag means that if pathname (name) is a symbolic
    // link, it should not be dereferened.
    fstatat(dirfd, name, sb, AT_SYMLINK_NOFOLLOW);

    print_mode(sb->st_mode);
    printf("%4ld", sb->st_nlink);
    print_uid(sb->st_uid);
    print_gid(sb->st_gid);
    
    /* TODO: For devices: print major/minor pair; for other files: size. */
    // Device ID consists of two parts: major ID (class of the device) and
    // minor ID (specific instance of a device in that class).
    // unsigned int major(dev_t dev);
    // unsigned int minor(dev_t dev);
    // S_ISCHR returns non-zero if the file is a character special file
    // (a device like a terminal) and S_ISBLK returns non-zero if the file
    // is a block special file (a device like a disk).
    if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode))
        printf("%2u, %2u", major(sb->st_rdev), minor(sb->st_rdev));
    else
        printf("%6lu", (size_t)sb->st_size);

    char *now = ctime(&sb->st_mtime);
    now[strlen(now) - 1] = '\0';
    printf("%26s", now);

    printf("    %s", name);

    if (S_ISLNK(sb->st_mode)) {
    /* TODO: Read where symlink points to and print '-> destination' string. */
        // const size_t bufsize = 255;
        char path[DIRBUFSZ];

        // readlinkat(2) places the contents of the symbolic link name in the
        // buffer buf, which has size bufsize, but it does not append null byte
        // to buf, thus we have to do it.
        const ssize_t len = readlinkat(dirfd, name, path, DIRBUFSZ);
        path[len] = '\0';
        printf(" -> %s", path);
    }

    putchar('\n');
}

int main(int argc, char *argv[]) {
    if (!argv[1])
        argv[1] = ".";

    int dirfd = Open(argv[1], O_RDONLY | O_DIRECTORY, 0);
    char buf[DIRBUFSZ];
    int n;

    while ((n = Getdents(dirfd, (void *)buf, DIRBUFSZ))) {
        struct linux_dirent *d;
        /* TODO: Iterate over directory entries and call file_info on them. */
        // int getdents(unsigned int fd, struct linux_dirent *dirp
        //              unsigned int count);
        // getdents(2) returns directory entries, it reads several linux_dirent
        // structures from the directory reffered to by the open file descriptor
        // fd into the buffer pointed to by dirp. The argument count specifies
        // the size of that buffer.
        const void* end = buf + n;
        void* it = buf;
        while (it < end) {
            d = (struct linux_dirent*)it;
            file_info(dirfd, d->d_name);
            it += d->d_reclen; // d_reclen is length of this linux_dirent
        }
    }

    Close(dirfd);
    return EXIT_SUCCESS;
}
