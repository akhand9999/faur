/*
    Copyright 2010, 2016-2019 Alex Margarit <alex@alxm.org>
    This file is part of a2x, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received Alpha copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "general/a_system_includes.h"

typedef struct ASprite ASprite;

#include "graphics/a_color.p.h"
#include "math/a_fix.p.h"

extern ASprite* a_sprite_newFromPng(const char* Path, int X, int Y, int FrameWidth, int FrameHeight);
extern ASprite* a_sprite_newFromSprite(const ASprite* Sheet, int X, int Y, int FrameWidth, int FrameHeight);
extern ASprite* a_sprite_newBlank(int Width, int Height, unsigned Frames, bool ColorKeyed);
extern ASprite* a_sprite_dup(const ASprite* Sprite);
extern void a_sprite_free(ASprite* Sprite);

extern void a_sprite_blit(const ASprite* Sprite, unsigned Frame, int X, int Y);
extern void a_sprite_blitEx(const ASprite* Sprite, unsigned Frame, int X, int Y, AFix Scale, unsigned Angle, int CenterX, int CenterY);

extern void a_sprite_swapColor(ASprite* Sprite, APixel OldColor, APixel NewColor);
extern void a_sprite_swapColors(ASprite* Sprite, const APixel* OldColors, const APixel* NewColors, unsigned NumColors);

extern AVectorInt a_sprite_sizeGet(const ASprite* Sprite);
extern int a_sprite_sizeGetWidth(const ASprite* Sprite);
extern int a_sprite_sizeGetHeight(const ASprite* Sprite);

extern unsigned a_sprite_framesNumGet(const ASprite* Sprite);

extern const APixel* a_sprite_pixelsGetBuffer(const ASprite* Sprite, unsigned Frame);
extern APixel a_sprite_pixelsGetValue(const ASprite* Sprite, unsigned Frame, int X, int Y);
