#include "tcp_socket.h"
#include "runtime/platform.h"
#include "runtime/tagging.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/core/closure/chime_closure.h"
#include "tcp_connection.h"

#ifdef PLATFORM_UNIX
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>

#   include <assert.h>
#   include <stdlib.h>
#   include <stdio.h>
#   include <unistd.h>
#endif

#ifdef PLATFORM_MAC_OS_X
#   include <dispatch/dispatch.h>
#endif

#ifdef PLATFORM_UNIX
static void unix_accept_connection(int listen_socket, chime_object_t* function);
#endif

chime_object_t* tcp_socket_new(chime_class_t* klass, chime_object_t* port_number)
{
    chime_object_t* object;
    
    object = chime_object_create(chime_runtime_get_class("Network.TCPSocket"));
    
    chime_object_invoke_1(object, "initialize:", port_number);
    
    return object;
}

chime_object_t* tcp_socket_initialize(chime_object_t* instance, chime_object_t* port_number)
{
    int port;
    
    port = chime_integer_decode(port_number);
    
#ifdef PLATFORM_UNIX
    int* descriptors;
    
    descriptors = chime_allocate(1);
    chime_zero_memory(descriptors, 1);
    
    if (port > 0)
    {
        descriptors[0] = socket(PF_INET, SOCK_STREAM, 0);
        assert(descriptors[0] >= 0);
        
        descriptors[1] = socket(PF_INET6, SOCK_STREAM, 0);
        assert(descriptors[1] >= 0);
    }
    
    chime_object_set_attribute(instance, "_file_descriptors", chime_tag_encode_raw_block(descriptors));
#endif
    
#ifdef PLATFORM_MAC_OS_X
    dispatch_source_t* sources;
    
    sources = chime_allocate(1);
    chime_zero_memory(sources, 1);
    
    chime_object_set_attribute(instance, "_dispatch_sources", chime_tag_encode_raw_block(sources));
#endif
    
    chime_object_set_attribute(instance, "port", port_number);
    
    return instance;
}

chime_object_t* tcp_socket_finalize(chime_object_t* instance)
{
    fprintf(stderr, "finalizing %p\n", instance);
    
    tcp_socket_close(instance);
    
#ifdef PLATFORM_UNIX
    int* descriptors;
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    chime_deallocate(descriptors);
#endif
    
#ifdef PLATFORM_MAC_OS_X
    dispatch_source_t* sources;
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    chime_deallocate(sources);
#endif
    
    return CHIME_NULL;
}

chime_object_t* tcp_socket_get_port(chime_object_t* instance)
{
    return chime_object_get_attribute(instance, "port");
}

chime_object_t* tcp_socket_close(chime_object_t* instance)
{
#ifdef PLATFORM_UNIX
    int  result;
    int  i;
    int* descriptors;
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    for (i = 0; i < 4; ++i)
    {
        if (descriptors[i] == 0)
            continue;
        
        result = close(descriptors[i]);
        assert(result == 0);
        descriptors[i] = 0;
    }
#endif
    
#ifdef PLATFORM_MAC_OS_X
    dispatch_source_t* sources;
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    for (i = 0; i < 4; ++i)
    {
        if (sources[i] == 0)
            continue;
        
        fprintf(stderr, "<%p> cancelling: %d\n", instance, i);
        dispatch_source_cancel(sources[i]);
        dispatch_release(sources[i]);
        sources[i] = 0;
    }
#endif
    
    return CHIME_NULL;
}

chime_object_t* tcp_socket_on_connection(chime_object_t* instance, chime_object_t* function)
{
    chime_object_set_attribute(instance, "on_connection_handler", function);
    
#ifdef PLATFORM_UNIX
    int  port;
    int* descriptors;
    int  result;
    int  yes;
    
    port        = chime_integer_decode(chime_object_get_attribute_unretained(instance, "port"));
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    yes         = 1;
    
    // IPv4 address
    struct sockaddr_in addr4 = { sizeof(addr4), AF_INET,  htons(port), { INADDR_ANY }, { 0 } };
    
    result = setsockopt(descriptors[0], SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));
    assert(result == 0);
    
    result = bind(descriptors[0], (void*)&addr4, sizeof(addr4));
    assert(result == 0);
    
    result = listen(descriptors[0], 16);
    assert(result == 0);
#endif
    
#ifdef PLATFORM_MAC_OS_X
    dispatch_source_t* sources;
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    if (sources[0])
    {
        dispatch_source_cancel(sources[0]);
        dispatch_release(sources[0]);
    }
    
    //sources[0] = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, descriptors[0], 0, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0));
    sources[0] = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, descriptors[0], 0, dispatch_get_main_queue());
    
    dispatch_source_set_event_handler(sources[0], ^{
        int listen_socket;
        
        listen_socket = dispatch_source_get_handle(sources[0]);
        
        unix_accept_connection(listen_socket, function);
    });
    
    dispatch_source_set_cancel_handler(sources[0], ^{
        fprintf(stderr, "*** listen socket cancelled\n");
    });
    
    dispatch_resume(sources[0]);
    
#endif
    
    return CHIME_NULL;
}

chime_object_t* tcp_socket_on_read(chime_object_t* instance, chime_object_t* function)
{
    chime_object_set_attribute(instance, "on_read_handler", function);
    
#ifdef PLATFORM_MAC_OS_X
    int*               descriptors;
    dispatch_source_t* sources;
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    //sources[2] = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, descriptors[2], 0, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0));
    sources[2] = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, descriptors[2], 0, dispatch_get_main_queue());
    
    fprintf(stderr, "socket on_read %p with closure %p\n", instance, function);
    
    dispatch_source_set_event_handler(sources[2], ^{
        int    descriptor = dispatch_source_get_handle(sources[2]);
        size_t estimated  = dispatch_source_get_data(sources[2]) + 1;
        
        if (estimated <= 1)
        {
            fprintf(stderr, "** read zero, cancelling read source\n");
            dispatch_source_cancel(sources[2]);
            return;
        }
        
        fprintf(stderr, "** estimated: %ld\n", estimated);
        
        char* buffer = (char*)malloc(estimated);
        
        if (buffer)
        {
            ssize_t actual = read(descriptor, buffer, (estimated));
            
            buffer[actual-1] = 0; // null_terminate
            
            fprintf(stderr, "** read:\n%s\n", buffer);
            free(buffer);
            
            chime_closure_invoke((chime_closure_t*)function, CHIME_NULL);
        }
    });
    
    dispatch_source_set_cancel_handler(sources[2], ^{
        fprintf(stderr, "*** read source cancelled\n");
    });
    
    dispatch_resume(sources[2]);
    
#endif
    
    return CHIME_NULL;
}

chime_object_t* tcp_socket_on_write(chime_object_t* instance, chime_object_t* function)
{
    return CHIME_NULL;
}

#ifdef PLATFORM_UNIX
static void unix_accept_connection(int listen_socket, chime_object_t* function)
{
    chime_object_t* socket_object;
    chime_object_t* connection;
    struct sockaddr addr;
    socklen_t       addrlen = sizeof(addr);
    int*            descriptors;
    
    socket_object = tcp_socket_new(NULL, chime_integer_encode(-1));
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(socket_object, "_file_descriptors"));
    
    descriptors[2] = accept(listen_socket, &addr, &addrlen);
    
    connection = tcp_connection_new(NULL, socket_object);
    chime_object_release(socket_object);
    
    chime_closure_invoke((chime_closure_t*)function, connection);
    
    fprintf(stderr, "closure invoked, releasing connection\n");
    chime_object_release(connection);
    fprintf(stderr, "closure invoked, released connection\n");
}

#endif
