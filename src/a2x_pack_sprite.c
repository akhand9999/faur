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

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "a2x_pack_sprite.v.h"

#include "a2x_pack_main.v.h"
#include "a2x_pack_mem.v.h"
#include "a2x_pack_png.v.h"
#include "a2x_pack_screen.v.h"
#include "a2x_pack_str.v.h"

APixel a_sprite__colorKey;
APixel a_sprite__colorLimit;

void a_sprite__init(void)
{
    a_sprite__colorKey = a_pixel_fromHex(A_CONFIG_COLOR_SPRITE_KEY);
    a_sprite__colorLimit = a_pixel_fromHex(A_CONFIG_COLOR_SPRITE_BORDER);
}

static ASprite* spriteNew(APixels* Pixels)
{
    ASprite* s = a_mem_malloc(sizeof(ASprite));

    s->nameId = NULL;
    s->pixels = Pixels;

    return s;
}

ASprite* a_sprite_newFromPng(const char* Path)
{
    APixels* pixels = a_png__readFile(Path);

    if(pixels == NULL) {
        A__FATAL("a_sprite_newFromPng(%s): Cannot read file", Path);
    }

    ASprite* s = spriteNew(pixels);

    s->nameId = a_str_dup(Path);

    a_pixels__commit(s->pixels);

    return s;
}

ASprite* a_sprite_newFromSprite(const ASprite* Sheet, int X, int Y)
{
    AVectorInt dim = a_pixels__boundsFind(Sheet->pixels, X, Y);

    return a_sprite_newFromSpriteEx(Sheet, X, Y, dim.x, dim.y);
}

ASprite* a_sprite_newFromSpriteEx(const ASprite* Sheet, int X, int Y, int W, int H)
{
    ASprite* s = spriteNew(a_pixels__new(W, H, true, true));

    const APixel* src = a_pixels__bufferGetFrom(Sheet->pixels, X, Y);
    APixel* dst = s->pixels->buffer;

    for(int i = H; i--; ) {
        memcpy(dst, src, (unsigned)W * sizeof(APixel));

        src += Sheet->pixels->w;
        dst += W;
    }

    a_pixels__commit(s->pixels);

    return s;
}

ASprite* a_sprite_newBlank(int Width, int Height, bool ColorKeyed)
{
    ASprite* s = spriteNew(a_pixels__new(Width, Height, true, true));

    if(ColorKeyed) {
        a_pixels__fill(s->pixels, a_sprite__colorKey);
    }

    a_pixels__commit(s->pixels);

    return s;
}

ASprite* a_sprite_dup(const ASprite* Sprite)
{
    ASprite* clone = spriteNew(a_pixels__dup(Sprite->pixels));

    a_pixels__commit(clone->pixels);

    #if !A_CONFIG_LIB_RENDER_SOFTWARE
        // Sprite's pixel buffer may be stale, blit the real texture
        a_color_push();
        a_screen_push(clone);

        a_color_reset();
        a_sprite_blit(Sprite, 0, 0);

        a_screen_pop();
        a_color_pop();
    #endif

    return clone;
}

void a_sprite_free(ASprite* Sprite)
{
    if(Sprite == NULL) {
        return;
    }

    a_pixels__free(Sprite->pixels);

    free(Sprite->nameId);
    free(Sprite);
}

void a_sprite_blit(const ASprite* Sprite, int X, int Y)
{
    a_platform_api__textureBlit(Sprite->pixels->texture,
                                X,
                                Y,
                                a__color.fillBlit);
}

void a_sprite_blitEx(const ASprite* Sprite, int X, int Y, AFix Scale, unsigned Angle, int CenterX, int CenterY)
{
    a_platform_api__textureBlitEx(Sprite->pixels->texture,
                                  X,
                                  Y,
                                  Scale,
                                  a_fix_angleWrap(Angle),
                                  CenterX,
                                  CenterY,
                                  a__color.fillBlit);
}

void a_sprite_swapColor(ASprite* Sprite, APixel OldColor, APixel NewColor)
{
    APixel* buffer = Sprite->pixels->buffer;

    for(size_t i = Sprite->pixels->bufferSize / sizeof(APixel); i--; ) {
        if(buffer[i] == OldColor) {
            buffer[i] = NewColor;
        }
    }

    a_pixels__commit(Sprite->pixels);
}

void a_sprite_swapColors(ASprite* Sprite, const APixel* OldColors, const APixel* NewColors, unsigned NumColors)
{
    APixel* buffer = Sprite->pixels->buffer;

    for(size_t i = Sprite->pixels->bufferSize / sizeof(APixel); i--; ) {
        const APixel pixel = buffer[i];

        for(unsigned c = NumColors; c--; ) {
            if(pixel == OldColors[c]) {
                buffer[i] = NewColors[c];
                break;
            }
        }
    }

    a_pixels__commit(Sprite->pixels);
}

AVectorInt a_sprite_sizeGet(const ASprite* Sprite)
{
    return (AVectorInt){Sprite->pixels->w, Sprite->pixels->h};
}

int a_sprite_sizeGetWidth(const ASprite* Sprite)
{
    return Sprite->pixels->w;
}

int a_sprite_sizeGetHeight(const ASprite* Sprite)
{
    return Sprite->pixels->h;
}

const APixel* a_sprite_pixelsGetBuffer(const ASprite* Sprite)
{
    return Sprite->pixels->buffer;
}

APixel a_sprite_pixelsGetPixel(const ASprite* Sprite, int X, int Y)
{
    return a_pixels__bufferGetAt(Sprite->pixels, X, Y);
}

APixel a_sprite_colorKeyGet(void)
{
    return a_sprite__colorKey;
}
