#include "network.h"
#include "library/network/tcp_socket.h"
#include "library/network/tcp_connection.h"

void chime_network_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create("Network.TCPSocket", chime_runtime_get_class("Source.Socket"));
    
    // class methods
    chime_class_set_class_method(klass, "new:", tcp_socket_new);
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize:",                    tcp_socket_initialize);
    chime_class_set_instance_method(klass, "finalize",                       tcp_socket_finalize);
    chime_class_set_instance_method(klass, "port",                           tcp_socket_get_port);
    chime_class_set_instance_method(klass, "close",                          tcp_socket_close);
    chime_class_set_instance_method(klass, "dispatch_to:on_connection:",     tcp_socket_on_connection);
    chime_class_set_instance_method(klass, "dispatch_to:on_read:",           tcp_socket_on_read);
    chime_class_set_instance_method(klass, "write:dispatch_to:on_complete:", tcp_socket_on_write);
    
    klass = chime_class_create_object_subclass("Network.TCPConnection");
    
    // class methods
    chime_class_set_class_method(klass, "new:", tcp_connection_new);
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize:", tcp_connection_initialize);
    chime_class_set_instance_method(klass, "socket",      tcp_connection_get_socket);
    chime_class_set_instance_method(klass, "close",       tcp_connection_close);
}
