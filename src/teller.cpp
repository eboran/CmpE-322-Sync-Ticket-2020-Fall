

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <chrono>
#include <thread>

#include "common.h"
#include "teller.h"
#include "utils.h"

using namespace std;

int client_count;

Client *teller_buffer[TELLER_COUNT];
int teller_empty[TELLER_COUNT];
int *client_buffer;

int seat_count; // seat count in the similation
char *name = "new_semaphore5";
int VALUE = 3; // max semaphore count in the critical section
sem_t *sem;

Client *client_data_array;
Teller *teller_data_array;

pthread_mutex_t seat_reservation_mutex;
pthread_mutex_t print_mutex;
pthread_mutex_t teller_empty_mutex;

sem_t *empty[TELLER_COUNT];
sem_t *full[TELLER_COUNT];

map<string, int> theatre_to_seat_count;
map<int, string> teller_id_to_name;

ifstream config_file; // input file is open
ofstream myoutputfile;

void *teller_runner(void *param)
/*
* Teller Thread Runner Function. 
* It waits the semaphore named full[teller id]. When it takes the signal,
* it knows there is a client that passed its data to the buffer. 
* It consumes buffer, it means that it make reservation if there is a seat. 
* Then it sends the signal with semaphore named empty to say you are done 
* to the client. 
* It continues this logic in a while(1) loop forever. 
*/
{
    Teller *teller_param;
    teller_param = (Teller *)param;
    /*init part begin*/

    pthread_mutex_lock(&print_mutex);
    myoutputfile << "TELLER " << teller_param->name << " has arrived." << endl;
    pthread_mutex_unlock(&print_mutex);
    /*init part end*/

    while (1)
    {
        // it waits until a client pass its data to the teller buffer.
        sem_wait(full[teller_param->id]);

        // make a reservation to the client.
        pthread_mutex_lock(&seat_reservation_mutex);
        string reservation_number = make_a_reservation(teller_buffer[teller_param->id]->seat_number);
        pthread_mutex_unlock(&seat_reservation_mutex);

        // The Teller sleeps, it means it is handling the client job.
        this_thread::sleep_for(chrono::milliseconds((teller_buffer[teller_param->id]->service_time)));

        pthread_mutex_lock(&print_mutex);

        myoutputfile << teller_buffer[teller_param->id]->client_name
                     << " requests seat " << teller_buffer[teller_param->id]->seat_number
                     << ", reserves " << reservation_number
                     << ". Signed by Teller " << teller_param->name << "." << endl;

        pthread_mutex_unlock(&print_mutex);

        pthread_mutex_lock(&teller_empty_mutex);
        teller_empty[teller_param->id]--; // make empty the teller to accept new clients
        pthread_mutex_unlock(&teller_empty_mutex);

        // send signal to say client that you are done.
        sem_post(empty[teller_param->id]);
    }
    pthread_exit(NULL);
}

string make_a_reservation(int seat_choice)
/* 
* It is a function to make a reservation of a client. 
* if seat choice of the client is available, it reserves the seat for the client
* otherwise, it reserves the seat with smallest number. 
*/
{

    int res;
    bool done = false;
    if (seat_availability[seat_choice] != 1)
    {
        for (int i = 1; i <= seat_count; i++)
        {
            if (seat_availability[i] == 1)
            {
                // it choses the available seat with smallest number.
                res = i;
                done = true;
                break;
            }
        }
    }
    else
    {
        // the chosen seat by client is available.
        res = seat_choice;
        done = true;
    }
    if (done)
    {
        seat_availability[res] = 0;
        return "seat " + to_string(res);
    }
    else
    {
        // It could not find any seat available to the client.
        return "None";
    }
}
