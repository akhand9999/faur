/*
    Copyright 2016-2018 Alex Margarit

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

#include "a2x_pack_input_controller.p.h"

#include "a2x_pack_input_analog.v.h"
#include "a2x_pack_strhash.v.h"

extern void a_input_controller__init(void);
extern void a_input_controller__init2(void);
extern void a_input_controller__uninit(void);

extern void a_controller__new(bool Generic, bool IsMapped);

extern void a_controller__buttonAdd(AButtonSource* Button, const char* Id);
extern AButtonSource* a_controller__buttonGet(const char* Id);
extern AStrHash* a_controller__buttonCollectionGet(void);

extern void a_controller__analogAdd(AAnalogSource* Analog, const char* Id);
extern AAnalogSource* a_controller__analogGet(const char* Id);
extern AStrHash* a_controller__analogCollectionGet(void);
