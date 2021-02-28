#ifndef COMMON_H
#define COMMON_H
#include <semaphore.h>
#include <map>
#include <string>
#define TELLER_COUNT 3

// burada declere edip kullanacağım yerde define ederim.

using namespace std;

// Data Structure for Client
struct Client
{
    string client_name;
    int arrival_time, service_time, seat_number;
};
// Data Structure for Teller
struct Teller
{
    string name;
    int id;
};

// It holds number of clients.
extern int client_count;
// Buffer of Tellers. It is used for passing client info to tellers.
extern Client *teller_buffer[TELLER_COUNT];
// It holds whether a teller is empty or not. It is used when choosing
extern int teller_empty[TELLER_COUNT];

extern int seat_count; // seat count in the theatre
extern char *name;     // semaphore name
extern int VALUE;      // max semaphore count in the critical section
extern sem_t *sem;     // semaphore used for clients when they enter their critical section.

extern int *seat_availability; // It is an array holding seats availability. If the seat is available, it is 1, otherwise 0.

// It is used to pass their data to client threads while creating them.
extern Client *client_data_array;
// It is used to pass their data to teller threads while creating them.
extern Teller *teller_data_array;

// It is used for only one teller to choose the seat.
extern pthread_mutex_t seat_reservation_mutex;
// It is used for printing file.
extern pthread_mutex_t print_mutex;
// It is used when changing the current situation of tellers on teller_empty array.
extern pthread_mutex_t teller_empty_mutex;

// It is used for syncronization. Clients waits it. Tellers send signals when
// they complete the task of a client.
extern sem_t *empty[TELLER_COUNT];

// It is used for syncronization. Tellers wait clients to pass their data to the buffer.
// When clients pass the request data, they send signal to the tellers.
extern sem_t *full[TELLER_COUNT];

// It is a map for theatre name to seat count.
extern map<string, int> theatre_to_seat_count;
// It is a map for teller id to name. Ex: {0: "A"}
extern map<int, string> teller_id_to_name;

extern ifstream config_file;  // input config file
extern ofstream myoutputfile; // output file

#endif
