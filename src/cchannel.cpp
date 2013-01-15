//
//
//  @ Project : channelbot
//  @ File Name : cchannel.cpp
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


#include "include/cchannel.h"
#include <gframe/output.h>

cchannel::cchannel() :
m_ChannelAuths(),
m_ChannelSettings(),
m_GlobalChannelSettings(),
m_ChannelCommandSettings(),
m_GlobalChannelCommandSettings()
{
    m_ChannelAuths.clear();
    m_ChannelSettings.clear();
    m_GlobalChannelSettings.clear();
    m_ChannelCommandSettings.clear();
    m_GlobalChannelCommandSettings.clear();

    m_GlobalChannelSettings.insert(std::pair< std::string, std::string >("giveops", "300"));
    m_GlobalChannelSettings.insert(std::pair< std::string, std::string >("givevoice", "100"));

}

cchannel::cchannel(const cchannel&)
{
}

cchannel::~cchannel()
{
}


std::shared_ptr<cauth> cchannel::addAuth(std::string authName)
{
    std::pair<std::map< std::string, std::shared_ptr<cauth> >::iterator, bool> ret;
    ret = m_ChannelAuths.insert ( std::pair< std::string, std::shared_ptr<cauth> >(authName, std::make_shared<cauth>()) );
    if (!ret.second)
    {
        return nullptr;
    }
    return ret.first->second;
}

bool cchannel::delAuth(std::string authName)
{
    if (m_ChannelAuths.erase(authName) >= 0)
    {
        return true;
    }
    return false;
}

std::shared_ptr<cauth> cchannel::getAuth(std::string authName)
{
    std::map< std::string, std::shared_ptr<cauth> >::iterator l_ChannelAuthsIterator;
    l_ChannelAuthsIterator = m_ChannelAuths.find(authName);
    if (l_ChannelAuthsIterator == m_ChannelAuths.end())
    {
        return nullptr;
    }
    return l_ChannelAuthsIterator->second;
}

std::map< std::string, std::shared_ptr<cauth> > cchannel::getAuths()
{
    return m_ChannelAuths;
}


void cchannel::setSetting(std::string settingName, std::string settingValue)
{
    m_ChannelSettings[settingName] = settingValue;
}

std::string cchannel::getSetting(std::string settingName)
{
    std::map< std::string, std::string >::iterator l_ChannelSettingsIterator;
    l_ChannelSettingsIterator = m_ChannelSettings.find(settingName);
    if (l_ChannelSettingsIterator == m_ChannelSettings.end())
    {
        return "";
    }
    return l_ChannelSettingsIterator->second;
}
std::string cchannel::getGlobalSetting(std::string settingName)
{
    std::map< std::string, std::string >::iterator l_ChannelSettingsIterator;
    l_ChannelSettingsIterator = m_GlobalChannelSettings.find(settingName);
    if (l_ChannelSettingsIterator == m_GlobalChannelSettings.end())
    {
        return "";
    }
    return l_ChannelSettingsIterator->second;
}

std::map< std::string, std::string > cchannel::getSettings()
{
    return m_ChannelSettings;
}


void cchannel::setCommandSetting(std::string settingName, std::string settingValue)
{
    m_ChannelCommandSettings[settingName] = settingValue;
}

std::string cchannel::getCommandSetting(std::string settingName)
{
    std::map< std::string, std::string >::iterator l_ChannelCommandSettingsIterator;
    l_ChannelCommandSettingsIterator = m_ChannelCommandSettings.find(settingName);
    if (l_ChannelCommandSettingsIterator == m_ChannelCommandSettings.end())
    {
        return "";
    }
    return l_ChannelCommandSettingsIterator->second;
}
std::string cchannel::getGlobalCommandSetting(std::string settingName)
{
    std::map< std::string, std::string >::iterator l_ChannelCommandSettingsIterator;
    l_ChannelCommandSettingsIterator = m_GlobalChannelSettings.find(settingName);
    if (l_ChannelCommandSettingsIterator == m_GlobalChannelSettings.end())
    {
        return "";
    }
    return l_ChannelCommandSettingsIterator->second;
}

std::map< std::string, std::string > cchannel::getCommandSettings()
{
    return m_ChannelCommandSettings;
}
