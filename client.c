#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>


struct sockaddr_in *destination;
uv_tcp_t *client_socket;
uv_connect_t* connection;
uv_loop_t *loop;

bool ISCONNECTED = false;

void on_connect(uv_connect_t* req, int status)
{
    if (status == 0) {
        printf("Connected successfully to host server\n");
        ISCONNECTED = true;
    }
    else {
        printf("couldnt connect, status= %s(%i)\n",uv_err_name(status),status);
        ISCONNECTED = false;
    }
}


void auto_connect(uv_timer_t* handle) // every second called regardless
{
    if (ISCONNECTED) {
        //uv_timer_stop(handle);
        return;
    }


    printf("auto_connect\n");

    //re-init tcp needed before new connection attempt
    free(client_socket);
    client_socket = malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop,client_socket);

    uv_tcp_connect(connection, client_socket, (const struct sockaddr*)destination, on_connect);
}

int main(int argc, char* argv[])
{
    loop = uv_default_loop();
    uv_loop_init(loop);


    //client_socket = malloc(sizeof(uv_tcp_t));
    // uv_tcp_init(loop, client_socket);

    connection = malloc(sizeof(uv_connect_t));

    destination = malloc(sizeof(struct sockaddr_in));
    uv_ip4_addr("127.0.0.1", 1415, destination);

    uv_timer_t *timer = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop,timer);
    uv_timer_start(timer,auto_connect,1000,1000);


    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    free(client_socket);
    free(connection);
    free(destination);
    return 0;
}
