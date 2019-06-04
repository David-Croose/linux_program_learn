#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

void sigroutine(int signo) {
    switch (signo) {
    case SIGALRM:
        printf("Catch a signal -- SIGALRM \n");
        signal(SIGALRM, sigroutine);
        break;

    case SIGVTALRM:
        printf("Catch a signal -- SIGVTALRM \n");
        signal(SIGVTALRM, sigroutine);
        break;
    }
}

int main() {
    struct itimerval value, value2, ovalue;

    printf("process id is %d\n", getpid());

    signal(SIGALRM, sigroutine);
    value.it_value.tv_sec     = 1;
    value.it_value.tv_usec    = 0;
    value.it_interval.tv_sec  = 1;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, &ovalue);

    signal(SIGVTALRM, sigroutine);
    value2.it_value.tv_sec     = 0;
    value2.it_value.tv_usec    = 500000;
    value2.it_interval.tv_sec  = 0;
    value2.it_interval.tv_usec = 500000;
    setitimer(ITIMER_VIRTUAL, &value2, &ovalue);

    while (1) {
    }
}
