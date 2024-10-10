#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, "tcp://*:5555");

    while (1) {
        // Criar uma string de atualização meteorológica
        char update[100];
        int zipcode = 1000; // Exemplo de código postal
        sprintf(update, "%04d Weather: Sunny %d°C", zipcode, rand() % 30);
        
        // Enviar a atualização
        zmq_send(publisher, update, strlen(update), 0);
        printf("Sent: %s\n", update);

        sleep(1); // Enviar uma atualização a cada segundo
    }

    zmq_close(publisher);
    zmq_ctx_destroy(context);
    return 0;
}