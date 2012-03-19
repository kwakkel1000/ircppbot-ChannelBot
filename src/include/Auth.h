//
//
//  @ Project : ircppbot ChannelBot
//  @ File Name : Auth.h
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



#ifndef SRC_INCLUDE_AUTH_H_
#define SRC_INCLUDE_AUTH_H_

#include <string>
#include <map>

class Auth
{
public:
    typedef std::map<std::string, std::string> UsetMap;

    Auth(std::string sAuthName, std::string sUserUUID);
    ~Auth();


    // setters
    void SetAuthName(std::string sAuthName) { m_sAuthName = sAuthName; };
    void SetUserUUID(std::string sUserUUID) { m_sUserUUID = sUserUUID; };

    // getters
    std::string GetAuthName() { return m_sAuthName; };
    std::string GetUserUUID() { return m_sUserUUID; };


private:

    std::string m_sAuthName;
    std::string m_sUserUUID;

    UsetMap m_Settings;
};

#endif // SRC_INCLUDE_AUTH_H_

