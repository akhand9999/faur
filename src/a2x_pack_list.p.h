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

#ifndef A2X_PACK_LIST_PH
#define A2X_PACK_LIST_PH

#include "a2x_app_includes.h"

typedef struct List List;
typedef struct ListNode ListNode;

extern List* a_list_new(void);
extern void a_list_free(List* const list, const bool freeContent);
extern void a_list_empty(List* const list, const bool freeContent);

extern ListNode* a_list_addFirst(List* const list, void* const content);
extern ListNode* a_list_addLast(List* const list, void* const content);

extern void a_list_remove(List* const list, const void* const v);
extern void* a_list_removeFirst(List* const list, const bool freeContent);
extern void* a_list_removeLast(List* const list, const bool freeContent);
extern void a_list_removeNode(ListNode* const node, const bool freeContent);

#define a_list_push(l, c) a_list_addFirst(l, c)
#define a_list_pop(l)     a_list_removeFirst(l, false)
#define a_list_peek(l)    a_list_first(l)

extern void a_list_reverse(List* const list);

extern void** a_list_getArray(List* const list);

extern void* a_list_first(const List* const list);
extern void* a_list_last(const List* const list);

extern void* a_list_get(const List* const list, const int index);

extern int a_list_size(const List* const list);
extern bool a_list_isEmpty(const List* const list);

#endif // A2X_PACK_LIST_PH
