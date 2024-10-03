#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    printf ("Connecting to hello world servers...\n");
    void *context = zmq_ctx_new ();
    
    // Create two requesters, one for each server
    void *requester1 = zmq_socket (context, ZMQ_REQ);
    void *requester2 = zmq_socket (context, ZMQ_REQ);
    
    // Connect to both servers
    zmq_connect (requester1, "tcp://localhost:5555");
    zmq_connect (requester2, "tcp://localhost:5556");
    
    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer [10];
        
        // Send to first server
        printf ("Sending Hello to server 1 (5555) - request %d...\n", request_nbr);
        zmq_send (requester1, "Hello", 5, 0);
        zmq_recv (requester1, buffer, 10, 0);
        printf ("Received from server 1: World %d\n", request_nbr);
        
        // Send to second server
        printf ("Sending Hello to server 2 (5556) - request %d...\n", request_nbr);
        zmq_send (requester2, "Hello", 5, 0);
        zmq_recv (requester2, buffer, 10, 0);
        printf ("Received from server 2: World %d\n", request_nbr);
    }
    
    zmq_close (requester1);
    zmq_close (requester2);
    zmq_ctx_destroy (context);
    return 0;
}