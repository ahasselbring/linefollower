#pragma once

#define MODULE(name) \
typedef struct name {

#define REQUIRES(name) const name##_t* name;

#define USES(name) const name##_t* name;

#define PROVIDES(name) name##_t* name;

#define END_MODULE(name) \
} name##_t; \
void name##_init(name##_t* this); \
void name##_execute(name##_t* this);

#define MODULE_INIT(name) void name##_init(name##_t* this)

#define MODULE_GET(name) \
  name##_t* global_get_##name(); \
  this->name = global_get_##name()

#define MODULE_EXECUTE(name) void name##_execute(name##_t* this)

#if defined TARGET_SIMULATOR && !(defined NDEBUG)
#include <stdio.h>
#define DEBUG_OUTPUT(...) printf(__VA_ARGS__)
#else
#define DEBUG_OUTPUT(...)
#endif
