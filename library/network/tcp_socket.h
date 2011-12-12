// Chime Library: network/tcp_socket.h

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* tcp_socket_new(chime_class_t* klass, chime_object_t* port_number);

chime_object_t* tcp_socket_initialize(chime_object_t* instance, chime_object_t* port_number);
chime_object_t* tcp_socket_finalize(chime_object_t* instance);
chime_object_t* tcp_socket_get_port(chime_object_t* instance);

chime_object_t* tcp_socket_close(chime_object_t* instance);

chime_object_t* tcp_socket_on_connection(chime_object_t* instance, chime_object_t* function);
chime_object_t* tcp_socket_on_read(chime_object_t* instance, chime_object_t* function);
chime_object_t* tcp_socket_on_write(chime_object_t* instance, chime_object_t* function);

#ifdef __cplusplus
}
#endif

#endif
