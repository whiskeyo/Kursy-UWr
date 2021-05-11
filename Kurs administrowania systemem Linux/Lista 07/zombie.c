#include <unistd.h>

int main() {
    if (fork() != 0)
        sleep(60);
    
    return 0;
}