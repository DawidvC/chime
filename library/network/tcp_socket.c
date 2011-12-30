#include "tcp_socket.h"
#include "runtime/tagging.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/core/closure/chime_closure.h"
#include "tcp_connection.h"
#include "library/execution/execution.h"
#include "library/data/chime_data.h"

#ifdef PLATFORM_UNIX
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>

#   include <assert.h>
#   include <stdlib.h>
#   include <stdio.h>
#   include <unistd.h>
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
    // fprintf(stderr, "finalizing %p\n", instance);
    
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
    int  i;
    
    for (i = 0; i < 2; ++i)
    {
        int result;
        int descriptor;
        
        descriptor = tcp_socket_get_file_descriptor(instance, i);
        if (descriptor == 0)
            continue;
        
        result = close(descriptor);
        assert(result == 0);
        
        tcp_socket_set_file_descriptor(instance, i, 0);
    }
#endif
    
#ifdef PLATFORM_MAC_OS_X
    for (i = 0; i < 4; ++i)
    {
        tcp_socket_set_dispatch_source(instance, i, NULL);
    }
#endif
    
    return CHIME_NULL;
}

chime_object_t* tcp_socket_on_connection(chime_object_t* instance, chime_object_t* context, chime_object_t* function)
{
    chime_object_set_attribute(instance, "on_connection_handler", function);
    
#ifdef PLATFORM_UNIX
    int port;
    int descriptor;
    int result;
    int yes;
    
    port       = chime_integer_decode(chime_object_get_attribute_unretained(instance, "port"));
    descriptor = tcp_socket_get_file_descriptor(instance, LISTEN_DESCRIPTOR);
    yes        = 1;
    
    // IPv4 address
    struct sockaddr_in addr4 = { sizeof(addr4), AF_INET,  htons(port), { INADDR_ANY }, { 0 } };
    
    result = setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));
    assert(result == 0);
    
    result = bind(descriptor, (void*)&addr4, sizeof(addr4));
    assert(result == 0);
    
    result = listen(descriptor, 16);
    assert(result == 0);
#endif
    
#ifdef PLATFORM_MAC_OS_X
    dispatch_queue_t  queue;
    dispatch_source_t source;
    
    queue  = execution_context_get_dispatch_queue(context);
    source = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, descriptor, 0, queue);
    tcp_socket_set_dispatch_source(instance, LISTEN_SOURCE, source);
    
    dispatch_source_set_event_handler(source, ^{
        int listen_socket;
        
        listen_socket = dispatch_source_get_handle(source);
        
        tcp_socket_accept_connection(listen_socket, function);
    });
    
    dispatch_source_set_cancel_handler(source, ^{
        // fprintf(stderr, "*** listen socket cancelled\n");
    });
    
    dispatch_resume(source);
    
#endif
    
    return CHIME_NULL;
}

chime_object_t* tcp_socket_on_read(chime_object_t* instance, chime_object_t* context, chime_object_t* function)
{
#ifdef PLATFORM_MAC_OS_X
    int               descriptor;
    dispatch_queue_t  queue;
    dispatch_source_t source;
    
    queue      = execution_context_get_dispatch_queue(context);
    descriptor = tcp_socket_get_file_descriptor(instance, READWRITE_DESCRIPTOR);
    
    source = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, descriptor, 0, queue);
    tcp_socket_set_dispatch_source(instance, READ_SOURCE, source);
    
    chime_object_retain(function);
    dispatch_source_set_event_handler(source, ^{
        int    fd;
        size_t estimated;
        
        fd        = dispatch_source_get_handle(source);
        estimated = dispatch_source_get_data(source) + 1;
        
        if (estimated <= 1)
        {
            // fprintf(stderr, "** read zero, cancelling read source\n");
            dispatch_source_cancel(source);
            return;
        }
        
        // fprintf(stderr, "** estimated: %ld\n", estimated);
        
        char* buffer = (char*)chime_malloc(estimated);
        
        if (buffer)
        {
            chime_object_t* tagged_data;
            
            ssize_t actual = read(fd, buffer, (estimated));
            
            buffer[actual] = 0; // null_terminate
            
            tagged_data = chime_tag_encode_raw_block(buffer);
            
            chime_closure_invoke_2((chime_closure_t*)function, tagged_data, chime_integer_encode(actual+1));
        }
    });
    
    dispatch_source_set_cancel_handler(source, ^{
        // fprintf(stderr, "*** read source cancelled\n");
        
        chime_object_release(function);
    });
    
    dispatch_resume(source);
    
#endif
    
    return CHIME_NULL;
}

chime_object_t* tcp_socket_on_write(chime_object_t* instance, chime_object_t* data, chime_object_t* context, chime_object_t* function)
{
#ifdef PLATFORM_MAC_OS_X
    signed long       block_index;
    signed long       offset;
    int               descriptor;
    dispatch_queue_t  queue;
    dispatch_source_t source;
    
    queue       = execution_context_get_dispatch_queue(context);
    descriptor  = tcp_socket_get_file_descriptor(instance, READWRITE_DESCRIPTOR);
    source      = dispatch_source_create(DISPATCH_SOURCE_TYPE_WRITE, descriptor, 0, queue);
    block_index = 0;
    offset      = 0;
    
    tcp_socket_set_dispatch_source(instance, WRITE_SOURCE, source);
    
    chime_object_retain(function);
    chime_object_retain(data);
    dispatch_source_set_event_handler(source, ^{
        int             fd;
        char*           block;
        size_t          block_size;
        ssize_t         bytes_written;
        chime_object_t* index_object;
        
        fd           = dispatch_source_get_handle(source);
        index_object = chime_integer_encode(block_index);
        block        = (char*)chime_tag_decode_raw_block(chime_data_get_block(data, index_object));
        block_size   = chime_integer_decode(chime_data_get_block_size(data, index_object));
        
        bytes_written = write(fd, block, block_size);
        // fprintf(stderr, "*** wrote %ld of %lu bytes\n", bytes_written, block_size);
        
        dispatch_source_cancel(source);
    });
    
    dispatch_source_set_cancel_handler(source, ^{
        // fprintf(stderr, "*** write source cancelled\n");
        
        chime_closure_invoke_0((chime_closure_t*)function);
        chime_object_release(function);
        chime_object_release(data);
    });
    
    dispatch_resume(source);
    
#endif
    
    return CHIME_NULL;
}

#ifdef PLATFORM_UNIX
int  tcp_socket_get_file_descriptor(chime_object_t* instance, int type)
{
    int* descriptors;
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    return descriptors[type];
}

void tcp_socket_set_file_descriptor(chime_object_t* instance, int type, int descriptor)
{
    int* descriptors;
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    descriptors[type] = descriptor;
}

void tcp_socket_accept_connection(int listen_descriptor, chime_object_t* function)
{
    chime_object_t* socket_object;
    chime_object_t* connection;
    struct sockaddr addr;
    socklen_t       addrlen = sizeof(addr);
    int             accepted_descriptor;
    
    accepted_descriptor = accept(listen_descriptor, &addr, &addrlen);
    
    socket_object = tcp_socket_new(NULL, chime_integer_encode(-1));
    
    tcp_socket_set_file_descriptor(socket_object, READWRITE_DESCRIPTOR, accepted_descriptor);
    
    connection = tcp_connection_new(NULL, socket_object);
    chime_object_release(socket_object);
    
    chime_closure_invoke_1((chime_closure_t*)function, connection);
    
    chime_object_release(connection);
}

#endif

#ifdef PLATFORM_MAC_OS_X
dispatch_source_t tcp_socket_get_dispatch_source(chime_object_t* instance, int type)
{
    dispatch_source_t* sources;
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    return sources[type];
}

void tcp_socket_set_dispatch_source(chime_object_t* instance, int type, dispatch_source_t source)
{
    dispatch_source_t* sources;
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    if (sources[type])
    {
        fprintf(stderr, "<%p> cancelling: %d\n", instance, type);
        
        dispatch_source_cancel(sources[type]);
        dispatch_release(sources[type]);
    }
    
    sources[type] = source;
}

#endif

