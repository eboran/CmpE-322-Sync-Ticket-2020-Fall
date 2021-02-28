#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <fstream>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <map>
#include <iostream>

#include "teller.h"
#include "client.h"
#include "common.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Needs config and output files.\n");
        return -1;
    }

    // it is for reading config file.
    string config_file_name = argv[1];
    config_file.open(config_file_name);

    // it is for writing output file.
    string output_file_name = argv[2];
    myoutputfile.open(output_file_name);

    // initization function called.
    init();

    // Creating teller threads.
    pthread_t teller_threads[TELLER_COUNT];
    for (int i = 0; i < TELLER_COUNT; i++)
    {
        pthread_create(&teller_threads[0], NULL, teller_runner,
                       (void *)&teller_data_array[i]);
        this_thread::sleep_for(chrono::milliseconds((10)));
    }

    // Creating client threads.
    pthread_t client_threads[client_count];
    for (int i = 1; i <= client_count; i++)
    {
        pthread_create(&client_threads[i], NULL, client_runner,
                       (void *)&client_data_array[i]);
    }

    // Waiting client threads from main thread.
    // When all clients are done, the project is succesfully completed.
    // So, it is not necessary to wait teller threads.
    for (int i = 1; i <= client_count; i++)
    {
        pthread_join(client_threads[i], NULL);
    }

    sem_close(sem);
    sem_unlink(name);

    pthread_mutex_lock(&print_mutex);
    myoutputfile << "All clients received service." << endl;
    pthread_mutex_unlock(&print_mutex);
    return 0;
}
