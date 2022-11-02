#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#define RECORD_SIZE 100

// Global variable
char* buf;

int main(int argc, char *argv[]) {
    struct stat st;
    int size;
    
    // Open a file
    int fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);  

    // Retrieves size of input file
    if(fstat(fd, &st) == -1)
        perror("Couldn't get file size\n");

    size = st.st_size;

    // Map the input file into a buffer
    buf = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

    // Each record can be accessed in 100 byte strides: index*100. 
    // Each record's first 4 bytes can be accessed by reading range: index*100 to (index*100)+4

    // Loops through each character in the binary file
    /*
    for(int i = 0; i< st.st_size; i++){
        printf("%c", buf[i]);
    }
    */

    return 0;
}