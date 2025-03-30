#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_ITERATIONS 5

//mutex lock
pthread_mutex_t mutex;

//shared data
int shared_data = 0;

void* writer_thread(void* arg) {

    //write to shared data
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        //lock before writing
        pthread_mutex_lock(&mutex);

        //update shared data with a new value
        shared_data = i * 10;

        printf("Writer Thread: Wrote %d to shared data\n", shared_data);

        //unlock after writing
        pthread_mutex_unlock(&mutex);

        //simulate work
        sleep(1);
    }
    pthread_exit(NULL);
}

void* reader_thread(void* arg) {

    //read from shared data
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        //lock before reading
        pthread_mutex_lock(&mutex);

        printf("Reader Thread: Read %d from shared data\n", shared_data);

        //unlock after reading
        pthread_mutex_unlock(&mutex);

        //simulate processing time
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    //initialize mutex
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }


    //create threads for writer and reader
    if(pthread_create(&thread1, NULL, writer_thread, NULL) != 0) {
        perror("Thread creation failed");
        return 1;
    }
    

    if(pthread_create(&thread2, NULL, reader_thread, NULL) != 0) {
        perror("Thread creation failed");
        return 1;
    }


    //wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    //destroy mutex after use
    pthread_mutex_destroy(&mutex);

    printf("Main Thread: All threads have finished execution. Shared data: %d\n", shared_data);
    return 0;
}
