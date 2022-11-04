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

// This function compares which character bytes are larger in size
// Returns negative number if record 1 is <= than record 2
// Returns positive number if record 1 is > record 2
int compareRecords(char* record1, char* record2){
    int key1 = *(int*) record1;
    int key2 = *(int*) record2;
    
    if(key1 <= key2){
        return -1;
    }else{
        return 1;
    }
}

// Each record can be accessed in 100 byte strides: index*100. 
// Each record's first 4 bytes can be accessed by reading range: index*100 to (index*100)+4
int stride(int index){
    return index*100;
}


void merge(char* buf, int bufSize, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    char L[n1 * RECORD_SIZE], R[n2 * RECORD_SIZE];

    // Copy temp array for each division
   
    // memcpy(&L, &buf[stride(l)], n1 * RECORD_SIZE);
    // memcpy(&R, &buf[stride(m + 1)], n2 * RECORD_SIZE);
    memcpy(&L, (buf + stride(l)), n1 * RECORD_SIZE);
    memcpy(&R, (buf + stride(m + 1)), n2 * RECORD_SIZE);

    
    i = 0;
    j = 0;
    k = l;

    
    while(i < n1 && j < n2){
        if(compareRecords((L + stride(i)), (R + stride(j))) < 0){
            memcpy((buf + stride(k)), &L[stride(i)], RECORD_SIZE);
            i++;
        }else{
            memcpy((buf + stride(k)), &R[stride(j)], RECORD_SIZE);
            j++;
        }
        k++;
    }

    // Copy remaining elements
    while(i < n1){
        memcpy((buf + stride(k)), &L[stride(i)], RECORD_SIZE);
        i++;
        k++;
    }

    while(j < n2){
        memcpy((buf + stride(k)), &R[stride(j)], RECORD_SIZE);
        j++;
        k++;
    }


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
    int bufSize;
    
    // Open a file
    int fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);  

    // Retrieves size of input file
    if(fstat(fd, &st) == -1)
        perror("Couldn't get file size\n");

    bufSize = st.st_size;

    // Map the input file into a buffer
    char* buf = mmap(NULL, bufSize, PROT_WRITE, MAP_PRIVATE, fd, 0);

    // Loops through each character in the binary file
    /*
    for(int i = 0; i< st.st_size; i++){
        printf("%c", buf[i]);
    }
    */

    // Calls merge sort
    int numberOfRecords = bufSize/ RECORD_SIZE;
    mergeSort(buf, bufSize, 0, numberOfRecords - 1);

    // Write to an output file
    int file, ret;
    file = creat("output.bin", S_IWUSR | S_IRUSR);
    if(file < -1){
            perror("creat() error\n");
            exit(1);
    }
   
    ret = write(file, buf, bufSize);
    if(ret < -1){
            perror("write() error\n");
            exit(1);
    }

    fsync(file);
    close(file);
    return 0;
}