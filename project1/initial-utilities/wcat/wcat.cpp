#include <iostream>
#include <string>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <fcntl.h>
#include <stdlib.h>


int main (int argc, char *argv[]) {
    int fileDescriptor;

    if (argc == 1) {
        fileDescriptor = STDIN_FILENO; // Read from stdin if no file is provided.
    }

    char *buffer = new char[4096];
    int ret;
    // make sure we keeping reading things, now we need to get things write out.
    for (int i = 1; i < argc; i++) {
        int fileDescriptor = open(argv[i], O_RDONLY);
        if (fileDescriptor == -1) {
            std::cerr << "wcat: could not open" << argv[1] << std::endl;
            exit(1);
        }
        while ((ret = read(fileDescriptor, buffer, sizeof(buffer) )) > 0) {
            write(STDOUT_FILENO, buffer, ret);
        }

        if (ret == -1) {
            std::cerr << "Error from reading files" << std::endl;
            exit(-1);
        }
    }

    if (fileDescriptor != STDIN_FILENO) {
        close(fileDescriptor);
    }
    delete[] buffer; // Free the buffer memory
    // std::cout << "wcat works perfectly!! " << std::endl;
    return 0;
}
