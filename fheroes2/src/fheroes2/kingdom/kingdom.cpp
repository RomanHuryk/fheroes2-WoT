/***************************************************************************
 *   Copyright (C) 2006 by Andrey Afletdinov                               *
 *   afletdinov@mail.dc.baikal.ru                                          *
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

#include <algorithm>

#include "config.h"
#include "castle.h"
#include "heroes.h"
#include "error.h"
#include "difficulty.h"
#include "game_statuswindow.h"
#include "payment.h"
#include "world.h"
#include "kingdom.h"

Kingdom::Kingdom(const Color::color_t cl, const Game::control_t con) : color(cl), control(con), play(cl & Settings::Get().FileInfo().KingdomColors() ? true : false)
{
    // set starting resource
    switch(Settings::Get().GameDifficulty())
    {
	case Difficulty::EASY:
	    resource.wood	= START_EASY_WOOD;
	    resource.ore	= START_EASY_ORE;
	    resource.mercury	= START_EASY_MERCURY;
	    resource.sulfur	= START_EASY_SULFUR;
	    resource.crystal	= START_EASY_CRYSTAL;
	    resource.gems	= START_EASY_GEMS;
	    resource.gold	= START_EASY_GOLD;
	    break;
	case Difficulty::NORMAL:
	    resource.wood	= START_NORMAL_WOOD;
	    resource.ore	= START_NORMAL_ORE;
	    resource.mercury	= START_NORMAL_MERCURY;
	    resource.sulfur	= START_NORMAL_SULFUR;
	    resource.crystal	= START_NORMAL_CRYSTAL;
	    resource.gems	= START_NORMAL_GEMS;
	    resource.gold	= START_NORMAL_GOLD;
	    break;
	case Difficulty::HARD:
	    resource.wood	= START_HARD_WOOD;
	    resource.ore	= START_HARD_ORE;
	    resource.mercury	= START_HARD_MERCURY;
	    resource.sulfur	= START_HARD_SULFUR;
	    resource.crystal	= START_HARD_CRYSTAL;
	    resource.gems	= START_HARD_GEMS;
	    resource.gold	= START_HARD_GOLD;
	    break;
	case Difficulty::EXPERT:
	    resource.wood	= START_EXPERT_WOOD;
	    resource.ore	= START_EXPERT_ORE;
	    resource.mercury	= START_EXPERT_MERCURY;
	    resource.sulfur	= START_EXPERT_SULFUR;
	    resource.crystal	= START_EXPERT_CRYSTAL;
	    resource.gems	= START_EXPERT_GEMS;
	    resource.gold	= START_EXPERT_GOLD;
	    break;
	case Difficulty::IMPOSSIBLE:
	    resource.wood	= START_IMPOSSIBLE_WOOD;
	    resource.ore	= START_IMPOSSIBLE_ORE;
	    resource.mercury	= START_IMPOSSIBLE_MERCURY;
	    resource.sulfur	= START_IMPOSSIBLE_SULFUR;
	    resource.crystal	= START_IMPOSSIBLE_CRYSTAL;
	    resource.gems	= START_IMPOSSIBLE_GEMS;
	    resource.gold	= START_IMPOSSIBLE_GOLD;
	    break;
    }

    heroes.reserve(KINGDOMMAXHEROES);
    castles.reserve(15);
}

void Kingdom::AITurns(void)
{
    Game::StatusWindow & status = Game::StatusWindow::Get();

    status.RedrawAITurns(color, 0);
    status.RedrawAITurns(color, 1);
    status.RedrawAITurns(color, 2);
    status.RedrawAITurns(color, 3);
    status.RedrawAITurns(color, 4);
    status.RedrawAITurns(color, 5);
    status.RedrawAITurns(color, 6);
    status.RedrawAITurns(color, 7);
    status.RedrawAITurns(color, 8);
    status.RedrawAITurns(color, 9);

    if(H2Config::Debug()) Error::Verbose("Kingdom::AITurns: " + Color::String(color) + " moved");
}

void Kingdom::ActionNewDay(void)
{
    // castle New Day
    std::vector<Castle *>::const_iterator itc = castles.begin();
    for(; itc != castles.end(); ++itc) if(*itc) (**itc).ActionNewDay();

    // heroes New Day
    std::vector<Heroes *>::const_iterator ith = heroes.begin();
    for(; ith != heroes.end(); ++ith) if(*ith) (**ith).ActionNewDay();

    // captured object
    resource.wood += DAY_PROFIT_WOOD * world.CountCapturedObject(MP2::OBJ_SAWMILL, color);
    resource.ore += DAY_PROFIT_ORE * world.CountCapturedMines(Resource::ORE, color);
    resource.mercury += DAY_PROFIT_MERCURY * world.CountCapturedObject(MP2::OBJ_ALCHEMYLAB, color);
    resource.sulfur += DAY_PROFIT_SULFUR * world.CountCapturedMines(Resource::SULFUR, color);
    resource.crystal += DAY_PROFIT_CRYSTAL * world.CountCapturedMines(Resource::CRYSTAL, color);
    resource.gems += DAY_PROFIT_GEMS * world.CountCapturedMines(Resource::GEMS, color);
    resource.gold += DAY_PROFIT_GOLD * world.CountCapturedMines(Resource::GOLD, color);

    // funds
    itc = castles.begin();
    for(; itc != castles.end(); ++itc)
    {
	if(*itc)
	{
	    const Castle & castle = **itc;

	    // castle or town profit
	    resource.gold += (castle.isCastle() ? INCOME_CASTLE_GOLD : INCOME_TOWN_GOLD);

	    // statue
	    resource.gold += (castle.isBuild(Castle::BUILD_STATUE) ? INCOME_STATUE_GOLD : 0);

	    // dungeon for warlock
	    resource.gold += (castle.isBuild(Castle::BUILD_SPEC) && Race::WRLK == castle.GetRace() ? INCOME_DUNGEON_GOLD : 0);
	}
    }

    ith = heroes.begin();
    for(; ith != heroes.end(); ++ith)
    {
	if(*ith)
	{
	    const Heroes & heroes = **ith;

	    if(heroes.GetArtifacts().size())
	    {
		std::vector<Artifact::artifact_t>::const_iterator it1 = heroes.GetArtifacts().begin();
		std::vector<Artifact::artifact_t>::const_iterator it2 = heroes.GetArtifacts().end();

		for(; it1 != it2; ++it1) switch(*it1)
		{
		    case Artifact::ENDLESS_SACK_GOLD:		resource.gold += INCOME_ENDLESS_SACK_GOLD; break;
		    case Artifact::ENDLESS_BAG_GOLD:		resource.gold += INCOME_ENDLESS_BAG_GOLD; break;
		    case Artifact::ENDLESS_PURSE_GOLD:		resource.gold += INCOME_ENDLESS_PURSE_GOLD; break;
		    case Artifact::ENDLESS_POUCH_SULFUR:	resource.sulfur += INCOME_ENDLESS_POUCH_SULFUR; break;
		    case Artifact::ENDLESS_VIAL_MERCURY:	resource.mercury += INCOME_ENDLESS_VIAL_MERCURY; break;
		    case Artifact::ENDLESS_POUCH_GEMS:		resource.gems += INCOME_ENDLESS_POUCH_GEMS; break;
		    case Artifact::ENDLESS_CORD_WOOD:		resource.wood += INCOME_ENDLESS_CORD_WOOD; break;
		    case Artifact::ENDLESS_CART_ORE:		resource.ore += INCOME_ENDLESS_CART_ORE; break;
		    case Artifact::ENDLESS_POUCH_CRYSTAL:	resource.crystal += INCOME_ENDLESS_POUCH_CRYSTAL; break;

		    default: break;
		}
	    }
	    
	    // estates skill bonus
	    switch(heroes.GetLevelSkill(Skill::Secondary::ESTATES))
	    {
		case Skill::Level::BASIC:	resource.gold += 100; break;
		case Skill::Level::ADVANCED:	resource.gold += 250; break;
		case Skill::Level::EXPERT:	resource.gold += 500; break;

		default: break;
	    }
	}
    }
}

void Kingdom::ActionNewWeek(void)
{

    // castle New Week
    std::vector<Castle *>::const_iterator itc = castles.begin();
    for(; itc != castles.end(); ++itc) if(*itc) (**itc).ActionNewWeek();

    // heroes New Week
    std::vector<Heroes *>::const_iterator ith = heroes.begin();
    for(; ith != heroes.end(); ++ith) if(*ith) (**ith).ActionNewWeek();

    // debug an gift
    if(H2Config::Debug() && color == Settings::Get().MyColor())
    {
	Error::Verbose("Kingdom::ActionNewWeek: for the best debugging, God has sent you a gift.");

	resource.wood += 20;
	resource.ore += 20;
	resource.sulfur += 10;
	resource.crystal += 10;
	resource.gems += 10;
	resource.mercury += 10;
	resource.gold += 5000;
    }
}

void Kingdom::ActionNewMonth(void)
{
    // castle New Month
    std::vector<Castle *>::const_iterator itc = castles.begin();
    for(; itc != castles.end(); ++itc) if(*itc) (**itc).ActionNewMonth();

    // heroes New Month
    std::vector<Heroes *>::const_iterator ith = heroes.begin();
    for(; ith != heroes.end(); ++ith) if(*ith) (**ith).ActionNewMonth();
}

void Kingdom::AddHeroes(const Heroes *hero)
{
    if(hero && heroes.end() == std::find(heroes.begin(), heroes.end(), hero))
	heroes.push_back(const_cast<Heroes *>(hero));
}

void Kingdom::RemoveHeroes(const Heroes *hero)
{
    //
    // TODO: for current week need move hero to tavern
    //

    if(hero && heroes.size())
	heroes.erase(std::find(heroes.begin(), heroes.end(), hero));

    if(heroes.empty() && castles.empty()) play = false;
}

void Kingdom::AddCastle(const Castle *castle)
{
    if(castle && castles.end() == std::find(castles.begin(), castles.end(), castle))
	castles.push_back(const_cast<Castle *>(castle));
}

void Kingdom::RemoveCastle(const Castle *castle)
{
    if(castle && castles.size())
	castles.erase(std::find(castles.begin(), castles.end(), castle));

    if(heroes.empty() && castles.empty()) play = false;
}

u8 Kingdom::GetCountCastle(void) const
{
    return std::count_if(castles.begin(), castles.end(), Castle::PredicateIsCastle);
}

u8 Kingdom::GetCountTown(void) const
{
    return std::count_if(castles.begin(), castles.end(), Castle::PredicateIsTown);
}

u8 Kingdom::GetCountMarketplace(void) const
{
    return std::count_if(castles.begin(), castles.end(), Castle::PredicateIsBuildMarketplace);
}

Race::race_t Kingdom::GetRace(void) const
{
    return Settings::Get().FileInfo().KingdomRace(color);
}

bool Kingdom::AllowPayment(const Resource::funds_t & funds) const
{
    return funds > resource ? false : true;
}

/* is visited cell */
bool Kingdom::isVisited(const Maps::Tiles & tile) const
{
    std::list<Visit::IndexObject>::const_iterator it1 = visit_object.begin();
    std::list<Visit::IndexObject>::const_iterator it2 = visit_object.end();

    const u16 & index = tile.GetIndex();
    const MP2::object_t & object = tile.GetObject();

    for(; it1 != it2; ++it1) if(index == (*it1).first && object == (*it1).second) return true;

    return false;
}

/* return true if object visited */
bool Kingdom::isVisited(const MP2::object_t & object) const
{
    std::list<Visit::IndexObject>::const_iterator it1 = visit_object.begin();
    std::list<Visit::IndexObject>::const_iterator it2 = visit_object.end();

    for(; it1 != it2; ++it1) if((*it1).second == object) return true;

    return false;
}

/* set visited cell */
void Kingdom::SetVisited(const u16 index, const MP2::object_t & object)
{
    const Maps::Tiles & tile = world.GetTiles(index);

    if(isVisited(tile)) return;

    const MP2::object_t obj = object != MP2::OBJ_ZERO ? object : tile.GetObject();

    if(MP2::OBJ_ZERO != obj) visit_object.push_front(Visit::IndexObject(index, obj));
}

bool Kingdom::HeroesMayStillMove(void) const
{
    std::vector<Heroes *>::const_iterator ith = heroes.begin();
    for(; ith != heroes.end(); ++ith) if(*ith && (**ith).MayStillMove()) return true;
    return false;
}
