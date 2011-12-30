// Chime Library: network/tcp_socket.h

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "runtime/chime_runtime.h"
#include "runtime/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* tcp_socket_new(chime_class_t* klass, chime_object_t* port_number);

chime_object_t* tcp_socket_initialize(chime_object_t* instance, chime_object_t* port_number);
chime_object_t* tcp_socket_finalize(chime_object_t* instance);
chime_object_t* tcp_socket_get_port(chime_object_t* instance);

chime_object_t* tcp_socket_close(chime_object_t* instance);

chime_object_t* tcp_socket_on_connection(chime_object_t* instance, chime_object_t* context, chime_object_t* function);
chime_object_t* tcp_socket_on_read(chime_object_t* instance, chime_object_t* context, chime_object_t* function);
chime_object_t* tcp_socket_on_write(chime_object_t* instance, chime_object_t* data, chime_object_t* context, chime_object_t* function);

#ifdef PLATFORM_UNIX
#   define LISTEN_DESCRIPTOR    0
#   define READWRITE_DESCRIPTOR 1

int  tcp_socket_get_file_descriptor(chime_object_t* instance, int type);
void tcp_socket_set_file_descriptor(chime_object_t* instance, int type, int descriptor);

void tcp_socket_accept_connection(int listen_descriptor, chime_object_t* function);
#endif

#ifdef PLATFORM_MAC_OS_X
#   include <dispatch/dispatch.h>

#   define LISTEN_SOURCE 0
#   define READ_SOURCE   1
#   define WRITE_SOURCE  2

dispatch_source_t tcp_socket_get_dispatch_source(chime_object_t* instance, int type);
void              tcp_socket_set_dispatch_source(chime_object_t* instance, int type, dispatch_source_t source);
#endif

#ifdef __cplusplus
}
#endif

#endif
