#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
struct stat st;
int size;
stat("output.bin", &st);
size = st.st_size;
unsigned char *buffer = malloc(100*sizeof(char));
FILE *fp;
fp = fopen("output.bin", "rb");
fread(buffer, sizeof(buffer), 1, fp);
for (int i = 0; i < 100; i++) {
    printf("%x ", buffer[i]);
}
    return 0;
}