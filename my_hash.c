#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char **argv){



    int fd;
    struct stat sb;
    off_t offset, pa_offset;
    int message_len;
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        handle_error("open");
    if (fstat(fd, &sb) == -1) // To obtain file size 
        handle_error("fstat");    

    pa_offset = 0 & ~(sysconf(_SC_PAGE_SIZE) - 1);
    message_len = sb.st_size;
    char* message;
    message = mmap(NULL, message_len - pa_offset, PROT_READ, MAP_PRIVATE, fd, pa_offset);
    if (message == MAP_FAILED)
            handle_error("mmap");


    unsigned int a = 0b11000000110000001100000011000000;
    unsigned int b = 0b10101010101010101010101010101010;
    unsigned int c = 0b01010101010101010101010101010101;
    unsigned int out = 0b01000000011011100111011011001000;
    unsigned int d = 0x0;
    unsigned int e = 0x0;
    unsigned int f = 0x0;
    unsigned int g = 0x0;
    int i;
    int n;
    int m;
    char t1;
    char t2;
    for(i=0;i<message_len;i+=2){
        a = 0b11000000110000001100000011000000;
        t1=message[i];
        t2=i==message_len-1?0x20:message[i+1];
        d=0x0;
        memcpy(&d, &t1, 1);
        d=d>>4;
        d=d<<26;
        e=0x0;
        memcpy(&e, &t1, 1);
        e=e<<28;
        e=e>>10;
        f=0x0;
        memcpy(&f, &t2, 1);
        f=f>>4;
        f=f<<10;
        g=0x0;
        memcpy(&g, &t2, 1);
        g=g<<28;
        g=g>>26;
        a=a|d|e|f|g;
        n=(a>>1) % 32;
        m=(a>>3) % 32;
        a = (a << (32 - n) | (a >> n));
        a = (a << (32 - m) | (a >> m));
        a = (a << 16 | (a >> 16));
        if(a>0b10000000000000000000000000000000)a=a^b;
        else a=a^c;
        out = out^a;
        out = (out << 16 | (out >> 16));
        n=(out>>3) % 32;
        out = (out << (32-n) | (out >> n));
    }
    n = message_len+7;
    out = (out << 16 | (out >> 16));
    out = (out << (32-n) | (out >> n));
    n=(out>>3) % 32;
    out = (out << (32-n) | (out >> n));

    unsigned char sha[4];
    char buf[4*2];
    memset(buf, 0x0, 4*2);
    memset(sha, 0x0, 4);
    memcpy(sha, &out, 4);
    for (i=0; i < 4; i++) {
        sprintf((char*)&(buf[i*2]), "%02x", sha[i]);
    }
    printf("%s\n", buf);
}