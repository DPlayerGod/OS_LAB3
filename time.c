#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    pid_t pid;

    // Lay thoi gian truoc khi thuc thi lenh
    gettimeofday(&start, NULL);

    // Tao tien trinh con
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Tien trinh con thuc hien voi execl()
        execl("/bin/sh", "sh", "-c", argv[1], NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Tien trinh cha doi tien trinh con ket thuc
        wait(NULL);
        
        // Lay thoi gian sau khi thuc thi lenh
        gettimeofday(&end, NULL);

        // Tinh toan thoi gian thuc thi
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("Thời gian thực thi: %f\n", elapsed);
    }

    return 0;
}