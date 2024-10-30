#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t child_pid;

void handle_sigint(int sig) {
    if (child_pid > 0) {
        kill(child_pid, SIGKILL);
        printf("\ncount.sh has stopped\n");
    }
}

int main() {
    // 1. In ra dong chao mung
    printf("Welcome to IT007, I am 23520192!\n");

    // 2. Thiet lap xu ly tin hieu SIGINT (CTRL+C)
    signal(SIGINT, handle_sigint);

    // 3. Tao tien trinh con de thuc thi count.sh
    child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Tien trinh con: Thuc thi count.sh voi tham so 120
        execl("./count.sh", "./count.sh", "120", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Tien trinh cha : Doi tien trinh con ket thuc
        wait(NULL);
    }

    return 0;
}
