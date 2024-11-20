#pragma once

#include "log.h"
#include "string.h"

void cleanup_strings(void);

void register_string(String *str);

void unregister_string(String *str);

void string_check_for_signal(void);

void signal_handler(int signum);

void register_signal_handlers(void);
