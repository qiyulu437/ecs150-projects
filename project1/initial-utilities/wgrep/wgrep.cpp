#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

bool readLine(int fileDescriptor, char* &buffer, int &bufIndex, int &bufSize) {
    char temp;
    bufIndex = 0;

    while (read(fileDescriptor, &temp, 1) == 1) {
        if (bufIndex >= bufSize - 1) {
            bufSize *= 2; 
            buffer = (char *) realloc(buffer, bufSize);
        }
        buffer[bufIndex++] = temp;
        if (temp == '\n') break;
    } 

    buffer[bufIndex] = '\0'; // Null-terminate the line

    return (bufIndex > 0);
}

bool searchTermInLine(const char *line, const char *searchTerm) {
    return strstr(line, searchTerm) != nullptr;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        const char *msg = "wgrep: searchterm [file ...]\n";
        write(STDERR_FILENO, msg, strlen(msg));
        exit(1);
    }

    const char *searchTerm = argv[1];
    int bufSize = 4096;
    char *buffer = (char *) malloc(bufSize);
    int bufIndex = 0;

    if (argc == 2) {
        while (readLine(STDIN_FILENO, buffer, bufIndex, bufSize)) {
            if (searchTermInLine(buffer, searchTerm)) {
                write(STDOUT_FILENO, buffer, bufIndex);
            }
        }
    }
    else {
        for (int i = 2; i < argc; i++) {
            int fileDescriptor = open(argv[i], O_RDONLY);
            if (fileDescriptor == -1) {
                const char *error_msg = "wgrep: cannot open file\n";
                write(STDERR_FILENO, error_msg, strlen(error_msg));
                free(buffer);
                exit(1);
            }

            while (readLine(fileDescriptor, buffer, bufIndex, bufSize)) {
                if (searchTermInLine(buffer, searchTerm)) {
                    write(STDOUT_FILENO, buffer, bufIndex);
                }
            }
            close(fileDescriptor);
        }
    }

    free(buffer);
    return 0;
}
