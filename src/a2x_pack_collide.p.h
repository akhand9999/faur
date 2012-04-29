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

#ifndef A2X_PACK_COLLIDE_PH
#define A2X_PACK_COLLIDE_PH

#include "a2x_app_includes.h"

typedef struct ColMap ColMap;
typedef struct ColPoint ColPoint;
typedef struct ColIt ColIt;

#include "a2x_pack_fix.p.h"
#include "a2x_pack_listit.p.h"

struct ColIt {
    ColPoint* callerPoint;
    ListIt points; // list of points in the current submap
};

extern ColMap* a_colmap_new(int totalWidth, int totalHeight, int gridDim);
extern void a_colmap_free(ColMap* c);

extern ColPoint* a_colpoint_new(ColMap* colmap);
extern void a_colpoint_free(ColPoint* c);

extern void a_colpoint_setCoords(ColPoint* b, fix8 x, fix8 y);
extern void a_colpoint_setParent(ColPoint* b, void* parent);
extern void* a_colpoint_getParent(ColPoint* b);

extern ColIt a_colit__new(ColPoint* b);
extern bool a_colit__next(ColIt* it);
extern ColPoint* a_colit__get(ColIt* it);

#define A_COL_ITERATE(colpoint, var)                                                       \
    for(ColIt a__ci = a_colit__new(colpoint); a__ci.callerPoint; a__ci.callerPoint = NULL) \
        for(ColPoint* var; a_colit__next(&a__ci) && (var = a_colit__get(&a__ci)); )

#define a_collide_boxes(x1, y1, w1, h1, x2, y2, w2, h2) \
(                                                       \
    !( (y1) >= (y2) + (h2)                              \
    || (y2) >= (y1) + (h1)                              \
    || (x1) >= (x2) + (w2)                              \
    || (x2) >= (x1) + (w1) )                            \
)

#define a_collide_boxOnScreen(x, y, w, h)		            	 \
(                      						                     \
    a_collide_boxes((x), (y), (w), (h), 0, 0, a_width, a_height) \
)

extern bool a_collide_circles(int x1, int y1, int r1, int x2, int y2, int r2);

#endif // A2X_PACK_COLLIDE_PH
