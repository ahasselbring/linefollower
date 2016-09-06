#pragma once

#define MODULE(name) \
typedef struct name {

#define REQUIRES(name) const name##_t* name;

#define USES(name) const name##_t* name;

#define PROVIDES(name) name##_t* name;

#define END_MODULE(name) \
} name##_t; \
void name##_init(name##_t* self); \
void name##_execute(name##_t* self);

#define MODULE_INIT(name) void name##_init(name##_t* self)

#define MODULE_GET(name) \
  name##_t* global_get_##name(); \
  self->name = global_get_##name()

#define MODULE_EXECUTE(name) void name##_execute(name##_t* self)

#if defined TARGET_SIMULATOR
void debug_output(const char* fmt, ...);
#define DEBUG_PRINT(...) debug_output(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif
