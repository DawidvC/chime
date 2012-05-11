#include "file_source.h"
#include "runtime/tagging.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/core/closure/chime_closure.h"
#include "library/execution/execution.h"
#include "library/data/chime_data.h"

#ifdef PLATFORM_UNIX
#   include <assert.h>
#   include <errno.h>
#   include <fcntl.h>
#   include <stdio.h>
#   include <unistd.h>
#endif

chime_object_t* file_new(chime_class_t* klass, chime_object_t* url)
{
    chime_object_t* object;
    
    object = chime_object_create(chime_runtime_get_class("Source.File"));
    
    chime_object_invoke_1(object, "initialize:", url);
    
    return object;
}

chime_object_t* file_initialize(chime_object_t* instance, chime_object_t* url)
{
    file_platform_initialize(instance);
    
    return instance;
}

void file_platform_initialize(chime_object_t* instance)
{
#ifdef PLATFORM_UNIX
    file_initialize_file_descriptors(instance);
#endif
    
#ifdef PLATFORM_MAC_OS_X
    file_initialize_dispatch_sources(instance);
#endif
}

chime_object_t* file_finalize(chime_object_t* instance)
{
    file_platform_finalize(instance);
    
    return CHIME_NIL;
}

void file_platform_finalize(chime_object_t* instance)
{
#ifdef PLATFORM_UNIX
    file_finalize_file_descriptors(instance);
#endif
    
#ifdef PLATFORM_MAC_OS_X
    file_finalize_dispatch_sources(instance);
#endif
}

chime_object_t* file_open(chime_object_t* instance)
{
#ifdef PLATFORM_UNIX
    int descriptor;
    
    descriptor = open("/tmp/chime/compiler/tests/fixtures/two_class_methods_in_a_row.cm", O_RDWR | O_NONBLOCK);
    if (descriptor == -1)
    {
        // here, we should probably check why this failure occurred, and
        // set some kind of internal error condition that causes subsequent operations
        // to fail
        fprintf(stderr, "Open failed (%s)\n", strerror(errno));
        return CHIME_NIL;
    }
    
    file_set_file_descriptor(instance, READWRITE_DESCRIPTOR, descriptor);
#endif
    
    return CHIME_NIL;
}

chime_object_t* file_close(chime_object_t* instance)
{
#ifdef PLATFORM_UNIX
    file_close_file_descriptors(instance);
#endif
    
#ifdef PLATFORM_MAC_OS_X
    file_close_dispatch_sources(instance);
#endif
    
    return CHIME_NIL;
}

chime_object_t* file_on_read(chime_object_t* instance, chime_object_t* context, chime_object_t* function)
{
#ifdef PLATFORM_MAC_OS_X
    int               descriptor;
    dispatch_queue_t  queue;
    dispatch_source_t source;
    //chime_object_t*   data_object;
    
    queue       = execution_context_get_dispatch_queue(context);
    descriptor  = file_get_file_descriptor(instance, READWRITE_DESCRIPTOR);
    //data_object = 
    
    source = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, descriptor, 0, queue);
    file_set_dispatch_source(instance, FILE_READ_DISPATCH_SOURCE, source);
    
    chime_object_retain(function);
    dispatch_source_set_event_handler(source, ^{
        int             fd;
        size_t          estimated_size;
        ssize_t         actual_size;
        char*           buffer;
        chime_object_t* tagged_data;
        
        fd             = dispatch_source_get_handle(source);
        estimated_size = dispatch_source_get_data(source) + 1;
        
        if (estimated_size <= 1)
        {
            // fprintf(stderr, "** read zero, cancelling read source\n");
            dispatch_source_cancel(source);
            return;
        }
        
        //fprintf(stderr, "** estimated: %ld\n", estimated);
        
        buffer = (char*)malloc(estimated_size);
        assert(buffer);
        
        actual_size = read(fd, buffer, estimated_size);
        tagged_data = chime_tag_encode_raw_block(buffer);
        
        chime_closure_invoke_2((chime_closure_t*)function, tagged_data, chime_integer_encode(actual_size));
    });
    
    dispatch_source_set_cancel_handler(source, ^{
        // fprintf(stderr, "*** read source cancelled\n");
        
        chime_object_release(function);
    });
    
    dispatch_resume(source);
#endif
    
    return CHIME_NIL;
}

chime_object_t* file_on_write(chime_object_t* instance, chime_object_t* data, chime_object_t* context, chime_object_t* function)
{
    return CHIME_NIL;
}

#ifdef PLATFORM_UNIX
void file_initialize_file_descriptors(chime_object_t* instance)
{
    int* descriptors;
    
    descriptors = malloc(sizeof(int) * DESCRIPTOR_COUNT);
    bzero(descriptors, sizeof(int) * DESCRIPTOR_COUNT);
    
    chime_object_set_attribute(instance, "_file_descriptors", chime_tag_encode_raw_block(descriptors));
}

void file_finalize_file_descriptors(chime_object_t* instance)
{
    int* descriptors;
    
    file_close_file_descriptors(instance);
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    free(descriptors);
}

void file_close_file_descriptors(chime_object_t* instance)
{
    int  i;
    
    for (i = 0; i < DESCRIPTOR_COUNT; ++i)
    {
        int result;
        int descriptor;
        
        descriptor = file_get_file_descriptor(instance, i);
        if (descriptor == 0)
            continue;
        
        result = close(descriptor);
        assert(result == 0);
        
        file_set_file_descriptor(instance, i, 0);
    }
}

int  file_get_file_descriptor(chime_object_t* instance, int type)
{
    int* descriptors;
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    return descriptors[type];
}

void file_set_file_descriptor(chime_object_t* instance, int type, int descriptor)
{
    int* descriptors;
    
    descriptors = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_file_descriptors"));
    
    descriptors[type] = descriptor;
}
#endif

#ifdef PLATFORM_MAC_OS_X
void file_initialize_dispatch_sources(chime_object_t* instance)
{
    dispatch_source_t* sources;
    
    sources = malloc(sizeof(void*) * DISPATCH_SOURCE_COUNT);
    bzero(sources, sizeof(void*) * DISPATCH_SOURCE_COUNT);
    
    chime_object_set_attribute(instance, "_dispatch_sources", chime_tag_encode_raw_block(sources));
}

void file_finalize_dispatch_sources(chime_object_t* instance)
{
    dispatch_source_t* sources;
    
    file_close_dispatch_sources(instance);
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    free(sources);
}

void file_close_dispatch_sources(chime_object_t* instance)
{
    int i;
    
    for (i = 0; i < DISPATCH_SOURCE_COUNT; ++i)
    {
        file_set_dispatch_source(instance, i, NULL);
    }
}

dispatch_source_t file_get_dispatch_source(chime_object_t* instance, int type)
{
    dispatch_source_t* sources;
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    return sources[type];
}

void file_set_dispatch_source(chime_object_t* instance, int type, dispatch_source_t source)
{
    dispatch_source_t* sources;
    
    sources = chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_sources"));
    
    if (sources[type])
    {
        dispatch_source_cancel(sources[type]);
        dispatch_release(sources[type]);
    }
    
    sources[type] = source;
}
#endif
