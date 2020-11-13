#include "csapp.h"

typedef struct {
    int child_fd;
    int parent_fd;
} sockpair_t;

static sockpair_t MakeSocketPair(void) {
    int sv[2];
    Socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    return (sockpair_t){.child_fd = sv[0], .parent_fd = sv[1]};
}

static bool MaybeReadNum(int fd, int *num_p) {
    return Read(fd, num_p, sizeof(int)) == sizeof(int);
}

static int ReadNum(int fd) {
    int num;
    if (Read(fd, &num, sizeof(int)) < sizeof(int))
        app_error("ReadNum error");
    return num;
}

static void WriteNum(int fd, int num) {
    if (Write(fd, &num, sizeof(int)) < sizeof(int))
        app_error("WriteNum error");
}

static void SendElem(int parent_fd, int child_fd, int nelem) {
    WriteNum(child_fd, nelem);
    for (int i = 0; i < nelem; i++)
        WriteNum(child_fd, ReadNum(parent_fd));
}

static void Merge(int left_fd, int right_fd, int parent_fd) {
    bool has_left = true;
    bool has_right = true;
    int left = ReadNum(left_fd);
    int right = ReadNum(right_fd);

    do {
        if ((has_left && has_right) ? left < right : has_left) {
            WriteNum(parent_fd, left);
            has_left = MaybeReadNum(left_fd, &left);
        } else {
            WriteNum(parent_fd, right);
            has_right = MaybeReadNum(right_fd, &right);
        }
    } while (has_left || has_right);
}

static void Sort(int parent_fd) {
    int nelem = ReadNum(parent_fd);

    if (nelem < 2) {
        WriteNum(parent_fd, ReadNum(parent_fd));
        Close(parent_fd);
        return;
    }

    sockpair_t left = MakeSocketPair();
    /* TODO: Spawn left child. */
    if (Fork() == 0) {
        // Close file descriptors so we do not write
        // to them when we have more than 2 elements
        // and we sort them [elements].
        Close(left.parent_fd);
        Close(parent_fd);

        Sort(left.child_fd);
        exit(0);
    }
    // After sorting we close fd of the left child.
    Close(left.child_fd);

    sockpair_t right = MakeSocketPair();
    /* TODO: Spawn right child. */
    if (Fork() == 0) {
        // Similarly to left child, we close fd, but
        // in this case we need to close also left,
        // as it might be open.
        Close(left.parent_fd);
        Close(right.parent_fd);
        Close(parent_fd);

        Sort(right.child_fd);
        exit(0);
    }
    // After sorting we close fd of the right child.
    Close(right.child_fd);
    
    /* TODO: Send elements to children and merge returned values afterwards. */
    const int32_t left_half  = nelem / 2;
    const int32_t right_half = nelem - left_half;
    
    // We send elements from parent to child and
    // the number of elements in each half. Then
    // we merge results (from childs' fds) and
    // save them in parent's fd.
    SendElem(parent_fd, left.parent_fd,  left_half);
    SendElem(parent_fd, right.parent_fd, right_half);
    Merge(left.parent_fd, right.parent_fd, parent_fd);

    // At the end we need to close every fd we used,
    // so there are none left open.
    Close(parent_fd);
    Close(left.parent_fd);
    Close(right.parent_fd);

    /* Wait for both children. */
    Wait(NULL);
    Wait(NULL);
}

static int GetNumber(void) {
    char buf[20];
    if (fgets(buf, sizeof(buf), stdin) == NULL)
        app_error("GetNumber error");
    return strtol(buf, NULL, 10);
}

int main(void) {
    sockpair_t sort = MakeSocketPair();

    if (Fork()) {
        /* parent */
        int nelem = GetNumber();
        if (nelem < 2)
            app_error("Number of sorted elements must be at least 2!\n");
        Close(sort.child_fd);

        /* Write unsorted numbers to mergesort */
        WriteNum(sort.parent_fd, nelem);
        for (int i = 0; i < nelem; i++)
            WriteNum(sort.parent_fd, GetNumber());

        /* Read sorted numbers from mergesort */
        int prev = INT_MIN;
        for (int i = 0; i < nelem; i++) {
            int elem = ReadNum(sort.parent_fd);
            fprintf(stderr, "%d\n", elem);
            assert(prev <= elem);
            prev = elem;
        }
        Close(sort.parent_fd);

        Wait(NULL);
    } else {
        /* child */
        Close(sort.parent_fd);
        Sort(sort.child_fd);
    }

    return 0;
}
