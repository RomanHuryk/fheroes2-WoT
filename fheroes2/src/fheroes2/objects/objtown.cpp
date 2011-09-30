/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "icn.h"
#include "direction.h"
#include "objtown.h"
#include "settings.h"

bool ObjTown::isPassable(u16 icn, u8 index, u16 direct)
{
    switch(icn)
    {
	case ICN::OBJNTWBA:
	    if(index > 10)
		index = index - 10 * (index / 10);
	     // 2, 12, 22, 32, 42, 52, 62, 72
	    if(2 == index)
		return Direction::UNKNOWN == direct || Direction::BOTTOM == direct;
	    else
	     // 7, 17, 27, 37, 47, 57, 67, 77
	    if(7 == index)
		return Direction::UNKNOWN == direct || (direct & (Direction::LEFT | Direction::RIGHT | DIRECTION_BOTTOM_ROW | Direction::TOP));
	    else
	    if(4 < index && index < 10)
		return Direction::UNKNOWN == direct || (direct & (Direction::LEFT | Direction::RIGHT | DIRECTION_BOTTOM_ROW));
	    else
            if(index < 5)
		return false;
	    else
		return true;
	    break;

	case ICN::OBJNTOWN:
	    if(13 == index || 29 == index || 45 == index || 61 == index ||
		77 == index || 93 == index || 109 == index || 125 == index ||
		141 == index || 157 == index || 173 == index || 189 == index)
		return Direction::UNKNOWN == direct || Direction::BOTTOM == direct;
	    else
	    // kngt
	    if((5 < index && index < 13) || (13 < index && index < 16) ||
		(21 < index && index < 29) || (29 < index && index < 32)) return false;
	    else
	    // barb
	    if((37 < index && index < 45) || (45 < index && index < 48) ||
		(53 < index && index < 61) || (61 < index && index < 64)) return false;
	    else
	    // sorc
	    if((69 < index && index < 77) || (77 < index && index < 80) ||
		(85 < index && index < 93) || (93 < index && index < 96)) return false;
	    else
	    // wrlk
	    if((101 < index && index < 109) || (109 < index && index < 112) ||
		(117 < index && index < 125) || (125 < index && index < 128)) return false;
	    else
	    // wzrd
	    if((133 < index && index < 141) || (141 < index && index < 144) ||
		(149 < index && index < 157) || (157 < index && index < 160)) return false;
	    else
	    // necr
	    if((165 < index && index < 173) || (173 < index && index < 176) ||
		(181 < index && index < 189) || (189 < index && index < 192)) return false;
	    else
		return true;

	default: break;
    }

    return false;
}
