/*######################################
# University of Information Technology
# IT007 Operating System
#
# <Dang Quoc Cuong>, <23520192>
# File: collatz.c
######################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 4096 

int is_positive_integer(const char *str) {
    // Kiem tra neu chuoi la so nguyen duong
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return atoi(str) > 0;
}

void collatz_sequence(int n, char *buffer) {
    char temp[20];
    while (n != 1) {
        sprintf(temp, "%d, ", n);
        strcat(buffer, temp);
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
    }
    sprintf(temp, "%d", n); 
    strcat(buffer, temp);
}

int main(int argc, char *argv[]) {
    if (argc != 2 || !is_positive_integer(argv[1])) {
        fprintf(stderr, "Usage: %s <positive_integer>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    char *shared_memory = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        collatz_sequence(n, shared_memory);
        exit(0);
    } else {
        wait(NULL);
        printf("Collatz sequence for %d: %s\n", n, shared_memory);
        munmap(shared_memory, BUFFER_SIZE);
    }

    return 0;
}
