#if NODEF
gcc -o event event.c -Wall -Wextra -Werror -g
exit 0
#endif
// Copyright (C) 2022 johannst

#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include <sys/time.h>
#include <linux/input-event-codes.h>

struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
};

const char* type(unsigned short t) {
    static char buf[32];
    const char* fmt = "0x%x";
    switch (t) {
#define FMT(TYPE) case TYPE: fmt = #TYPE"(0x%x)"; break
        FMT(EV_SYN);
        FMT(EV_KEY);
        FMT(EV_REL);
        FMT(EV_ABS);
#undef FMT
    }
    snprintf(buf, sizeof(buf), fmt, t);
    return buf;
}

const char* code(unsigned short c) {
    static char buf[32];
    const char* fmt = "0x%x";
    switch (c) {
#define FMT(CODE) case CODE: fmt = #CODE"(0x%x)"; break
        FMT(BTN_LEFT);
        FMT(BTN_RIGHT);
        FMT(BTN_MIDDLE);
        FMT(REL_X);
        FMT(REL_Y);
#undef FMT
    }
    snprintf(buf, sizeof(buf), fmt, c);
    return buf;
}

const char* timefmt(const struct timeval* t) {
    assert(t);
    struct tm* lt = localtime(&t->tv_sec); // Returns pointer to static tm object.
    static char buf[64];
    strftime(buf, sizeof(buf), "%H:%M:%S", lt);
    return buf;
}

int main(int argc, char* argv[]) {
    assert(argc == 2);

    int fd = open(argv[1], O_RDONLY);
    assert(fd != -1);

    struct input_event inp;
    while (1) {
        int ret = read(fd, &inp, sizeof(inp));
        assert(ret == sizeof(inp));
        printf("time: %s type: %s code: %s value: 0x%x\n",
               timefmt(&inp.time), type(inp.type), code(inp.code), inp.value);
    }
}
