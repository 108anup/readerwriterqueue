#include "../../readerwriterqueue.h"               // Mine
#include "systemtime.h"
#include "../../tests/common/simplethread.h"

using namespace moodycamel;

typedef unsigned long long counter_t;
volatile int forceNoOptimizeDummy;

int main(){
    const counter_t MAX = 1000 * 1000;
    ReaderWriterQueue<int> q(MAX);
    int element = -1;
    SystemTime start = getSystemTime();
    SimpleThread consumer([&]() {
                              for (counter_t i = 0; i != MAX; ++i) {
                                  q.try_dequeue(element);
                              }
                          });
    SimpleThread producer([&]() {
                              int num = 0;
                              for (counter_t i = 0; i != MAX; ++i) {
                                  q.enqueue(num);
                                  ++num;
                              }
                          });
    producer.join();
    consumer.join();
    double result = getTimeDelta(start) / 1000.0;
    forceNoOptimizeDummy = (int)(q.try_dequeue(element));
    printf("Took %.6f s, i.e. %.4f Mtx/s", result,
           ((double) MAX) / (result * 1e6));
    return 0;
}
