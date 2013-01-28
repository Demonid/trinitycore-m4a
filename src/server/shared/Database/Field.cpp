/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.hpp"
#include "Field.h"
#include <new>

Field::Field()
{
    data.value = NULL;
    data.capacity = 0;
    data.type = MYSQL_TYPE_NULL;
    data.length = 0;
}

Field::~Field()
{
    CleanUp();
}

void Field::SetByteValue(const void* newValue, const size_t newSize, enum_field_types newType, uint32 length)
{
    // This value stores raw bytes that have to be explicitly casted later
    if (newValue)
    {
        if (data.capacity < newSize)
        {
            if (auto ptr = std::realloc(data.value, newSize))
                data.value = ptr;
            else
                throw std::bad_alloc();
            data.capacity = newSize;
        }
        memcpy(data.value, newValue, newSize);
        data.length = length;
    }
    else
        CleanUp();
    data.type = newType;
    data.raw = true;
}

void Field::SetStructuredValue(char* newValue, enum_field_types newType)
{
    // This value stores somewhat structured data that needs function style casting
    if (newValue)
    {
        size_t size = strlen(newValue);
        if (data.capacity < size+1)
        {
            if (auto ptr = std::realloc(data.value, size+1))
                data.value = ptr;
            else
                throw std::bad_alloc();
            data.capacity = size+1;
        }
        memcpy(data.value, newValue, size+1);
        data.length = size;
    }
    else
        CleanUp();

    data.type = newType;
    data.raw = false;
}
