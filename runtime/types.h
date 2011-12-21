// Chime Runtime: chime_types.h

#ifndef CHIME_TYPES_H
#define CHIME_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef bool
typedef unsigned char bool;

#define true (1)
#define false (0)
#endif // bool
#endif // __cplusplus

#define CHIME_NIL   ((chime_object_t*)0x00000000)
#define CHIME_TRUE  ((chime_object_t*)0x00000006)
#define CHIME_FALSE ((chime_object_t*)0x0000000A)

#define CHIME_NULL          CHIME_NIL
#define CHIME_LITERAL_NULL  CHIME_NIL
#define CHIME_LITERAL_TRUE  CHIME_TRUE
#define CHIME_LITERAL_FALSE CHIME_FALSE

typedef struct _chime_class chime_class_t;

typedef struct _chime_object chime_object_t;

#ifdef __cplusplus
}
#endif

#endif // CHIME_TYPES_H
