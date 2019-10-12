#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5

void *PrintHello(void *threadid)
{
    int tid = *((int*)threadid);
    cout << "Hello w3cschool! 线程ID，" << tid << endl;
    pthread_exit(NULL);
}

int main()
{
    //定义线程的id变量，多个变量使用数组
    pthread_t threads[NUM_THREADS];
    int indexes[NUM_THREADS];
    int rc;
    int i;
    for(int i=0; i<NUM_THREADS; ++i)
    {
        cout << "main() : 创建线程，" << i << endl;
        indexes[i] = i;
        rc = pthread_create(&threads[i], NULL, PrintHello, (void*)&(indexes[i]));
        if(rc){
            cout << "Error:无法创建线程，" << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
