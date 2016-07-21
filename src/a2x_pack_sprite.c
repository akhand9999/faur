/*
    Copyright 2010 Alex Margarit

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

#include "a2x_pack_sprite.v.h"

static AList* sprites;

void a_sprite__init(void)
{
    sprites = a_list_new();
}

void a_sprite__uninit(void)
{
    A_LIST_ITERATE(sprites, ASprite, s) {
        a_sprite__free(s);
    }

    a_list_free(sprites);
}

ASprite* a_sprite_fromFile(const char* Path)
{
    int w = 0;
    int h = 0;
    ASprite* s;
    APixel* pixels = NULL;

    a_png_readFile(Path, &pixels, &w, &h);

    if(pixels == NULL) {
        return NULL;
    }

    s = a_sprite_fromPixels(pixels, w, h);
    free(pixels);

    return s;
}

ASprite* a_sprite_fromData(const uint8_t* Data)
{
    int w;
    int h;
    ASprite* s;
    APixel* pixels = NULL;

    a_png_readMemory(Data, &pixels, &w, &h);

    if(pixels == NULL) {
        return NULL;
    }

    s = a_sprite_fromPixels(pixels, w, h);
    free(pixels);

    return s;
}

ASprite* a_sprite_fromPixels(APixel* Pixels, int Width, int Height)
{
    ASprite* s = a_sprite_blank(Width, Height);

    memcpy(s->data, Pixels, Width * Height * sizeof(APixel));
    a_sprite_refresh(s);

    return s;
}

ASprite* a_sprite_new(const ASprite* Sheet, int X, int Y)
{
    return a_sprite_zoomed(Sheet, X, Y, 1);
}

ASprite* a_sprite_zoomed(const ASprite* Sheet, int X, int Y, int Zoom)
{
    int w = 0;
    int h = 0;

    const int width = Sheet->w;
    const int height = Sheet->h;

    for(int sheetx = X; sheetx < width; sheetx++) {
        const APixel hPixel = a_sprite__getPixel(Sheet, sheetx, Y);

        // reached right edge
        if(hPixel == A_SPRITE_LIMIT || hPixel == A_SPRITE_END) {
            for(int sheety = Y; sheety < height; sheety++) {
                const APixel vPixel = a_sprite__getPixel(Sheet, X, sheety);

                // reached bottom edge
                if(vPixel == A_SPRITE_LIMIT) {
                    w = sheetx - X;
                    h = sheety - Y;
                    goto Done;
                }
            }
        }
    }

    Done:

    if(w == 0 || h == 0) {
        if(X == 0 && Y == 0) {
            // no boundary borders for full-image sprites
            w = width;
            h = height;
        } else {
            a_out__error("ASprite coords %d, %d are invalid", X, Y);
            return NULL;
        }
    }

    const int spritew = w * Zoom;
    const int spriteh = h * Zoom;

    ASprite* const s = a_sprite_blank(spritew, spriteh);

    const int wp = Sheet->w;
    const APixel* const src = Sheet->data;
    APixel* const dst = s->data;

    // put the pixels on

    for(int i = Y; i < Y + h; i++) {
        for(int j = X; j < X + w; j++) {
            const APixel pixel = *(src + i * wp + j);

            const int dy = (i - Y) * Zoom;
            const int dx = (j - X) * Zoom;

            for(int zy = Zoom; zy--; ) {
                for(int zx = Zoom; zx--; ) {
                    dst[(dy + zy) * spritew + dx + zx] = pixel;
                }
            }
        }
    }

    a_sprite_refresh(s);

    return s;
}

ASprite* a_sprite_blank(int Width, int Height)
{
    ASprite* const s = a_mem_malloc(sizeof(ASprite) + Width * Height * sizeof(APixel));

    s->w = Width;
    s->wLog2 = (int)log2f(Width);
    s->h = Height;
    s->alpha = 255;
    s->spans = NULL;
    s->spansSize = 0;

    for(int i = Width * Height; i--; ) {
        s->data[i] = A_SPRITE_TRANSPARENT;
    }

    s->node = a_list_addLast(sprites, s);

    return s;
}

void a_sprite_free(ASprite* Sprite)
{
    a_list_removeNode(Sprite->node);
    a_sprite__free(Sprite);
}

void a_sprite__free(ASprite* Sprite)
{
    free(Sprite->spans);
    free(Sprite);
}

int a_sprite_w(const ASprite* Sprite)
{
    return Sprite->w;
}

int a_sprite_wLog2(const ASprite* Sprite)
{
    return Sprite->wLog2;
}

int a_sprite_h(const ASprite* Sprite)
{
    return Sprite->h;
}

APixel* a_sprite_data(ASprite* Sprite)
{
    return Sprite->data;
}

uint8_t a_sprite_getAlpha(const ASprite* Sprite)
{
    return Sprite->alpha;
}

void a_sprite_setAlpha(ASprite* Sprite, uint8_t Alpha)
{
    Sprite->alpha = Alpha;
}

APixel a_sprite_getPixel(const ASprite* Sprite, int X, int Y)
{
    return *(Sprite->data + Y * Sprite->w + X);
}

void a_sprite_refresh(ASprite* Sprite)
{
    const int w = Sprite->w;
    const int h = Sprite->h;
    const APixel* const dst = Sprite->data;

    // Spans format:
    // [1 (draw) / 0 (transp)][[len]...][0 (end line)]

    unsigned int num = 0;
    const APixel* dest = dst;

    for(int y = h; y--; ) {
        num += 3; // transparency start + first len + end line
        dest++; // start from the second pixel in the line
        for(int x = w - 1; x--; dest++) {
            if((*dest == A_SPRITE_TRANSPARENT && *(dest - 1) != A_SPRITE_TRANSPARENT)
                || (*dest != A_SPRITE_TRANSPARENT && *(dest - 1) == A_SPRITE_TRANSPARENT)) {
                num++; // transparency change, add a len
            }
        }
    }

    const size_t newSpansSize = num * sizeof(uint16_t);

    if(Sprite->spansSize < newSpansSize) {
        free(Sprite->spans);
        Sprite->spans = a_mem_malloc(newSpansSize);
        Sprite->spansSize = newSpansSize;
    }

    uint16_t* spans = Sprite->spans;

    dest = dst;

    for(int y = h; y--; ) {
        *spans++ = *dest != A_SPRITE_TRANSPARENT; // transparency start
        dest++; // start from the second pixel in the line
        uint16_t len = 1;
        for(int x = 1; x < w; x++, dest++) {
            if((*dest == A_SPRITE_TRANSPARENT && *(dest - 1) != A_SPRITE_TRANSPARENT)
                || (*dest != A_SPRITE_TRANSPARENT && *(dest - 1) == A_SPRITE_TRANSPARENT)) {
                *spans++ = len; // record len
                len = 1;
            } else {
                len++;
            }
        }
        *spans++ = len; // record last len of line
        *spans++ = 0; // mark end of line
    }
}

ASprite* a_sprite_clone(const ASprite* Sprite)
{
    ASprite* const s = a_sprite_blank(Sprite->w, Sprite->h);

    s->alpha = Sprite->alpha;
    memcpy(s->data, Sprite->data, Sprite->w * Sprite->h * sizeof(APixel));
    a_sprite_refresh(s);

    return s;
}
