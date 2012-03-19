//
//
//  @ Project : ircppbot ChannelBot
//  @ File Name : Channel.cpp
//  @ Date : 4/18/2011
//  @ Author : Gijs Kwakkel
//
//
// Copyright (c) 2011 Gijs Kwakkel
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


#include "include/Channel.h"
#include <core/BotLib.h>
#include <core/Global.h>
#include <core/Output.h>
#include <core/DatabaseData.h>
#include <boost/algorithm/string.hpp>

/**
 * Constructor
 * @param sChannelName name of the channel
 * @param sChannelUUID UUID of the channel
 *
 */
Channel::Channel(std::string sChannelName, std::string sChannelUUID)
{
    SetChannelName(sChannelName);
    SetChannelUUID(sChannelUUID);
}

/**
 * Destructor
 *
 */
Channel::~Channel()
{
}



/**
 * GetStringSetting
 * @param sKey  the key to search for
 * @param sRes  the result string
 * @param sDefault  the default result when error
 * @return true for success, false for error.
 *
 */
bool Channel::GetStringSetting(const std::string sKey, std::string sRes, std::string sDefault = "")
{
    SetMap::iterator SetMapIterator = m_Settings.find(sKey);
    sRes = sDefault;
    if (SetMapIterator == m_Settings.end() || SetMapIterator->second.empty())
        return false;
    sRes = SetMapIterator->second;
    return true;
}

/**
 * GetIntSetting
 * @param sKey  the key to search for
 * @param iRes  the result integer
 * @param iDefault  the default result when error
 * @return true for success, false for error.
 *
 */
bool Channel::GetIntSetting(const std::string sKey, int iRes, int iDefault = 0)
{
    std::string _sTempRes;
    if (GetStringSetting(sKey, _sTempRes))
    {
        iRes = BotLib::IntFromString(_sTempRes);
        return true;
    }
    iRes = iDefault;
    return false;
}

/**
 * GetBoolSetting
 * @param sKey  the key to search for
 * @param bRes  the result bool
 * @param bDefault  the default result when error
 * @return true for success, false for error.
 *
 */
bool Channel::GetBoolSetting(const std::string sKey, bool bRes, bool bDefault = false)
{
    std::string _sTempRes;
    if (GetStringSetting(sKey, _sTempRes))
    {
        //bRes = BotLib::BoolFromString(_sTempRes);  // not implemented yet
        return true;
    }
    bRes = bDefault;
    return false;
}

/**
 * SetStringSetting
 * @param sKey  the key to set
 * @param sValue  the value to set the key
 * @param bUpdate  update database
 * @return true for success, false for error.
 *
 */
bool Channel::SetStringSetting(const std::string sKey, std::string sValue, bool bUpdate = false)
{
    if (bUpdate)
    {
        DatabaseData::Instance().UpdateData("channels", sKey, sValue, "`ChannelUuid` = '" + m_sChannelUUID +"'");
    }
    m_Settings[sKey] = sValue;
    return true;
}

/**
 * SetIntSetting
 * @param sKey  the key to set
 * @param iValue  the value to set the key
 * @param bUpdate  update database
 * @return true for success, false for error.
 *
 */
bool Channel::SetIntSetting(const std::string sKey, int iValue, bool bUpdate = false)
{
    if (SetStringSetting(sKey, BotLib::StringFromInt(iValue), bUpdate))
    {
        return true;
    }
    return false;
}

/**
 * SetBoolSetting
 * @param sKey  the key to set
 * @param bValue  the value to set the key
 * @param bUpdate  update database
 * @return true for success, false for error.
 *
 */
bool Channel::SetBoolSetting(const std::string sKey, bool bValue, bool bUpdate = false)
{
    //if (SetStringSetting(sKey, BotLib::StringFromBool(bValue), bUpdate))
    {
        return true;
    }
    return false;
}
