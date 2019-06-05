#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void *thread_print(void *p) {
    int res = 0x45;

    printf("this is sub thread\n");
    sleep(5);
    pthread_exit((void *)res);
}

int main(void) {
    pthread_t thd;
    int res;
    void *sta;

    printf("main thread creating...\n");
    if ((res = pthread_create(&thd, NULL, thread_print, NULL)) != 0) {
        printf("error in main thread, res=0x%08x\n", res);
        return -1;
    }

    // the main thread will wait for the termination of the sub thread in blocking
    if ((res = pthread_join(thd, &sta)) != 0) {
        printf("error in pthread_join, res=0x%08x\n", res);
        return -2;
    }
    printf("the sub thread has returned, res=0x%08x\n", (int)sta);

    printf("main thread exit\n");
    pthread_exit(NULL);
    return 0;
}
