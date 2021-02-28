#ifndef CLIENT_H
#define CLIENT_H

// funtion to choose available teller.
int choose_teller();
// client thread runner
void *client_runner(void *param);
#endif
