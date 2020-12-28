/*
    Copyright 2020 Alex Margarit <alex@alxm.org>
    This file is part of Faur, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef F_INC_GENERAL_SYM_P_H
#define F_INC_GENERAL_SYM_P_H

#include "../general/f_system_includes.h"

extern void f_sym_set(const char* Name, uintptr_t Value);

#ifndef FAUR_IMPLEMENT_SYM
    #define f_sym_set(Symbol) f_sym_set(#Symbol, (uintptr_t)Symbol)
#endif

extern bool f_sym_test(const char* Name);

extern int f_sym_int(const char* Name);
extern unsigned f_sym_intu(const char* Name);
extern uintptr_t f_sym_address(const char* Name);

#endif // F_INC_GENERAL_SYM_P_H
