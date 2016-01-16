/*
    Copyright 2016 Alex Margarit

    This file is part of a2x-framework.

    a2x-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    a2x-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with a2x-framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "a2x_pack_out.p.h"

extern void a_out__init(void);
extern void a_out__uninit(void);

#if A_PLATFORM_LINUXPC
    #define A_OUT__HEADER(title, color) "\033[" #color ";1m[ a2x " title " ]\033[0m "
#else
    #define A_OUT__HEADER(title, color) "[ a2x " title " ] "
#endif

#define a_out(...)                        \
({                                        \
    if(!a2x_bool("app.quiet")) {          \
        printf(A_OUT__HEADER("Msg", 32)); \
        printf(__VA_ARGS__);              \
        printf("\n");                     \
    }                                     \
})

#define a_warning(...)                         \
({                                             \
    fprintf(stderr, A_OUT__HEADER("Wrn", 33)); \
    fprintf(stderr, __VA_ARGS__);              \
    fprintf(stderr, "\n");                     \
})

#define a_error(...)                           \
({                                             \
    fprintf(stderr, A_OUT__HEADER("Err", 31)); \
    fprintf(stderr, __VA_ARGS__);              \
    fprintf(stderr, "\n");                     \
})

#define a_fatal(...)                           \
({                                             \
    fprintf(stderr, A_OUT__HEADER("Ftl", 35)); \
    fprintf(stderr, __VA_ARGS__);              \
    fprintf(stderr, "\n");                     \
    exit(1);                                   \
})
