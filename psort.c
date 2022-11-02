#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    unsigned char buffer[100];
    FILE *fp;
    fp = fopen("output.bin", "rb");
    fread(buffer, sizeof(buffer), 1, fp);
    for (int i = 0; i < 100; i++) {
        printf("%x ", buffer[i]);
    }
    return 0;
}