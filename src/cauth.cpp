//
//
//  @ Project : channelbot
//  @ File Name : cauth.cpp
//  @ Date : 15-01-2013
//  @ Author : Gijs Kwakkel
//
//
// Copyright (c) 2013 Gijs Kwakkel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//


#include "include/cauth.h"
#include <gframe/output.h>
#include <gframe/database.h>
#include <gframe/configreader.h>

cauth::cauth() :
m_Access(0),
m_NoAutoOp(false),
m_AutoInvite(false),
m_Info("")
{
}

cauth::cauth(const cauth&)
{
}

cauth::~cauth()
{
}


void cauth::setAccess(size_t access)
{
    m_Access = access;
}
size_t cauth::getAccess()
{
    return m_Access;
}

void cauth::setNoAutoOp(bool noAutoOp)
{
    m_NoAutoOp = noAutoOp;
}
bool cauth::getNoAutoOp()
{
    return m_NoAutoOp;
}

void cauth::setAutoInvite(bool autoInvite)
{
    m_AutoInvite = autoInvite;
}
bool cauth::getAutoInvite()
{
    return m_AutoInvite;
}

void cauth::setInfo(std::string info)
{
    m_Info = info;
}
std::string cauth::getInfo()
{
    return m_Info;
}
