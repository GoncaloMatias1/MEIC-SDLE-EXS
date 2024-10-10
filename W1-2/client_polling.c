#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Connecting to hello world servers...\n");
    void *context = zmq_ctx_new();

    // Criar dois sockets de request, um para cada servidor
    void *requester1 = zmq_socket(context, ZMQ_REQ);
    void *requester2 = zmq_socket(context, ZMQ_REQ);

    // Conectar aos servidores
    zmq_connect(requester1, "tcp://localhost:5555");
    zmq_connect(requester2, "tcp://localhost:5556");

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer[10];

        // Enviar pedido ao primeiro servidor
        printf("Sending Hello to server 1 (5555) - request %d...\n", request_nbr);
        zmq_send(requester1, "Hello", 5, 0);

        // Usar zmq_poll para esperar a resposta
        zmq_pollitem_t items[] = {
            { requester1, 0, ZMQ_POLLIN, 0 },
            { requester2, 0, ZMQ_POLLIN, 0 }
        };

        // Poll com um timeout (por exemplo, 1000 ms)
        zmq_poll(items, 2, 1000);

        // Verificar se temos uma resposta do servidor 1
        if (items[0].revents & ZMQ_POLLIN) {
            zmq_recv(requester1, buffer, 10, 0);
            printf("Received from server 1: World %d\n", request_nbr);
        }

        // Enviar pedido ao segundo servidor
        printf("Sending Hello to server 2 (5556) - request %d...\n", request_nbr);
        zmq_send(requester2, "Hello", 5, 0);

        // Poll novamente para o segundo servidor
        zmq_poll(items, 2, 1000);
        
        // Verificar se temos uma resposta do servidor 2
        if (items[1].revents & ZMQ_POLLIN) {
            zmq_recv(requester2, buffer, 10, 0);
            printf("Received from server 2: World %d\n", request_nbr);
        }
    }

    zmq_close(requester1);
    zmq_close(requester2);
    zmq_ctx_destroy(context);
    return 0;
}