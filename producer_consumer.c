#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;    // chi so ghi
    int out;   // chi so doc
    int total; // tong
} SharedData;

void *producer(void *arg) {
    SharedData *data = (SharedData *)arg;
    srand(time(NULL));

    while (1) {
        // Tao so ngau nhien 10 den 20
        int num = rand() % 11 + 10;

        // Doi cho den khi co khong gian trong buffer
        while (data->count == BUFFER_SIZE);

        // Them so vao buffet
        data->buffer[data->in] = num;
        data->in = (data->in + 1) % BUFFER_SIZE;
        data->count++;

        printf("Producer produced: %d\n", num);

        // Kiem tra tong
        data->total += num;
        if (data->total > 100) {
            break;
        }
        usleep(100000);
    }
    return NULL;
}

void *consumer(void *arg) {
    SharedData *data = (SharedData *)arg;

    while (1) {
        // Doi cho den khi co du lieu trong buffer
        while (data->count == 0);

        // Lay so tu buffer
        int num = data->buffer[data->out];
        data->out = (data->out + 1) % BUFFER_SIZE;
        data->count--;

        printf("Consumer consumed: %d\n", num);

        // Kiem tra tong
        data->total += num;
        if (data->total > 100) {
            break;
        }

        usleep(100000);
    }
    return NULL;
}

int main() {
    int shm_id = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    SharedData *data = (SharedData *)shmat(shm_id, NULL, 0);
    if (data == (SharedData *)-1) {
        perror("shmat");
        exit(1);
    }

    //Khoi tao du lieu chia se
    data->count = 0;
    data->in = 0;
    data->out = 0;
    data->total = 0;

    pthread_t prod_thread, cons_thread;

    // Tao tien trinh Producer vàa Consumer
    pthread_create(&prod_thread, NULL, producer, (void *)data);
    pthread_create(&cons_thread, NULL, consumer, (void *)data);

    // Cho cac tien trinh hoan thanh
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    printf("Final total: %d\n", data->total);

    // Giai phong bo nho chia se
    shmdt(data);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
