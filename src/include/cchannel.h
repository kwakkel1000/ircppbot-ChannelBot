//
//
//  @ Project : channelbot
//  @ File Name : cchannel.h
//  @ Date : 13-01-2013
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


#ifndef SRC_INCLUDE_CCHANNEL_H
#define SRC_INCLUDE_CCHANNEL_H
#include <string>
#include <map>
#include <set>
#include <atomic>
#include <mutex>
#include <memory>
#include <cstddef>

#include "cauth.h"

class cauth;
class cchannel
{
    public:
        cchannel();
        cchannel(const cchannel&);
        ~cchannel();

        // maybe
        //init();
        std::shared_ptr<cauth> addAuth(std::string authName);
        bool delAuth(std::string authName);
        std::shared_ptr<cauth> getAuth(std::string authName);
        std::map< std::string, std::shared_ptr<cauth> > getAuths();

        void setSetting(std::string settingName, std::string settingValue);
        std::string getSetting(std::string settingName);
        std::string getGlobalSetting(std::string settingName);
        std::map< std::string, std::string > getSettings();

        void setCommandSetting(std::string settingName, std::string settingValue);
        std::string getCommandSetting(std::string settingName);
        std::string getGlobalCommandSetting(std::string settingName);
        std::map< std::string, std::string > getCommandSettings();

    private:
        // authName, channelAccess
        std::map< std::string, std::shared_ptr<cauth> > m_ChannelAuths;
        // settingName, settingValue ("global" for default global value)
        std::map< std::string, std::string > m_ChannelSettings;
        std::map< std::string, std::string > m_GlobalChannelSettings;
        // settingName, settingValue ("global" for default global value)
        std::map< std::string, std::string > m_ChannelCommandSettings;
        std::map< std::string, std::string > m_GlobalChannelCommandSettings;

        std::mutex m_AuthsMutex;
        std::mutex m_SettingsMutex;
};
#endif // SRC_INCLUDE_CCHANNEL_H
