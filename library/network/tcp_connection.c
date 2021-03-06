#include "tcp_connection.h"

#include <stdio.h>

chime_object_t* tcp_connection_new(chime_class_t* klass, chime_object_t* tcp_socket)
{
    chime_object_t* object;
    
    object = chime_object_create(chime_runtime_get_class("Network.TCPConnection"));
    
    chime_object_invoke_1(object, "initialize:", tcp_socket);
    
    return object;
}

chime_object_t* tcp_connection_initialize(chime_object_t* instance, chime_object_t* tcp_socket)
{
    chime_object_set_attribute(instance, "socket", tcp_socket);
    
    return instance;
}

chime_object_t* tcp_connection_get_socket(chime_object_t* instance)
{
    return chime_object_get_attribute(instance, "socket");
}

chime_object_t* tcp_connection_close(chime_object_t* instance)
{
    return chime_object_invoke_0(chime_object_get_attribute_unretained(instance, "socket"), "close");
}