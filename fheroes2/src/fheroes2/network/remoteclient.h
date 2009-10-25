/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov                               *
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

#ifndef H2REMOTECLIENT_H
#define H2REMOTECLIENT_H

#ifdef WITH_NET

#include "gamedefs.h"
#include "thread.h"
#include "network.h"

class FH2Server;

class FH2RemoteClient : public FH2Client
{
public:
    FH2RemoteClient();

    int ConnectionChat(void);
    int StartGame(void);

    int Logout(void);
    void RunThread(void);
    void ShutdownThread(void);

    void MsgBroadcast(void);
    void MsgPing(bool);
    void MsgLogout(bool);
    bool MsgMapsInfoSet(bool);
    bool MsgMapsInfoGet(bool);
    bool MsgMapsListGet(bool);
    bool MsgPlayersGet(bool);

    static int callbackCreateThread(void *);

    Thread thread;
};

#endif
#endif
