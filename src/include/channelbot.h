//
//
//  @ Project : ircppbot-channelbot
//  @ File Name : channelbot.h
//  @ Date : 08-01-2013
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

#ifndef SRC_INCLUDE_CHANNELBOT_H
#define SRC_INCLUDE_CHANNELBOT_H

#include <string>
#include <vector>
#include <map>

#include <atomic>
#include <thread>
#include <mutex>
#include <memory>

#include <ircppbot/moduleinterface.h>
#include <ircppbot/management.h>
#include <ircppbot/ircdata.h>

#include "cchannel.h"

class cchannel;
class ircdata;
class channelbot : public moduleinterface
{
    public:
        channelbot();
        ~channelbot();
        void read();
        void stop();
        void init();
        void timerrun(){}

    private:

        std::atomic<bool> m_Run;
        ircdata* m_IrcData;


        /*void join(std::vector< std::string > data);
        void part(std::vector< std::string > data);
        void kick(std::vector< std::string > data);
        void quit(std::vector< std::string > data);

        void nick(std::vector< std::string > data);
        void mode(std::vector< std::string > data);*/

        void parseModes();
        void parseEvents();
        void parsePrivmsg();

        std::shared_ptr< std::thread > m_ModesThread;
        std::shared_ptr< std::thread > m_EventsThread;
        std::shared_ptr< std::thread > m_PrivmsgThread;

        /*DataInterface* mpDataInterface;
        WhoisDataContainerInterface* mpWhoisDataContainerInterface;

        void parse_raw();
        void parse_privmsg();
        void WhoisLoop();
        void ParseData(std::vector< std::string > data);
        void ParsePrivmsg(std::string mNick, std::string command, std::string chan, std::vector< std::string > args, int chantrigger);
        void PING(std::vector<std::string> data);
        //void PRIVMSG(std::vector<std::string> data);
        void INVITE(std::vector< std::string > data);
        void WHOIS(std::vector< std::string > data);
        void JOIN(std::vector< std::string > data);
        void PART(std::vector< std::string > data);
        void KICK(std::vector< std::string > data);
        void NICK(std::vector< std::string > data);
        void MODE(std::vector< std::string > data);
        void QUIT(std::vector< std::string > data);

        void adduser(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int reqaccess, int miChannelAcess);
        void deluser(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int miChannelAcess);
        void changelevel(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int reqaccess, int miChannelAcess);
        void access(std::string chan, std::string mNick, std::string reqnick, std::string reqauth, int miChannelAcess);
        void myaccess(std::string mNick, std::string reqnick, std::string reqauth, int miChannelAcess);
        */
        void version(std::string target, std::string userName);

        void up(std::string channelName, std::string userName);
        void down(std::string channelName, std::string userName);

        /*void version(std::string chan, std::string mNick, std::string auth, int miChannelAcess);
        void uptime(std::string chan, std::string mNick, std::string auth, int miChannelAcess);
        void up(std::string chan, std::string mNick, std::string auth, int miChannelAcess);
        void down(std::string chan, std::string mNick, std::string auth, int miChannelAcess);
        void resync(std::string chan, std::string mNick, std::string auth, int miChannelAcess);
        void stats(std::string sNick, std::string sAuth, int miChannelAcess);
        void users(std::string chan, std::string mNick, std::string auth, int miChannelAcess);
        void readchannel(std::string chan, std::string nick, std::string auth, int miChannelAcess);
        void kickuser(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, std::string reason, int miChannelAcess);
        void InviteMe(std::string msChannel, std::string msNick, std::string msAuth, int miChannelAcess);

        void set(std::string msChannel, std::string msNick, std::string msAuth, int miChannelAcess);
        void set(std::string msChannel, std::string msNick, std::string msAuth, std::string msSetting, int miChannelAcess);
        void set(std::string msChannel, std::string msNick, std::string msAuth, std::string msSetting, std::string msValue, int miChannelAcess);

        void op(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int miChannelAcess);
        void deop(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int miChannelAcess);
        void voice(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int miChannelAcess);
        void devoice(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int miChannelAcess);

        void channelcommands(std::string msNick, std::string msAuth, int miChannelAcess);

        void OnUserJoin(std::string msChan, std::string msNick);

        //void DBreplyinit();

        void timerlong();
        std::vector< int > timer_sec;
        std::vector< std::string > timer_command;
        std::vector< int > timer_long_sec;
        std::vector< std::string > timer_long_command;
        int longtime;

*/
        void channelCommands(std::string, std::string);
        bool nickFromHostmask(std::string& data);
        bool deleteFirst(std::string& data, std::string character);

        std::shared_ptr<cchannel> addCchannel(std::string channelName);
        std::shared_ptr<cchannel> getCchannel(std::string channelName);

        std::map< std::string, std::shared_ptr<cchannel> > m_Cchannels;

        std::mutex m_CchannelsMutex;

};

#endif // SRC_INCLUDE_CHANNELBOT_H
