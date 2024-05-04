#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

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

int main() {
    char buffer[BUFFER_SIZE] = "";
    char key;
    int buffer_index = 0;

    printf("Appuyer sur 'q' pour quitter.\n");

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

