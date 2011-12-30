// Chime Library: chime_data.h

#ifndef CHIME_DATA_H
#define CHIME_DATA_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_data_initialize(void);

chime_object_t* data_initialize(chime_object_t* instance);
chime_object_t* data_finalize(chime_object_t* instance);

chime_object_t* data_get_length(chime_object_t* instance);
chime_object_t* data_append_block(chime_object_t* instance, chime_object_t* block, chime_object_t* size);
chime_object_t* data_append_raw_block(chime_object_t* instance, void* ptr, unsigned long size);
chime_object_t* data_each(chime_object_t* instance, chime_object_t* function);

chime_object_t* chime_data_get_block(chime_object_t* instance, chime_object_t* index);
chime_object_t* chime_data_get_block_size(chime_object_t* instance, chime_object_t* index);

#ifdef __cplusplus
}
#endif

#endif // CHIME_DATA_H
