#include <stdio.h>

int
main(int argc, char *argv[]) {
    int ln, n;
    int c;
    int i, j;
    FILE *fp;

    ln = 0;
    n = 10;
    fp = stdin;

    //"All output should go to the standard output"
    //requiring putc over putchar implies handling streams properly
    if(argc > 3) {
        fprintf(stderr, "Usage: head [-n] [file]\n");
        return 1;
    }
    for(i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            n = 0;
            for(j = 1; argv[i][j] != '\0'; j++) {
                n = n * 10 + (argv[i][j] - '0');
            }
        }
        else {
            fp = fopen(argv[i], "r");
            if(fp == NULL) {
                perror(argv[i]);
                return 1;
            }
        }
    }
    while(ln < n && (c = getc(fp)) != EOF) {
        putc(c, stdout);
        if(c == '\n') {
            ln++;
        }
    }
    if(fp != stdin) {
        fclose(fp);
    }
    return 0;
}
