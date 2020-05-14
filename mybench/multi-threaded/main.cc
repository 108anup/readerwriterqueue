#include <thread>
#include "../readerwriterqueue.h"
#include <chrono>
#include <errno.h>
#include <unistd.h>
#include <csignal>

#define handle_error_en(en, msg)                                    \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace std;

struct myt{
    long long x;
};

const int MAX_ELEMENTS = 2000;
moodycamel::ReaderWriterQueue<myt> q(MAX_ELEMENTS);
const long long NUM_OPS = 1e9;

void set_affinity(int cpu_num){
    int s, j;
    cpu_set_t cpuset;
    pthread_t thread;

    thread = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET(cpu_num, &cpuset);

    s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0)
        handle_error_en(s, "pthread_setaffinity_np");

    /* Check the actual affinity mask assigned to the thread */
    s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0)
        handle_error_en(s, "pthread_getaffinity_np");

    printf("Set returned by pthread_getaffinity_np() contained:\n");
    for (j = 0; j < CPU_SETSIZE; j++)
        if (CPU_ISSET(j, &cpuset))
            printf("    CPU %d\n", j);
}

void insert(){
    set_affinity(0);
    long long i;
    myt t;
    t.x = 0;
    for(i=0; i<NUM_OPS; i++){
        t.x = i;
        while(q.try_emplace(t) == false);
    }
}

#define TIME 5

int counter = 0;
void handler(int sig){
    printf("%d\n", counter/TIME);
    alarm(TIME);
    counter = 0;
}

volatile int no_optimize;
void fetch(){
    set_affinity(1);
    myt t;
    t.x = 0;
    while(true){
        if(t.x == NUM_OPS-1LL)
            break;
        while(q.try_dequeue(t))
        counter++;
    }
    no_optimize = (int)(q.try_dequeue(t));
}

int main(){

    printf("Starting...\n");
    signal(SIGALRM, handler);
    alarm(1);

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    thread t1(insert);
    thread t2(fetch);

    t1.join();
    t2.join();
    chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
    chrono::duration<long double, nano> d = end - start;
    printf("%.10LF million transfers / s\n", NUM_OPS * 1e3 / d.count());
    printf("Took %10Lf s", d.count() / 1e9);

    return 0;
}
