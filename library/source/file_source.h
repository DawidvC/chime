// Chime Library: source/file_source.h

#ifndef CHIME_FILE_SOURCE_H
#define CHIME_FILE_SOURCE_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* file_new(chime_class_t* klass, chime_object_t* url);

chime_object_t* file_initialize(chime_object_t* instance, chime_object_t* url);
void            file_platform_initialize(chime_object_t* instance);
chime_object_t* file_finalize(chime_object_t* instance);
void            file_platform_finalize(chime_object_t* instance);

chime_object_t* file_open(chime_object_t* instance);
chime_object_t* file_close(chime_object_t* instance);

chime_object_t* file_on_read(chime_object_t* instance, chime_object_t* context, chime_object_t* function);
chime_object_t* file_on_write(chime_object_t* instance, chime_object_t* data, chime_object_t* context, chime_object_t* function);

#ifdef PLATFORM_UNIX
#   define READWRITE_DESCRIPTOR 0

#   define DESCRIPTOR_COUNT     2

void file_initialize_file_descriptors(chime_object_t* instance);
void file_finalize_file_descriptors(chime_object_t* instance);
void file_close_file_descriptors(chime_object_t* instance);
int  file_get_file_descriptor(chime_object_t* instance, int type);
void file_set_file_descriptor(chime_object_t* instance, int type, int descriptor);
#endif

#ifdef PLATFORM_MAC_OS_X
#   include <dispatch/dispatch.h>

#   define FILE_READ_DISPATCH_SOURCE  0
#   define FILE_WRITE_DISPATCH_SOURCE 1

#   define DISPATCH_SOURCE_COUNT      4

void              file_initialize_dispatch_sources(chime_object_t* instance);
void              file_finalize_dispatch_sources(chime_object_t* instance);
void              file_close_dispatch_sources(chime_object_t* instance);
dispatch_source_t file_get_dispatch_source(chime_object_t* instance, int type);
void              file_set_dispatch_source(chime_object_t* instance, int type, dispatch_source_t source);
#endif

#ifdef __cplusplus
}
#endif

#endif // CHIME_FILE_SOURCE_H
