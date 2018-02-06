#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void myFuncTerm();

int main(void)
{
    if (signal(SIGTERM, myFuncTerm) == SIG_ERR) {
        printf("SIGTERM error.\n");
    }
    printf("READY\n");
    while (1) {
        sleep(2);
    }
}

void myFuncTerm()
{
    FILE *f;
    int retVal;
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
    f = fopen("testing.txt", "a");
    if (f == NULL) {
        printf("File open error!\n");
        return;
    }
    retVal = fprintf(f, "SIGTERM! - %s\n", c_time_string);
    if (retVal < 0) {
        printf("File write error!\n");
        return;
    }
    fflush(f);
    fclose(f);
}

