#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    // Check if port number is provided
    if (argc != 2) {
        printf("Usage: ./server <port>\n");
        return 1;
    }

    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    
    // Create the connection string with the provided port
    char connection[30];
    sprintf(connection, "tcp://*:%s", argv[1]);
    
    int rc = zmq_bind (responder, connection);
    assert (rc == 0);
    
    printf("Server starting on port %s...\n", argv[1]);
    
    while (1) {
        char buffer [10];
        zmq_recv (responder, buffer, 10, 0);
        printf ("Received Hello on port %s\n", argv[1]);
        sleep (1);
        zmq_send (responder, "World", 5, 0);
    }
    return 0;
}