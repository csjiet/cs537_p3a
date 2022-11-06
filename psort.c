#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#define RECORD_SIZE 100
#define MAP_HUGE_1GB (30 << MAP_HUGE_SHIFT)

// Global variable
char* buf;
//char* refBuf;
int bufSize;
int section = 0;

pthread_mutex_t lock;
// Functions
// This function compares which character bytes are larger in size
// Returns negative number if record 1 is <= than record 2
// Returns positive number if record 1 is > record 2
int compare(int record1, int record2){

    //return record1 - record2;
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


void merge(int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    //char L[n1 * RECORD_SIZE], R[n2 * RECORD_SIZE];

    char* L = malloc((n1* RECORD_SIZE) * sizeof(char));
    char* R = malloc((n2* RECORD_SIZE) * sizeof(char));

    // Copy temp array for each division
    memcpy(L, (buf + stride(l)), n1 * RECORD_SIZE);
    memcpy(R, (buf + stride(m + 1)), n2 * RECORD_SIZE);

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

    free(L);
    free(R);

}

void mergeSort(int l, int r){

    if(l < r){
        int m = l + (r-l) /2;

        mergeSort(l, m);
        mergeSort(m+1, r);

        merge(l, m , r);

    }

}

void* thread_mergeSort(void* args) {
    pthread_mutex_lock(&lock);
    int nprocs = get_nprocs();
    int thread_section = section++;
    int elements = (bufSize / RECORD_SIZE);
    int low = thread_section * (elements / nprocs);
    int high = (thread_section + 1) * (elements / nprocs) - 1;

    int mid = low + (high - low) / 2;
    if (low < high) {
        mergeSort(low, mid);
        mergeSort(mid + 1, high);
        merge(low, mid, high);
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]) {
    struct stat st;
    
    // Open a file
    int fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);  
    
    // Retrieves size of input file
    if (fstat(fd, &st) == -1) {
        fprintf(stderr, "An error has occurred\n");
        exit(0);
    }
    bufSize = st.st_size;

    if (bufSize == 0) {
        fprintf(stderr, "An error has occurred\n");
        exit(0);
    }
    // Map the input file into a buffer
    buf = mmap(NULL, bufSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_NORESERVE, fd, 0);
    
    int np = get_nprocs();
    pthread_t threads[np];
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Mutex init failed\n");
        exit(1);
    }
    for (int i = 0; i < np; i++) {
        pthread_create(&threads[i], NULL, thread_mergeSort, (void*)NULL);
    }
    for (int i = 0; i < np; i++) {
        pthread_join(threads[i], NULL);
    }
    // Calls merge sort
    int numberOfRecords = bufSize/ RECORD_SIZE;
    mergeSort(0, numberOfRecords - 1);

    // Write to an output file
    int file, ret;
    file = creat(argv[2], S_IWUSR | S_IRUSR);
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