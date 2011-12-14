#include "network.h"
#include "library/network/tcp_socket.h"
#include "library/network/tcp_connection.h"

void chime_network_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Network.TCPSocket");
    
    // class methods
    chime_class_set_class_method(klass, "new:", tcp_socket_new);
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize:",    tcp_socket_initialize);
    chime_class_set_instance_method(klass, "finalize",       tcp_socket_finalize);
    chime_class_set_instance_method(klass, "port",           tcp_socket_get_port);
    chime_class_set_instance_method(klass, "close",          tcp_socket_close);
    chime_class_set_instance_method(klass, "on_connection:", tcp_socket_on_connection);
    chime_class_set_instance_method(klass, "on_read:",       tcp_socket_on_read);
    
    klass = chime_class_create_object_subclass("Network.TCPConnection");
    
    // class methods
    chime_class_set_class_method(klass, "new:", tcp_connection_new);
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize:", tcp_connection_initialize);
    chime_class_set_instance_method(klass, "socket",      tcp_connection_get_socket);
    chime_class_set_instance_method(klass, "close",       tcp_connection_close);
}
