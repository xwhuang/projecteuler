#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

static unsigned long int num = 123456;
static int key[10][2];

static inline int set_key(unsigned long int i)
{
    int j = 0;
    for (j=0; j < 10; j++) {
        key[j][0] = key[j][1] = 0;
    }

    unsigned long int value = i;
    int mod = 0;
    while (value) {
        mod = value%10;
        if (key[mod][0] != 0) {
            return -1;
        }
        key[mod][0] = 1;
        key[mod][1] = 1;
        value /= 10;
    }

    return 0;
}

void halt_print(int signal)
{
    if (signal == SIGINT) {
        printf("num = %lu\n", num);
    }
}

int main(int argc, char *argv[])
{
    int times = 0;
    if (argc != 2) {
        printf("%s <times>\n", argv[0]);
        exit(1);
    }
    times = atoi(argv[1]);

    signal(SIGINT, halt_print);

    struct timeval ts, te;
    //125874
    /*+-+-+-+-+-+-+-+-+-+-+
      |0|1|2|3|4|5|6|7|8|9|
      +-+-+-+-+-+-+-+-+-+-+
      |0|1|1|0|0|1|0|1|1|0|
      +-+-+-+-+-+-+-+-+-+-+
     */
    gettimeofday(&ts, NULL);
    do {
        if (set_key(num) != 0) {
            num++;
            continue;
        }

        unsigned long int k = 2;
        unsigned long int value = 0;
        unsigned long int mod;
        unsigned long int x;
        for (k = 2; k < times; k++) {
            value = num*k;
            while (value) {
                mod = value%10;
                x = key[mod][0] + key[mod][1];
                if (x == 2) {
                    key[mod][1] = 0;
                } else {
                    break;
                }
                value /= 10;
            }
            int j = 0;
            for (j = 0; j < 10;j++) {
                key[j][1] = key[j][0];
            }
            if (value != 0) {
                break;
            }
        }
        if (k == times) {
            gettimeofday(&te, NULL);
            printf("%lu(%lu)\n", num,
                   (te.tv_sec*1000000000+te.tv_usec*1000) - 
                   (ts.tv_sec*1000000000+ts.tv_usec*1000)
                );            
            return 0;
        }
        num++;
    } while (1);

    return 0;
}
