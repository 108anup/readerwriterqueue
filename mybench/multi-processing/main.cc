#include <thread>
#include "../../readerwriterqueue.h"
#include <chrono>
#include <errno.h>
#include <unistd.h>
#include <csignal>
#include "util.h"

#define handle_error_en(en, msg)                                    \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace std;

struct myt{
    long long x;
};

const int MAX_ELEMENTS = 2000;
const long long NUM_OPS = 1e9;
typedef moodycamel::ReaderWriterQueue<myt> shared_queue_t;
shared_queue_t *q;

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

shared_queue_t* create_shm(const char* name, size_t data_size){

    LOG_MSG("Attempting to create shared_queue in shm region: %s", name);

    shared_queue_t* q = (shared_queue_t *) malloc(sizeof(shared_queue_t));
}
