
#include "common.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <fcntl.h>

int *seat_availability;

void init()
{

    myoutputfile << "Welcome to the Sync-Ticket!" << endl;

    // open a semaphore with a initial value 3. It means only 3 client can enter
    // the critical section.
    sem = sem_open(name, O_CREAT, 0666, VALUE);

    if (sem == SEM_FAILED)
    {
        perror("sem_open(3) failed");
    }

    // Capacity of The Theatres
    theatre_to_seat_count["OdaTiyatrosu"] = 60;
    theatre_to_seat_count["UskudarStudyoSahne"] = 80;
    theatre_to_seat_count["KucukSahne"] = 200;

    // Mapping from teller id to name
    teller_id_to_name[0] = "A";
    teller_id_to_name[1] = "B";
    teller_id_to_name[2] = "C";

    // Creating an array for teller data. It is used to pass teller when creating threads.
    teller_data_array = new Teller[TELLER_COUNT];

    // Inıtilization of teller data structures.
    for (int i = 0; i < TELLER_COUNT; i++)
    {
        // Gives names and id to the tellers.
        teller_data_array[i].name = teller_id_to_name[i];
        teller_data_array[i].id = i;

        // Make tellers empty initially.
        teller_empty[i] = 0;

        // Creating sem for empty and full
        string s_f = "full_" + to_string(i);
        char name_f[s_f.size() + 1];
        strcpy(name_f, s_f.c_str());

        // Every teller has a unique semaphore.
        full[i] = sem_open(name_f, O_CREAT, 0666, 0);
        if (full[i] == SEM_FAILED)
        {
            perror("full sem_open(3) failed");
        }

        string s_e = "empty_" + to_string(i);
        char name_e[s_e.size() + 1];
        strcpy(name_e, s_e.c_str());

        empty[i] = sem_open(name_e, O_CREAT, 0666, 0);
        if (empty[i] == SEM_FAILED)
        {
            perror("empty sem_open(3) failed");
        }
    }

    string tiyatro_tipi;
    // reading threatre type.
    config_file >> tiyatro_tipi;
    // getting seat count from threatre map.
    seat_count = theatre_to_seat_count[tiyatro_tipi];

    // init of seat availability
    // make available all seats by setting 1.
    seat_availability = new int[seat_count + 1];
    for (int i = 1; i <= seat_count + 1; i++)
    {
        seat_availability[i] = 1;
    }

    pthread_mutex_init(&seat_reservation_mutex, NULL);

    // Getting client count in similation.
    string line;
    config_file >> line;
    client_count = stoi(line);

    // We create an array which is used for passing data to the client threads.
    // It has a length of client + 1. It is just to write more readable code.
    // We use only the indexes from 1 to client_count.
    client_data_array = new Client[client_count + 1];
    for (int i = 1; i <= client_count; i++)
    {
        // Read a line
        config_file >> line;
        // Get client name
        string client_name = line.substr(0, line.find(","));
        // Get arrival time
        line = line.substr(line.find(",") + 1);
        int arrival_time = stoi(line.substr(0, line.find(",")));
        // Get service time
        line = line.substr(line.find(",") + 1);
        int service_time = stoi(line.substr(0, line.find(",")));
        // Get seat number
        line = line.substr(line.find(",") + 1);
        int seat_number = stoi(line);

        // Set client data to the client_data_array
        // It is used for passing client threads.
        client_data_array[i].client_name = client_name;
        client_data_array[i].arrival_time = arrival_time;
        client_data_array[i].service_time = service_time;
        client_data_array[i].seat_number = seat_number;
    }
    // print_mutex is used when writing file.
    pthread_mutex_init(&print_mutex, NULL);
}
