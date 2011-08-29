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

#include "a2x_pack_menu.v.h"

typedef enum MenuState {
    A_MENU_RUNNING, A_MENU_ACCEPT, A_MENU_CANCEL, A_MENU_SPENT
} MenuState;

struct Menu {
    List* items;
    void (*freeItem)(void* v);
    int selectedIndex;
    void* selectedItem;
    MenuState state;
    int pause;
    bool used;
    void (*input)(struct Menu* const m, void* const v);
    void* v;
    char* title;
    Sprite* sprite;
    Sound* soundAccept;
    Sound* soundCancel;
    Sound* soundBrowse;
    Input* next;
    Input* back;
    Input* select;
    Input* cancel;
};

#define A_MENU_PAUSE (a2x_int("fps.rate") / 6)

Menu* a_menu_new(Input* const next, Input* const back, Input* const select, Input* const cancel, void (*freeItem)(void* v))
{
    Menu* const m = malloc(sizeof(Menu));

    m->items = a_list_new();
    m->freeItem = freeItem;

    m->selectedIndex = 0;
    m->selectedItem = NULL;

    m->state = A_MENU_RUNNING;
    m->pause = A_MENU_PAUSE;
    m->used = false;

    m->input = NULL;
    m->v = NULL;
    m->title = NULL;

    m->sprite = NULL;

    m->soundAccept = NULL;
    m->soundCancel = NULL;
    m->soundBrowse = NULL;

    m->next = next;
    m->back = back;
    m->select = select;
    m->cancel = cancel;

    return m;
}

void a_menu_free(Menu* const m)
{
    if(m->freeItem) {
        A_LIST_ITERATE(m->items, void, v) {
            m->freeItem(v);
        }
    }

    a_list_free(m->items, false);
    free(m);
}

void a_menu_addInput(Menu* const m, void (*input)(struct Menu* const m, void* const v))
{
    m->input = input;
}

void a_menu_addV(Menu* const m, void* const v)
{
    m->v = v;
}

void a_menu_addTitle(Menu* const m, const char* const t)
{
    m->title = a_str_dup(t);
}

void a_menu_addSprite(Menu* const m, Sprite* const s)
{
    m->sprite = s;
}

void a_menu_addSounds(Menu* const m, Sound* const accept, Sound* const cancel, Sound* const browse)
{
    m->soundAccept = accept;
    m->soundCancel = cancel;
    m->soundBrowse = browse;
}

void a_menu_addItem(Menu* const m, void* const v)
{
    a_list_addLast(m->items, v);

    if(m->selectedItem == NULL) {
        m->selectedItem = v;
    }
}

void a_menu_input(Menu* const m)
{
    if(!a_menu_running(m)) {
        m->state = A_MENU_SPENT;
        return;
    }

    m->used = false;

    if(!m->pause) {
        if(a_input_get(m->back)) {
            m->selectedIndex--;
            m->selectedItem = a_list_get(m->items, m->selectedIndex);
            m->used = true;
        } else if(a_input_get(m->next)) {
            m->selectedIndex++;
            m->selectedItem = a_list_get(m->items, m->selectedIndex);
            m->used = true;
        }
    } else {
        if(!a_input_get(m->back) && !a_input_get(m->next) && !a_input_get(m->select) && !(m->cancel && a_input_get(m->cancel))) {
            m->pause = 0;
        } else {
            m->pause--;
        }
    }

    if(m->used) {
        m->pause = A_MENU_PAUSE;

        if(m->soundBrowse) {
            a_sfx_play(m->soundBrowse);
        }

        if(m->selectedIndex < 0) {
            m->selectedIndex = a_list__size(m->items) - 1;
            m->selectedItem = a_list_last(m->items);
        } else if(m->selectedIndex == a_list__size(m->items)) {
            m->selectedIndex = 0;
            m->selectedItem = a_list_first(m->items);
        }
    } else {
        if(a_input_get(m->select)) {
            m->state = A_MENU_ACCEPT;

            if(m->soundAccept) {
                a_sfx_play(m->soundAccept);
            }
        } else if(m->cancel && a_input_get(m->cancel)) {
            m->state = A_MENU_CANCEL;

            if(m->soundCancel) {
                a_sfx_play(m->soundCancel);
            }
        }
    }

    if(m->input) {
        m->input(m, m->v);
    }

    if(m->state != A_MENU_RUNNING) {
        a_input_unpress(m->next);
        a_input_unpress(m->back);
        a_input_unpress(m->select);

        if(m->cancel) {
            a_input_unpress(m->cancel);
        }
    }
}

List* a_menu__items(const Menu* const m)
{
    return m->items;
}

bool a_menu_isSelected(const Menu* const m, const void* const item)
{
    return item == m->selectedItem;
}

void a_menu_keepRunning(Menu* const m)
{
    m->state = A_MENU_RUNNING;
}

bool a_menu_running(const Menu* const m)
{
    return m->state == A_MENU_RUNNING;
}

bool a_menu_finished(const Menu* const m)
{
    return m->state == A_MENU_ACCEPT || m->state == A_MENU_CANCEL;
}

bool a_menu_accept(const Menu* const m)
{
    return m->state == A_MENU_ACCEPT;
}

bool a_menu_cancel(const Menu* const m)
{
    return m->state == A_MENU_CANCEL;
}

int a_menu_choice(const Menu* const m)
{
    return m->selectedIndex;
}
