/*
    freeHeroes2 engine
    turn-based game engine (clone of Heroes Of the Might and Magic II)
    Copyright (C) 2006

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    3DO, New World Computing, Heroes of Might and Magic, and the related
    3DO logos are trademarks and or registered trademarks of 3DO Company.
    All other trademarks belong to their respective owners.

    Web: http://sourceforge.net/projects/fheroes2

    Description:
*/

#include "SDL.h"
#include "gamedefs.h"
#include "string.h"
#include "config.h"
#include "heroes.h"
#include "kingdom.h"


static S_KINGDOM *kingdom = NULL;

BOOL	InitKingdom(void){

    if(NULL == (kingdom = (S_KINGDOM *) malloc(sizeof(S_KINGDOM) * KINGDOMMAX))){
	fprintf(stderr, "InitKingdom: error malloc: %d\n", sizeof(S_KINGDOM) * KINGDOMMAX);
	return FALSE;
    }
    
    memset(kingdom, 0, sizeof(S_KINGDOM) * KINGDOMMAX);

    kingdom[GRAY].play = TRUE;
    kingdom[GRAY].allowBuild = FALSE;
    kingdom[GRAY].ore = 0;
    kingdom[GRAY].wood = 0;
    kingdom[GRAY].mercury = 0;
    kingdom[GRAY].sulfur = 0;
    kingdom[GRAY].crystal = 0;
    kingdom[GRAY].gems = 0;
    kingdom[GRAY].gold = 0;

    if(GetIntValue(KINGDOMCOLORS) & COLORBLUE){
	kingdom[BLUE].play = TRUE;
	kingdom[BLUE].allowBuild = TRUE;
	kingdom[BLUE].ore = 20;
	kingdom[BLUE].wood = 20;
	kingdom[BLUE].mercury = 5;
	kingdom[BLUE].sulfur = 5;
	kingdom[BLUE].crystal = 5;
	kingdom[BLUE].gems = 5;
	kingdom[BLUE].gold = 7500;
    }

    if(GetIntValue(KINGDOMCOLORS) & COLORGREEN){
	kingdom[GREEN].play = TRUE;
	kingdom[GREEN].allowBuild = TRUE;
	kingdom[GREEN].ore = 20;
	kingdom[GREEN].wood = 20;
	kingdom[GREEN].mercury = 5;
	kingdom[GREEN].sulfur = 5;
	kingdom[GREEN].crystal = 5;
	kingdom[GREEN].gems = 5;
	kingdom[GREEN].gold = 7500;
    }

    if(GetIntValue(KINGDOMCOLORS) & COLORRED){
        kingdom[RED].play = TRUE;
        kingdom[RED].allowBuild = TRUE;
	kingdom[RED].ore = 20;
	kingdom[RED].wood = 20;
	kingdom[RED].mercury = 5;
	kingdom[RED].sulfur = 5;
	kingdom[RED].crystal = 5;
	kingdom[RED].gems = 5;
	kingdom[RED].gold = 7500;
    }

    if(GetIntValue(KINGDOMCOLORS) & COLORYELLOW){
	kingdom[YELLOW].play = TRUE;
	kingdom[YELLOW].allowBuild = TRUE;
	kingdom[YELLOW].ore = 20;
	kingdom[YELLOW].wood = 20;
	kingdom[YELLOW].mercury = 5;
	kingdom[YELLOW].sulfur = 5;
	kingdom[YELLOW].crystal = 5;
	kingdom[YELLOW].gems = 5;
	kingdom[YELLOW].gold = 7500;
    }

    if(GetIntValue(KINGDOMCOLORS) & COLORORANGE){
	kingdom[ORANGE].play = TRUE;
	kingdom[ORANGE].allowBuild = TRUE;
	kingdom[ORANGE].ore = 20;
	kingdom[ORANGE].wood = 20;
	kingdom[ORANGE].mercury = 5;
	kingdom[ORANGE].sulfur = 5;
	kingdom[ORANGE].crystal = 5;
	kingdom[ORANGE].gems = 5;
	kingdom[ORANGE].gold = 7500;
    }

    if(GetIntValue(KINGDOMCOLORS) & COLORPURPLE){
	kingdom[PURPLE].play = TRUE;
	kingdom[PURPLE].allowBuild = TRUE;
	kingdom[PURPLE].ore = 20;
	kingdom[PURPLE].wood = 20;
	kingdom[PURPLE].mercury = 5;
	kingdom[PURPLE].sulfur = 5;
	kingdom[PURPLE].crystal = 5;
	kingdom[PURPLE].gems = 5;
	kingdom[PURPLE].gold = 7500;
    }

    if(GetIntValue(DEBUG)){
	fprintf(stderr, "InitKingdom:");
	if(GetIntValue(KINGDOMCOLORS) & COLORBLUE)   fprintf(stderr, " BLUE");
	if(GetIntValue(KINGDOMCOLORS) & COLORGREEN)  fprintf(stderr, " GREEN");
	if(GetIntValue(KINGDOMCOLORS) & COLORRED)    fprintf(stderr, " RED");
	if(GetIntValue(KINGDOMCOLORS) & COLORYELLOW) fprintf(stderr, " YELLOW");
	if(GetIntValue(KINGDOMCOLORS) & COLORORANGE) fprintf(stderr, " ORANGE");
	if(GetIntValue(KINGDOMCOLORS) & COLORPURPLE) fprintf(stderr, " PURPLE");
	if(! GetIntValue(KINGDOMCOLORS)) fprintf(stderr, " (test mode?)");
	fprintf(stderr, "\n");

	if(GetIntValue(HUMANCOLORS) == BLUE)   	    fprintf(stderr, "Human Kingdom: BLUE\n");
	else if(GetIntValue(HUMANCOLORS) == GREEN)  fprintf(stderr, "Human Kingdom: GREEN\n");
	else if(GetIntValue(HUMANCOLORS) == RED)    fprintf(stderr, "Human Kingdom: RED\n");
	else if(GetIntValue(HUMANCOLORS) == YELLOW) fprintf(stderr, "Human Kingdom: YELLOW\n");
	else if(GetIntValue(HUMANCOLORS) == ORANGE) fprintf(stderr, "Human Kingdom: ORANGE\n");
	else if(GetIntValue(HUMANCOLORS) == PURPLE) fprintf(stderr, "Human Kingdom: PURPLE\n");
    }

    return TRUE;
}

void	FreeKingdom(void){

    if(kingdom) free(kingdom);
}

S_KINGDOM *GetStatKingdom(E_COLORS color){

    if(! kingdom) return NULL;
    
    return &kingdom[color];
}

void    RecalculateKingdomDay(E_COLORS color){

    if(! kingdom[color].play) return;
    
    S_CASTLE *castle = GetFirstCastle(color);

    while(castle){
    
	// gold
	if(castle->castle)
	    kingdom[color].gold += GOLD_CASTLE_DAY;
	else
	    kingdom[color].gold += GOLD_TOWN_DAY;

	if(castle->building & BUILD_STATUE)
	    kingdom[color].gold += GOLD_STATUE_DAY;
	
	if(WARLOCK == castle->race && castle->building & BUILD_SPEC)
	    kingdom[color].gold += GOLD_DUNGEON_DAY;

	castle = GetNextCastle(color);
    }

    //
    // MINES gold ore wood sulfur crystal mercury gems
    //
    // HeroesLevelSkill(S_HEROES *, E_SKILL)
    // ESTATES +100 +250 +500
    //
    // HeroesArtifactPresent(S_HEROES *, E_ARTIFACT)
    // ENDLESS_SACK_GOLD	+1000 gold
    // ENDLESS_BAG_GOLD		+750 gold
    // ENDLESS_PURSE_GOLD	+500 gold
    // ENDLESS_POUCH_SULFUR	+1 sulfur
    // ENDLESS_VIAL_MERCURY	+1 mercury
    // ENDLESS_POUCH_GEMS	+1 gems
    // ENDLESS_CORD_WOOD	+1 wood
    // ENDLESS_CART_ORE		+1 ore
    // ENDLESS_POUCH_CRYSTAL	+1 crystal
    // TAX_LIEN			-250 gold

    KingdomSetAllowBuild(color, TRUE);
}

BOOL KingdomAllowBuild(E_COLORS color){

    if(! kingdom) return FALSE;
    
    return kingdom[color].allowBuild;
}

void KingdomSetAllowBuild(E_COLORS color, BOOL allow){

    kingdom[color].allowBuild = allow;
}

void KingdomWasteResource(E_COLORS color, Uint16 gold, Uint8 wood, Uint8 ore, Uint8 mercury, Uint8 crystal, Uint8 sulfur , Uint8 gems){

    if(gold){

	if(kingdom[color].gold < gold){

	    if(GetIntValue(DEBUG)) fprintf(stderr, "KingdomWasteResource: incorrect count: %d, gold: 0\n", gold);
	    kingdom[color].gold = 0;

	}else
	    kingdom[color].gold -= gold;
    }

    if(wood){
    
	if(kingdom[color].wood < wood){

	    if(GetIntValue(DEBUG)) fprintf(stderr, "KingdomWasteResource: incorrect count: %d, wood: 0\n", wood);
	    kingdom[color].wood = 0;

	}else
	    kingdom[color].wood -= wood;
    }

    if(ore){
    
	if(kingdom[color].ore < ore){

		if(GetIntValue(DEBUG)) fprintf(stderr, "KingdomWasteResource: incorrect count: %d, ore: 0\n", ore);
		kingdom[color].ore = 0;
	}else
	    kingdom[color].ore -= ore;
    }

    if(mercury){
    
	if(kingdom[color].mercury < mercury){

		if(GetIntValue(DEBUG)) fprintf(stderr, "KingdomWasteResource: incorrect count: %d, mercury: 0\n", mercury);
		kingdom[color].mercury = 0;
	}else
	    kingdom[color].mercury -= mercury;
    }

    if(crystal){
    
	if(kingdom[color].crystal < crystal){

		if(GetIntValue(DEBUG)) fprintf(stderr, "KingdomWasteResource: incorrect count: %d, crystal: 0\n", crystal);
		kingdom[color].crystal = 0;
	}else
	    kingdom[color].crystal -= crystal;
    }

    if(sulfur){
    
	if(kingdom[color].sulfur < sulfur){

		if(GetIntValue(DEBUG)) fprintf(stderr, "KingdomWasteResource: incorrect count: %d, sulfur: 0\n", sulfur);
		kingdom[color].sulfur = 0;
	}else
	    kingdom[color].sulfur -= sulfur;
    }

    if(gems){
    
	if(kingdom[color].gems < gems){

		if(GetIntValue(DEBUG)) fprintf(stderr, "KingdomWasteResource: incorrect count: %d, gems: 0\n", gems);
		kingdom[color].gems = 0;
	}else
	    kingdom[color].gems -= gems;
    }

}
