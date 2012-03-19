//
//
//  @ Project : ircppbot ChannelBot
//  @ File Name : Channel.h
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



#ifndef SRC_INCLUDE_CHANNEL_H_
#define SRC_INCLUDE_CHANNEL_H_

#include "Auth.h"

#include <string>
#include <map>

class Auth;
class Channel
{
public:
    typedef std::map<std::string, std::string> SetMap;
    typedef std::map<std::string, Auth> AuthMap;

    Channel(std::string sChannelName, std::string sChannelUUID);
    ~Channel();

    bool AddAuth(std::string sAuthName, std::string sUserUUID);
    bool RemAuth(std::string sAuthName, std::string sUserUUID);
    bool ChangeAuth(std::string sAuthName, std::string sUserUUID);
    Auth* GetAuth(std::string sAuthName, std::string sUserUUID);


    // setters
    void SetChannelName(std::string sChannelName) { m_sChannelName = sChannelName; };
    void SetChannelUUID(std::string sChannelUUID) { m_sChannelUUID = sChannelUUID; };

    // getters
    std::string GetChannelName() { return m_sChannelName; };
    std::string GetChannelUUID() { return m_sChannelUUID; };

    bool GetStringSetting(const std::string sKey, std::string sRes, std::string sDefault);
    bool GetIntSetting(const std::string sKey, int iRes, int iDefault);
    bool GetBoolSetting(const std::string sKey, bool bRes, bool bDefault);

    bool SetStringSetting(const std::string sKey, std::string sValue, bool bUpdate);
    bool SetIntSetting(const std::string sKey, int iValue, bool bUpdate);
    bool SetBoolSetting(const std::string sKey, bool bValue, bool bUpdate);

private:

    std::string m_sChannelName;
    std::string m_sChannelUUID;

    SetMap m_Settings;
    AuthMap m_Auths;
};

#endif // SRC_INCLUDE_CHANNEL_H_
