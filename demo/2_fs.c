#include <stdio.h>

int main() {
    FILE *file = fopen("./1_hello.c", "rb");
    if (!file) {
        printf("failed to open file");
        return 1;
    }
    while (!feof(file)) {
        char c = fgetc(file);
        if (c != EOF) {
            putchar(c);
        }
    }
    fclose(file);
    return 0;
}

// emcc 2_fs.c -o hello.html --preload-file ./1_hello.c