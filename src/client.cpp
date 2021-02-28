
#include <iostream>
#include <pthread.h>
#include <chrono>
#include <thread>
#include "common.h"
#include "teller.h"
#include "utils.h"

using namespace std;

int choose_teller()
/*
* It starts to search an available teller from starting A. 
* It chooses the first seat available.
*/
{
    int i = 0;
    for (; i < TELLER_COUNT; i++)
    {
        if (teller_empty[i] == 0)
        {
            teller_empty[i]++;
            return i;
        }
    }
}

void *client_runner(void *param)
/*
* Client Thread Runner Function.
* It uses sem which has initial value 3. It means it only accepts 3 clients to enter 
* the critical section. Since we have 3 teller, we know that there is always an 
* available teller to chose. First, the client chose the teller. Second, it pass its  
* data to the chosen teller buffer. Send signal the chosen teller to say 
* I passed my data. After that, it waits to be handled its task by teller. 
* When teller is done, it sends the data to the client. Client wake up post sem to 
* say I am exiting from the critical section and then, the client thread exits.
*/
{

    struct Client *client_data;
    client_data = (struct Client *)param;
    
    this_thread::sleep_for(chrono::milliseconds((client_data->arrival_time)));
    // starts of the critical section of the client.
    if (sem_wait(sem) < 0)
    {
        perror("sem_wait(3) failed on child");
    }

    pthread_mutex_lock(&teller_empty_mutex);
    int teller_id = choose_teller();
    pthread_mutex_unlock(&teller_empty_mutex);

    // The Client passes its data. 
    teller_buffer[teller_id] = client_data;

    // The Client say to the chosen teller that I passed the data and handle my request please.
    sem_post(full[teller_id]);

    // Waits the request to be handled. 
    sem_wait(empty[teller_id]);

    // Decrease the semaphore value while exiting the critical section.
    if (sem_post(sem) < 0)
    {
        perror("sem_post(3) error on child");
    }
    // end of critical section of client

    pthread_exit(NULL);
}
