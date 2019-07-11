/*
    Copyright 2016-2019 Alex Margarit <alex@alxm.org>
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

#include <a2x.v.h>

static AList* g_lists[A_ECS__NUM]; // Each entity is in exactly one of these
static bool g_deleting; // Set at uninit time to prevent using freed entities
static ACollection* g_collection; // New entities are added to this collection

static void a_ecs__init(void)
{
    for(int i = A_ECS__NUM; i--; ) {
        g_lists[i] = a_list_new();
    }

    a_component__init();
    a_template__init();
}

static void a_ecs__uninit(void)
{
    g_deleting = true;

    for(int i = A_ECS__NUM; i--; ) {
        a_list_freeEx(g_lists[i], (AFree*)a_entity__free);
    }

    a_template__uninit();
    a_system__uninit();
    a_component__uninit();
}

const APack a_pack__ecs = {
    "ECS",
    {
        [0] = a_ecs__init,
    },
    {
        [0] = a_ecs__uninit,
    },
};

ACollection* a_ecs_collectionGet(void)
{
    return g_collection;
}

void a_ecs_collectionSet(ACollection* Collection)
{
    g_collection = Collection;
}

bool a_ecs__isDeleting(void)
{
    return g_deleting;
}

void a_ecs__tick(void)
{
    a_ecs__flushEntitiesFromSystems();

    // Check what systems the new entities match
    A_LIST_ITERATE(g_lists[A_ECS__NEW], AEntity*, e) {
        for(int s = A_CONFIG_ECS_SYS_NUM; s--; ) {
            a_entity__systemMatch(e, a_system__get(s, __func__));
        }

        a_ecs__entityAddToList(e, A_ECS__RESTORE);
    }

    // Add entities to the systems they match
    A_LIST_ITERATE(g_lists[A_ECS__RESTORE], AEntity*, e) {
        a_entity__systemsAddTo(e);
        a_ecs__entityAddToList(e, A_ECS__DEFAULT);
    }

    a_list_clear(g_lists[A_ECS__NEW]);
    a_list_clear(g_lists[A_ECS__RESTORE]);
    a_list_clearEx(g_lists[A_ECS__REMOVED_FREE], (AFree*)a_entity__free);
}

bool a_ecs__entityIsInList(const AEntity* Entity, AEcsListId List)
{
    return a_entity__ecsListGet(Entity) == g_lists[List];
}

void a_ecs__entityAddToList(AEntity* Entity, AEcsListId List)
{
    a_entity__ecsListAdd(Entity, g_lists[List]);
}

void a_ecs__entityMoveToList(AEntity* Entity, AEcsListId List)
{
    a_entity__ecsListMove(Entity, g_lists[List]);
}

void a_ecs__flushEntitiesFromSystems(void)
{
    A_LIST_ITERATE(g_lists[A_ECS__MUTED_QUEUE], AEntity*, e) {
        a_entity__systemsRemoveFromAll(e);

        a_ecs__entityAddToList(e, A_ECS__DEFAULT);
    }

    A_LIST_ITERATE(g_lists[A_ECS__REMOVED_QUEUE], AEntity*, e) {
        a_entity__systemsRemoveFromAll(e);

        if(a_entity__refGet(e) == 0) {
            a_ecs__entityAddToList(e, A_ECS__REMOVED_FREE);
        } else {
            a_ecs__entityAddToList(e, A_ECS__REMOVED_LIMBO);
        }
    }

    a_list_clear(g_lists[A_ECS__MUTED_QUEUE]);
    a_list_clear(g_lists[A_ECS__REMOVED_QUEUE]);
}