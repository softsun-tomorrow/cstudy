#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5

struct thread_data{
    int thread_id;
    const char* message;
};

void *PrintHello(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    cout << "Thread ID: " << my_data->thread_id;
    cout << "  Message: " << my_data->message << endl;
    pthread_exit(NULL);
}

int main()
{
    //定义线程的id变量，多个变量使用数组
    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];
    int rc;
    int i;
    for(int i=0; i<NUM_THREADS; ++i)
    {
        cout << "main() : 创建线程，" << i << endl;
        td[i].thread_id = i;
        td[i].message = "This is message ";
        rc = pthread_create(&threads[i], NULL, PrintHello, (void*)&(td[i]));
        if(rc){
            cout << "Error:无法创建线程，" << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
