#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>

sem_t mutex;

void* thread(void* arg)
{
    //wait
    for (int i = 0; i < 1000000; i++){
        
        double x = (double)rand()/RAND_MAX*2.0-1.0;   
        double y = (double)rand()/RAND_MAX*2.0-1.0;   
        double xAndY = x*x + y*y;
        double distance = sqrt(xAndY); 
        if (distance <= 1){
            sem_wait(&mutex);
         
       //critical section
            int *p = (int*)(arg);
            *p=*p+1;
      
            //signal
            sem_post(&mutex);
        }
    }
    return 0;
}

int main(void)
{
    srand(time(NULL));
    int * num_points = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    sem_init(&mutex, 0, 1);
    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, NULL, thread, (void*) num_points);
    pthread_create(&t2, NULL, thread, (void*) num_points);
    pthread_create(&t3, NULL, thread, (void*) num_points);
    pthread_create(&t4, NULL, thread, (void*) num_points);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);

    sem_destroy(&mutex);
    
    double final = 4*(*num_points/((double)4000000));
    printf("%f\n",final);
    return 0;
}
