// Chime Library: source/socket_source.h

#ifndef CHIME_SOCKET_SOURCE_H
#define CHIME_SOCKET_SOURCE_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* socket_open(chime_object_t* instance);
chime_object_t* socket_close(chime_object_t* instance);

chime_object_t* socket_on_read(chime_object_t* instance, chime_object_t* context, chime_object_t* function);
chime_object_t* socket_on_write(chime_object_t* instance, chime_object_t* data, chime_object_t* context, chime_object_t* function);

chime_object_t* socket_get_port(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_SOCKET_SOURCE_H
