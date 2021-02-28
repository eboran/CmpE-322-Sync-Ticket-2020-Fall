#ifndef TELLER_H
#define TELLER_H
using namespace std;

// Teller thread runner
void *teller_runner(void *param);
// Making a reservation for a client
string make_a_reservation(int seat_choice);
#endif
