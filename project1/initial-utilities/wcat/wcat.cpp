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
    char *buffer = new char[4096];
    int ret;

    if (argc == 1) {
        fileDescriptor = STDIN_FILENO; // Read from stdin if no file is provided.
        /*while ((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
            write(STDIN_FILENO, buffer, ret);
        }
        if (ret == -1) {
            std::cerr << "wcat: cannot reading stdin" << std::endl;
            delete[] buffer;
            exit(1);
        }*/
    }
    
    // make sure we keeping reading things, now we need to get things write out.
    //std::cout << "Number of arguments: " << argc << std::endl;
    for (int i = 1; i < argc; i++) {
        fileDescriptor = open(argv[i], O_RDONLY);
        if (fileDescriptor == -1) {
            const char *error_msg = "wcat: cannot open file\n";
            //std::cerr << "wcat: cannot open file" << std::endl;
            write(STDERR_FILENO, error_msg, strlen(error_msg));
            delete[] buffer; 
            exit(1);
        }
        while ((ret = read(fileDescriptor, buffer, sizeof(buffer) )) > 0) {
            write(STDOUT_FILENO, buffer, ret);
        }

        if (ret == -1) {
            std::cerr << "Error from reading files" << std::endl;
            delete[] buffer;
            exit(-1);
        }
        close(fileDescriptor);
    }
    delete[] buffer; // Free the buffer memory
    return 0;
}
