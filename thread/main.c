#include <stdio.h>
#include <pthread.h>

static void *thread_print(void *p) {
    long seq = (long)p;

    printf("this is thread %d\n", seq);
    pthread_exit(NULL);
}

int main(void) {
    pthread_t thd[5];
    long seq;
    int res;

    for (seq = 0; seq < sizeof(thd) / sizeof(thd[0]); seq++) {
        printf("main thread creating thread %d\n", seq);
        if ((res = pthread_create(&thd[seq], NULL, thread_print, (void *)seq)) != 0) {
            printf("error in main thread, when created thread %d, res=0x%08x\n", seq, res);
            return -1;
        }
    }

    printf("main thread exit\n");
    pthread_exit(NULL);
    return 0;
}

