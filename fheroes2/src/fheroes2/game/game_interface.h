/***************************************************************************
 *   Copyright (C) 2008 by Andrey Afletdinov                               *
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

#ifndef H2GAMEINTERFACE_H
#define H2GAMEINTERFACE_H

#include "gamedefs.h"
#include "button.h"
#include "game.h"

namespace Game
{
    class Interface
    {
    public:
    	static Interface & Get(void);

	void	Draw(void);
	u8	CountIcons(void);

	Button	buttonScrollHeroesUp;
	Button	buttonScrollCastleUp;
	Button	buttonNextHero;
	Button	buttonMovement;
	Button	buttonKingdom;
	Button	buttonSpell;
	Button	buttonEndTur;
	Button	buttonAdventure;
	Button	buttonFile;
	Button	buttonSystem;
	Button	buttonScrollHeroesDown;
	Button	buttonScrollCastleDown;

    private:
	Interface();
	
	u8 count_icons;
    };
};

#endif
