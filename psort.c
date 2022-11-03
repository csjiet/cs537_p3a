#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define RECORD_SIZE 100

// Functions
// This function compares which character bytes are larger in size
// Returns negative number if record 1 is <= than record 2
// Returns positive number if record 1 is > record 2
int compare(int indexOfRecord1, int indexOfRecord2){

    return -1;
}

int stride(int index){
    return index*100;
}


void merge(char* buf, int bufSize, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    char* returnBuf;
    strncpy(returnBuf, buf, bufSize);

    // Create temp array for each division
    int Lstride[n1], Rstride[n2];

    // Copy data to temp arrays
    for(i = 0; i< n1; i++){
        Lstride[i] = stride(l+i);
    }

    for(j=0; j< n2; j++){
        Rstride[j] = stride(m+ 1 + j);

    }

    i = 0; 
    j = 0;
    k = l;

    while(i < n1 && j < n2){
        if(compare(Lstride[i], Rstride[j]) < 0){
            returnBuf[stride(k)] = buf[Lstride[i]];
        }

    }

    // Remember to assign buf pointer with returnBuf
    // and munmap() returnBuf


}

void mergeSort(char* buf, int bufSize, int l, int r){

    if(l < r){
        int m = l + (r-l) /2;

        mergeSort(buf, bufSize, l, m);
        mergeSort(buf, bufSize, m+1, r);

        merge(buf, bufSize, l, m , r);

    }

}

int main(int argc, char *argv[]) {
    struct stat st;
    int size;
    
    // Open a file
    int fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);  

    // Retrieves size of input file
    if(fstat(fd, &st) == -1)
        perror("Couldn't get file size\n");

    int bufSize = st.st_size;

    // Map the input file into a buffer
    char* buf = mmap(NULL, bufSize, PROT_READ, MAP_PRIVATE, fd, 0);

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