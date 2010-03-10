/*************************************************************************** 
 *   Copyright (C) 2010 by Andrey Afletdinov <fheroes2@gmail.com>          *
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

#include "monster.h"
#include "dialog.h"
#include "world.h"
#include "spell.h"
#include "settings.h"
#include "heroes.h"

bool ActionSpellViewMines(Heroes &);
bool ActionSpellViewResources(Heroes &);
bool ActionSpellViewArtifacts(Heroes &);
bool ActionSpellViewTowns(Heroes &);
bool ActionSpellViewHeroes(Heroes &);
bool ActionSpellViewAll(Heroes &);
bool ActionSpellIdentifyHero(Heroes &);
bool ActionSpellSummonBoat(Heroes &);
bool ActionSpellDimensionDoor(Heroes &);
bool ActionSpellTownGate(Heroes &);
bool ActionSpellTownPortal(Heroes &);
bool ActionSpellVisions(Heroes &);
bool ActionSpellSetGuardian(Heroes &, Monster::monster_t);

bool Heroes::ActionSpellCast(Spell::spell_t spell)
{
    DEBUG(DBG_GAME, DBG_INFO, "ActionSpell: " << GetName() << " cast spell: " << Spell::GetName(spell));

    switch(spell)
    {
	case Spell::VIEWMINES:		return ActionSpellViewMines(*this);
	case Spell::VIEWRESOURCES:	return ActionSpellViewResources(*this);
	case Spell::VIEWARTIFACTS:	return ActionSpellViewArtifacts(*this);
	case Spell::VIEWTOWNS:		return ActionSpellViewTowns(*this);
	case Spell::VIEWHEROES:		return ActionSpellViewHeroes(*this);
	case Spell::VIEWALL:		return ActionSpellViewAll(*this);
	case Spell::IDENTIFYHERO:	return ActionSpellIdentifyHero(*this);
	case Spell::SUMMONBOAT:		return ActionSpellSummonBoat(*this);
	case Spell::DIMENSIONDOOR:	return ActionSpellDimensionDoor(*this);
	case Spell::TOWNGATE:		return ActionSpellTownGate(*this);
	case Spell::TOWNPORTAL:		return ActionSpellTownPortal(*this);
	case Spell::VISIONS:		return ActionSpellVisions(*this);
	case Spell::HAUNT:		return ActionSpellSetGuardian(*this, Monster::GHOST);
	case Spell::SETEGUARDIAN:	return ActionSpellSetGuardian(*this, Monster::EARTH_ELEMENT);
	case Spell::SETAGUARDIAN:	return ActionSpellSetGuardian(*this, Monster::AIR_ELEMENT);
	case Spell::SETFGUARDIAN:	return ActionSpellSetGuardian(*this, Monster::FIRE_ELEMENT);
	case Spell::SETWGUARDIAN:	return ActionSpellSetGuardian(*this, Monster::WATER_ELEMENT);
	default: break;
    }
    return false;
}

bool ActionSpellViewMines(Heroes & hero)
{
    return false;
}

bool ActionSpellViewResources(Heroes & hero)
{
    return false;
}

bool ActionSpellViewArtifacts(Heroes & hero)
{
    return false;
}

bool ActionSpellViewTowns(Heroes & hero)
{
    return false;
}

bool ActionSpellViewHeroes(Heroes & hero)
{
    return false;
}

bool ActionSpellViewAll(Heroes & hero)
{
    return false;
}

bool ActionSpellIdentifyHero(Heroes & hero)
{
    return false;
}

bool ActionSpellSummonBoat(Heroes & hero)
{
    u8 chance = 0;

    switch(hero.GetLevelSkill(Skill::Secondary::WISDOM))
    {
	case Skill::Level::BASIC:	chance = 50; break;
	case Skill::Level::ADVANCED:	chance = 75; break;
	case Skill::Level::EXPERT:	chance = 100; break;
	default: chance = 30; break;
    }

    const Spell::spell_t spell = Spell::SUMMONBOAT;
    const u8 cost = Spell::CostManaPoints(spell, &hero);
    const u16 points = hero.GetSpellPoints();
    const u16 center = hero.GetIndex();

    // apply cast
    if(points >= cost)
    {
    	hero.SetSpellPoints(points - cost);

	// find water
	std::vector<u16> v;
	std::vector<u16>::const_iterator dst;
	if(Maps::ScanAroundObject(center, MP2::OBJ_ZERO, true, v))
        {
            dst = v.begin();
            // find_if water
            for(; dst != v.end(); ++dst) if(Maps::Ground::WATER == world.GetTiles(*dst).GetGround()) break;
        }

	// find boat
	const u16 src = world.GetNearestObject(center, MP2::OBJ_BOAT);

	if(Rand::Get(1, 100) <= chance && MAXU16 != src && dst != v.end())
        {
	    world.GetTiles(src).SetObject(MP2::OBJ_ZERO);
	    world.GetTiles(*dst).SetObject(MP2::OBJ_BOAT);
        }
	else
	// failed
	{
	    std::string str = "%{spell} failed!!!";
	    String::Replace(str, "%{spell}", Spell::GetName(spell));
	    Dialog::Message("", str, Font::BIG, Dialog::OK);
	}
    }

    return false;
}

bool ActionSpellDimensionDoor(Heroes & hero)
{
    return false;
}

bool ActionSpellTownGate(Heroes & hero)
{
    return false;
}

bool ActionSpellTownPortal(Heroes & hero)
{
    return false;
}

bool ActionSpellVisions(Heroes & hero)
{
    return false;
}

bool ActionSpellSetGuardian(Heroes & hero, Monster::monster_t m)
{
    switch(m)
    {
	case Monster::GHOST:
	case Monster::EARTH_ELEMENT:
	case Monster::AIR_ELEMENT:
	case Monster::FIRE_ELEMENT:
	case Monster::WATER_ELEMENT:
	default: break;
    }

    return false;
}
