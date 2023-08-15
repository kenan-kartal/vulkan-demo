#ifndef CONFIG_H
#define CONFIG_H

#ifdef NDEBUG
static constexpr bool enable_validation_layers = false;
#else
static constexpr bool enable_validation_layers = true;
#endif

#endif
