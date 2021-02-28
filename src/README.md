
# CmpE-322-Sync-Ticket - Project 2 

# Emre Boran - 2019700162

# Running Simulation

* Run the project with these commands.

* $make
* $./simulation.o input_file_path output_file_path

## Inıt

* In this project, we have implemented Sync Ticket.
* I have used 3 semaphores to sync threads. These are sem, full[3], empty[3].
* There is two types of thread runner. Client Runner and Teller Runner.
* Counts of clients are given in the config file and 3 tellers named A, B, and C.

## General Logic of the Project

* We know there is 3 teller. 
* We use a semaphore called "sem" to accept only 3 clients in the critical section of client threads. 
* The reason we choose sem with initial value 3 is that teller count is 3. 
* In the critical section, clients pass their data to an available teller's buffer, and they wait for the teller to complete the reservation. 
* After reservation, the client goes out from the critical section by signaling 'sem' semaphore. 
* All clients in the critical section know that there is an available teller. Since there is 3 teller and we only accept 3 clients. For example, only if a client exists when there are 3 clients in the critical section, a new client can enter the critical section. 
* Clients choose their teller and pass their data when they enter their critical section.
* After passing their data, they wait for a reservation. When the reservation is done, the client thread exits. However, teller threads wait for another client to handle its reservation.
* The simulation goes on until all clients are served. 

### full[3] semaphore

* There is a unique full semaphore for every teller. So, there is an array of length 3. Its initial value is 0, which means when a thread calls wait full[thread_id], it needs a signal to continue. If it already has, it goes on without sleeping. 
* The purpose of the full semaphore is to let the teller know that there is a new client that passed its data to the buffer. 
* Since the initial value is 0, tellers wait until their client signals them. 

### empty[3] semaphore

* Its initial value is 0 as a full semaphore. So, the working mechanism is the same. 
* After client passed its data to buffer and signal full[chosen_teller], it calls wait empty[chosen_teller]. It means that the client waits for tellers to reserve a seat. 
* Teller sends signal empty[teller_id] to inform its client about reservation completion.
* When clients receive a signal, they know they are done and they exist.

## Client Side

* First of all, client threads have a critical section that accepts only 3 clients. 
* When a client in the critical section, it knows there should be an available teller. 
* The client chooses the teller.
* Then the client only communicates with the chosen teller with full[chosen_teller] and empty[chosen_teller] semaphore.  
* Client goes to the available teller, passes its data to the buffer, and signal full[chosen_teller]. Signaling means I have passed the data to the chosen teller and let know the chosen teller about it. 
* After that, the client waits for semaphore empty[chosen_teller]. It means that client is waiting for a reservation from chosen teller.
* Then, the client exits.

## Teller Side

* Teller thread runners are in while(true) loop. 
* Every iteration, a teller handles a client's request.
* Teller call wait full[teller_id] semaphore. It means it waits to be filled buffer by a client. 
* When it receives a signal, it means the buffer is full. Teller gets the client data from its buffer.
* If the client's choice is available, the teller reserves it.
* If not, searches from seat 1 to 'seat_count'. Then reserve the first available seat for the client.
* If there is no available seat. The teller reserves 'None'.


## Ending of Simulation

* Main thread waits for only client threads by calling pthread_join.
* Every client exits when they have a signal from the teller about their seat. So, we know clients exit only when tellers serve them. 
* It means it is enough to wait for just clients from the main thread. 



