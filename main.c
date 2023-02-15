// gcc -g -s -O0 -Wl,-z,relro,-z,now -fstack-protector-all main.c -o xcode

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void xor(char* data, int data_len, unsigned char key) {
    for (int i = 0; i < data_len; i++) {
        data[i] ^= key;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <raw shellcode>\n", argv[0]);
        exit(1);
    }

    char* fp = argv[1];
    srand(time(NULL));
    unsigned char key = rand() & 0xff;

    FILE* f = fopen(fp, "rb");
    if (f == NULL) {
        printf("[-] Error while opening the file!\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* shellcode = malloc(file_size);
    fread(shellcode, 1, file_size, f);
    fclose(f);

    xor(shellcode, file_size, key);
    printf("unsigned char key = 0x%02x;\n", key);
    printf("unsigned char shellcode[] = { ");
    for (int i = 0; i < file_size; i++) {
        printf("0x%02x", (unsigned char) shellcode[i]);
        if (i < file_size - 1) {
            printf(", ");
        }
    }
    printf(" };\n");

    free(shellcode);
    return 0;
}

