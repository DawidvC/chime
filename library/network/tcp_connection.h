// Chime Library: network/tcp_connection.h

#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* tcp_connection_new(chime_class_t* klass, chime_object_t* tcp_socket);

chime_object_t* tcp_connection_initialize(chime_object_t* instance, chime_object_t* tcp_socket);
chime_object_t* tcp_connection_get_socket(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // TCP_CONNECTION_H
