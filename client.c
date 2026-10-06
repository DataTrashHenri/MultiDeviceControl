#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>



int counter = 0;

void on_connect(uv_connect_t* req, int status)
{
    if (status == 0)
        printf("Connected successfully to host server\n");
    else
        printf("couldnt connect, status= %s(%i)\n",uv_err_name(status),status);
}

void idle_activity(uv_idle_t* handle)
{
    printf("idling...\n");
    counter++;

    if (counter == 5)
        uv_idle_stop(handle);


}

void timer_activity(uv_timer_t* handle)
{
    printf("aaah timer\n");
}

int main(int argc, char* argv[])
{
    uv_loop_t *loop = uv_default_loop();
    uv_loop_init(loop);


    uv_tcp_t* socket = malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, socket);

    uv_connect_t* connect = malloc(sizeof(uv_connect_t));

    struct sockaddr_in *dest = malloc(sizeof(struct sockaddr_in));
    uv_ip4_addr("127.0.0.1", 1415, dest);
    uv_tcp_connect(connect, socket, (const struct sockaddr*)dest, on_connect);

    uv_idle_t *idle = malloc(sizeof(uv_idle_t));
    uv_idle_init(loop,idle);
    uv_idle_start(idle,idle_activity);

    uv_timer_t *timer = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop,timer);
    uv_timer_start(timer,timer_activity,5000,1000);




    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    free(socket);
    free(connect);
    free(dest);
    return 0;
}
