#include "source.h"
#include "file_source.h"
#include "socket_source.h"

void chime_source_initialize(void)
{
    chime_class_t* source_class;
    chime_class_t* klass;
    
    source_class = chime_class_create_object_subclass("Source");
    
    klass = chime_class_create("Source.File", source_class);
    
    chime_class_set_class_method(klass,    "new:", file_new);
    
    // Source.File instance methods
    chime_class_set_instance_method(klass, "initialize:",                    file_initialize);
    chime_class_set_instance_method(klass, "finalize",                       file_finalize);
    
    chime_class_set_instance_method(klass, "open",                           file_open);
    chime_class_set_instance_method(klass, "close",                          file_close);
    chime_class_set_instance_method(klass, "dispatch_to:on_read:",           file_on_read);
    chime_class_set_instance_method(klass, "write:dispatch_to:on_complete:", file_on_write);
    
    // klass = chime_class_create("Source.Socket", source_class);
    // 
    // // Source.Socket instance methods
    // chime_class_set_instance_method(klass, "open",                           socket_open);
    // chime_class_set_instance_method(klass, "close",                          socket_close);
    // chime_class_set_instance_method(klass, "dispatch_to:on_read:",           socket_on_read);
    // chime_class_set_instance_method(klass, "write:dispatch_to:on_complete:", socket_on_write);
}
