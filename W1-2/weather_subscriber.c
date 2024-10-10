#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Connecting to weather update servers...\n");
    void *context = zmq_ctx_new();

    // Criar dois sockets de subscription
    void *subscriber1 = zmq_socket(context, ZMQ_SUB);
    void *subscriber2 = zmq_socket(context, ZMQ_SUB);

    // Conectar aos publicadores
    zmq_connect(subscriber1, "tcp://localhost:5555"); // Publicador de PT
    zmq_connect(subscriber2, "tcp://localhost:5556");

    // Inscrever-se em todas as mensagens (ou use um filtro específico)
    zmq_setsockopt(subscriber1, ZMQ_SUBSCRIBE, "", 0);
    zmq_setsockopt(subscriber2, ZMQ_SUBSCRIBE, "", 0);

    while (1) {
        zmq_pollitem_t items[] = {
            { subscriber1, 0, ZMQ_POLLIN, 0 },
            { subscriber2, 0, ZMQ_POLLIN, 0 }
        };

        // Poll para verificar se há mensagens
        zmq_poll(items, 2, -1);

        // Verificar se há uma mensagem do primeiro publicador
        if (items[0].revents & ZMQ_POLLIN) {
            char buffer[100];
            zmq_recv(subscriber1, buffer, 100, 0);
            printf("Received from PT Publisher: %s\n", buffer);
        }

        // Verificar se há uma mensagem do segundo publicador
        if (items[1].revents & ZMQ_POLLIN) {
            char buffer[100];
            zmq_recv(subscriber2, buffer, 100, 0);
            printf("Received from US Publisher: %s\n", buffer);
        }
    }

    zmq_close(subscriber1);
    zmq_close(subscriber2);
    zmq_ctx_destroy(context);
    return 0;
}