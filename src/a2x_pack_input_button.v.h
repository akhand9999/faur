/*
    Copyright 2010, 2016-2018 Alex Margarit

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

#include "a2x_pack_input_button.p.h"

typedef struct AButtonSource AButtonSource;

extern void a_input_button__init(void);
extern void a_input_button__uninit(void);

extern AButtonSource* a_input_button__newSource(const char* Name, const char* Id);
extern void a_input_button__freeSource(AButtonSource* Button);

extern AButtonSource* a_input_button__keyGet(const char* Id);
extern void a_input_button__forwardToButton(AButtonSource* Button, AButtonSource* Binding);
extern void a_input_button__stateSet(AButtonSource* Button, bool Pressed);
extern void a_input_button__processQueue(void);
