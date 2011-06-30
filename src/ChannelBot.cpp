//
//
//  @ Project : ircppbot ChannelBot
//  @ File Name : ChannelBot.cpp
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


#include "include/ChannelBot.h"
#include <management/Whois.h>
#include <management/WhoisDataContainer.h>
#include <core/BotLib.h>
#include <core/Global.h>
#include <core/Output.h>
#include <core/DatabaseData.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <boost/algorithm/string.hpp>

extern "C" ModuleInterface* create()
{
    return new ChannelBot;
}

extern "C" void destroy(ModuleInterface* x)
{
    delete x;
}

ChannelBot::ChannelBot()
{
    bParseRaw = true;
    bParsePrivmsg = true;
    // bParseRaw = true;
    // bParseRaw = true;
}

ChannelBot::~ChannelBot()
{
    stop();
    Global::Instance().get_IrcData().DelConsumer(mpDataInterface);
    delete mpDataInterface;
    Whois::Instance().DelConsumer(mpWhoisDataContainerInterface);
    delete mpWhoisDataContainerInterface;
}

void ChannelBot::Init(DataInterface* pData)
{
    mpWhoisDataContainerInterface = new WhoisDataContainer();
    mpWhoisDataContainerInterface->Init();
    Whois::Instance().AddConsumer(mpWhoisDataContainerInterface);
    mpDataInterface = pData;
    mpDataInterface->Init(true, false, false, true);
    Global::Instance().get_IrcData().AddConsumer(mpDataInterface);
    channelbottrigger = Global::Instance().get_ConfigReader().GetString("channelbottrigger");
    command_table = "ChannelBotCommands";
    longtime = 100;


    /*int Tijd;
    time_t t= time(0);
    Tijd = t;

    timer_long_sec.push_back(Tijd + 120);
    timer_long_command.push_back("time 120 from now");
    timer_long_sec.push_back(Tijd + 20);
    timer_long_command.push_back("time 20 from now");*/
    timerlong();
    DatabaseData::Instance().DatabaseData::AddBinds(command_table);
}

void ChannelBot::stop()
{
    run = false;
    mpDataInterface->stop();
    std::string sOutput;
    sOutput = "ChannelBot::stop";
    Output::Instance().addOutput(sOutput, 3);
    raw_parse_thread->join();
    sOutput = "raw_parse_thread stopped";
    Output::Instance().addOutput(sOutput, 3);
    privmsg_parse_thread->join();
    sOutput = "privmsg_parse_thread stopped";
    Output::Instance().addOutput(sOutput, 3);
}

void ChannelBot::read()
{
    run = true;
    assert(!raw_parse_thread);
    raw_parse_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ChannelBot::parse_raw, this)));
    assert(!privmsg_parse_thread);
    privmsg_parse_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ChannelBot::parse_privmsg, this)));
    assert(!whois_loop_thread);
    whois_loop_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ChannelBot::WhoisLoop, this)));
}

void ChannelBot::parse_raw()
{
    std::vector< std::string > data;
    while(run)
    {
        data = mpDataInterface->GetRawQueue();
        ParseData(data);
    }
}

void ChannelBot::parse_privmsg()
{
    std::vector< std::string > data;
    while(run)
    {
        data = mpDataInterface->GetPrivmsgQueue();
        PRIVMSG(data, channelbottrigger);
    }
}

void ChannelBot::WhoisLoop()
{
    std::string sOutput;
    std::pair< std::string, std::string > data;
    while(run)
    {
        data = mpWhoisDataContainerInterface->GetWhoisQueue();
        std::string nick = data.first;
        std::string chan = data.second;
        std::string botnick = Global::Instance().get_BotNick();
        UsersInterface& U = Global::Instance().get_Users();
        sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
        Output::Instance().addOutput(sOutput, 3);
        if (nick == botnick)
        {
        }
        else
        {
            if (U.GetUid(nick) != "NULL")
            {
                OnUserJoin(chan, nick);
            }
        }
    }
}

void ChannelBot::ParseData(std::vector< std::string > data)
{
    /*if (data.size() == 3)
    {
        if (data[1] == "JOIN")      //JOIN
        {
            JOIN(data);
        }
        if (data[1] == "NICK")      //NICK nickchange
        {
            NICK(data);
        }
    }*/
    /*if (data.size() >= 3)
    {
        if (data[1] == "PART")      //PART
        {
            PART(data);
        }
    }*/
    if (data.size() >= 4)
    {
        if (data[1] == "INVITE")   //INVITE
        {
            INVITE(data);
        }
    }
    /*if (data.size() >= 5)
    {
        if (data[1] == "KICK")      //KICK
        {
            KICK(data);
        }
    }*/
}

void ChannelBot::ParsePrivmsg(std::string nick, std::string command, std::string chan, std::vector< std::string > args, int chantrigger)
{
    UsersInterface& U = Global::Instance().get_Users();
    std::string auth = U.GetAuth(nick);
    std::string bind_command = DatabaseData::Instance().GetCommandByBindNameAndBind(command_table, command);
    int bind_access = DatabaseData::Instance().GetAccessByBindNameAndBind(command_table, command);
    std::string sOutput;
    sOutput = bind_command + " " + Output::Instance().StringFromInt(bind_access);
    Output::Instance().addOutput(sOutput, 3);

    //auth
    if (boost::iequals(bind_command, "auth"))
    {
        if (args.size() == 0)
        {
            authme(nick, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //ping
    if (boost::iequals(bind_command, "ping"))
    {
        if (args.size() == 0)
        {
            ping(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //version
    if (boost::iequals(bind_command, "version"))
    {
        if (args.size() == 0)
        {
            version(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //uptime
    if (boost::iequals(bind_command, "uptime"))
    {
        if (args.size() == 0)
        {
            uptime(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //up
    if (boost::iequals(bind_command, "up"))
    {
        if (args.size() == 0)
        {
            up(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //down
    if (boost::iequals(bind_command, "down"))
    {
        if (args.size() == 0)
        {
            down(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //resync
    if (boost::iequals(bind_command, "resync"))
    {
        if (args.size() == 0)
        {
            resync(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //access
    if (boost::iequals(bind_command, "access"))
    {
        if (args.size() == 0)
        {
            if (chantrigger >= 0)
            {
                access(chan, nick, nick, auth, bind_access);
            }
            else
            {
                //help(bind_command);
            }
        }
        else if (args.size() == 1)
        {
            if (chantrigger >= 0)
            {
                access(chan, nick, args[0], U.GetAuth(args[0]), bind_access);
            }
            else
            {
                //help(bind_command);
            }
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //myaccess
    if (boost::iequals(bind_command, "myaccess"))
    {
        if (args.size() == 0)
        {
            myaccess(nick, nick, auth, bind_access);
        }
        else if (args.size() == 1)
        {
            if (U.GetGod(nick) == 1)
            {
                myaccess(nick, args[0], U.GetAuth(args[0]), bind_access);
            }
            else
            {
                std::string returnstring = "NOTICE " + nick + " :" + irc_reply("need_god", U.GetLanguage(nick)) + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //users
    if (boost::iequals(bind_command, "users"))
    {
        if (args.size() == 0)
        {
            users(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    // readchannel
    if (boost::iequals(bind_command, "readchannel"))
    {
        if (args.size() == 0)
        {
            readchannel(chan, nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //channelcommands
    if (boost::iequals(bind_command, "channelcommands"))
    {
        if (args.size() == 0)
        {
            channelcommands(nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //op
    if (boost::iequals(bind_command, "op"))
    {
        if (args.size() >= 1)
        {
            op(chan, nick, auth, args[0], U.GetAuth(args[0]), bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //deop
    if (boost::iequals(bind_command, "deop"))
    {
        if (args.size() >= 1)
        {
            deop(chan, nick, auth, args[0], U.GetAuth(args[0]), bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //voice
    if (boost::iequals(bind_command, "voice"))
    {
        if (args.size() >= 1)
        {
            voice(chan, nick, auth, args[0], U.GetAuth(args[0]), bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //devoice
    if (boost::iequals(bind_command, "devoice"))
    {
        if (args.size() >= 1)
        {
            devoice(chan, nick, auth, args[0], U.GetAuth(args[0]), bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //kickuser
    if (boost::iequals(bind_command, "kickuser"))
    {
        if (args.size() >= 1)
        {
            std::string reason = "";
            for (unsigned int args_it = 1; args_it < args.size()-1; args_it++)
            {
                reason = reason + args[args_it] + " ";
            }
            if (args.size() > 1)
            {
                reason = reason + args[args.size()-1];
            }
            kickuser(chan, nick, auth, args[0], U.GetAuth(args[0]), reason, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //deluser
    if (boost::iequals(bind_command, "deluser"))
    {
        if (args.size() >= 1)
        {
            for (unsigned int args_it = 0; args_it < args.size(); args_it++)
            {
                deluser(chan, nick, auth, args[args_it], U.GetAuth(args[args_it]), bind_access);
            }
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //adduser
    if (boost::iequals(bind_command, "adduser"))
    {
        if (args.size() >= 2)
        {
            unsigned int last_args_it = args.size() - 1;
            if (fmod(convertString(args[last_args_it]), 1) == 0)
            {
                for (unsigned int args_it = 0; args_it < last_args_it; args_it++)
                {
                    adduser(chan, nick, auth, args[args_it], U.GetAuth(args[args_it]), convertString(args[last_args_it]), bind_access);
                }
            }
            else
            {
                //help(integer);
            }
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //changelevel
    if (boost::iequals(bind_command, "changelevel"))
    {
        if (args.size() >= 2)
        {
            unsigned int last_args_it = args.size() - 1;
            for (unsigned int args_it = 0; args_it < last_args_it; args_it++)
            {
                changelevel(chan, nick, auth, args[args_it], U.GetAuth(args[args_it]), convertString(args[last_args_it]), bind_access);
            }
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    // stats
    if (boost::iequals(bind_command, "stats"))
    {
        if (args.size() == 0)
        {
            stats(nick, auth, bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    // set
    if (boost::iequals(bind_command, "set"))
    {
        if (args.size() == 0)  // set list
        {
            set(chan, nick, auth, bind_access);
        }
        else if (args.size() == 1)  // set get value
        {
            set(chan, nick, auth, args[0], bind_access);
        }
        else if (args.size() == 2)  // set set value
        {
            set(chan, nick, auth, args[0], args[1], bind_access);
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }

    //simulate
    if (boost::iequals(bind_command, "simulate"))
    {
        if (args.size() >= 2)
        {
            if (U.GetGod(nick) == 1)
            {
                std::vector< std::string > simulate_args;
                for (unsigned int args_it = 2; args_it < args.size(); args_it++)
                {
                    simulate_args.push_back(args[args_it]);
                }
                simulate(nick, auth, chan, args[0], args[1], simulate_args, bind_access);
            }
            else
            {
                std::string returnstring = "NOTICE " + nick + " :" + irc_reply("need_god", U.GetLanguage(nick)) + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            //help(bind_command);
        }
        overwatch(bind_command, command, chan, nick, auth, args);
    }
}

void ChannelBot::version(std::string chan, std::string nick, std::string auth, int ca)
{
    std::string returnstr = "PRIVMSG " + chan + " :" + nick + ": Tran V1.5.0 C++ IRC bot\r\n";
    Send(returnstr);
}

void ChannelBot::uptime(std::string chan, std::string nick, std::string auth, int ca)
{
    int uptime;
    time_t t = time(0);
    uptime = t - Global::Instance().get_StartTime();
    std::vector< int > _timevector = VectorTimeFromSecondsTime(uptime);
    if (_timevector.size() == 6)
    {
        int iYear = _timevector[0];
        int iWeek = _timevector[1];
        int iDay = _timevector[2];
        int iHour = _timevector[3];
        int iMinutes = _timevector[4];
        int iSeconds = _timevector[5];
        std::string sYear = convertInt(iYear);
        std::string sWeek = convertInt(iWeek);
        std::string sDay = convertInt(iDay);
        std::string sHour = convertInt(iHour);
        std::string sMinutes = convertInt(iMinutes);
        std::string sSeconds = convertInt(iSeconds);
        std::string returnstring = "PRIVMSG " + chan + " :" + nick + ": Uptime: ";

        if (iYear > 0)
        {
            returnstring = returnstring + sYear + " Year";
            if (iYear > 1)
            {
                returnstring = returnstring + "s";
            }
            returnstring = returnstring + " ";
        }
        if (iWeek > 0)
        {
            returnstring = returnstring + sWeek + " Week";
            if (iWeek > 1)
            {
                returnstring = returnstring + "s";
            }
            returnstring = returnstring + " ";
        }
        if (iDay > 0)
        {
            returnstring = returnstring + sDay + " Day";
            if (iDay > 1)
            {
                returnstring = returnstring + "s";
            }
            returnstring = returnstring + " ";
        }
        if (iHour > 0)
        {
            returnstring = returnstring + sHour + " Hour";
            if (iHour > 1)
            {
                returnstring = returnstring + "s";
            }
            returnstring = returnstring + " ";
        }
        if (iMinutes > 0)
        {
            returnstring = returnstring + sMinutes + " Minute";
            if (iMinutes > 1)
            {
                returnstring = returnstring + "s";
            }
            returnstring = returnstring + " ";
        }
        if (iSeconds > 0)
        {
            returnstring = returnstring + sSeconds + " Second";
            if (iSeconds > 1)
            {
                returnstring = returnstring + "s";
            }
            returnstring = returnstring + " ";
        }
        returnstring = returnstring + "\r\n";
        Send(returnstring);
    }
    else
    {
        // output::Instance().add("_timevector.size() != 6", 1);
    }
}

void ChannelBot::authme(std::string nick, int ca)
{
    std::string returnstr = "WHOIS " + nick + " " + nick + "\r\n";
    Send(returnstr);
}

void ChannelBot::ping(std::string chan, std::string nick, std::string auth, int ca)
{
    std::string returnstr = "PRIVMSG " + chan + " :" + nick + ": PONG\r\n";
    Send(returnstr);
}

void ChannelBot::adduser(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, int reqaccess, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int access = C.GetAccess(chan, auth);
        if (access > reqaccess || U.GetGod(nick) == 1)
        {
            if (C.AddAuth(chan, reqauth))
            {
                std::string UserUuid = U.GetUid(reqnick);
                std::string ChannelUuid = C.GetCid(chan);
                if ((ChannelUuid != "NULL") && (UserUuid != "NULL") && (reqaccess > 0))
                {
                    C.AddUserToChannel(ChannelUuid, UserUuid, reqaccess);
                    std::string sendstring = "NOTICE " + nick + " :user " + reqnick + "[" + reqauth + "] added to the userlist with " + convertInt(reqaccess) + " access\r\n";
                    Send(sendstring);
                }
            }
        }
    }
}

void ChannelBot::deluser(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int access = C.GetAccess(chan, auth);
        int tmpaccess = C.GetAccess(chan, reqauth);
        if (tmpaccess > 0)
        {
            if (tmpaccess < access || U.GetGod(nick) == 1)
            {
                std::string UserUuid = U.GetUid(reqnick);
                std::string ChannelUuid = C.GetCid(chan);
                if ((ChannelUuid != "NULL") && (UserUuid != "NULL"))
                {
                    C.DeleteUserFromChannel(ChannelUuid, UserUuid);
                    std::string sendstring = "NOTICE " + nick + " :user " + reqnick + "[" + reqauth + "] deleted from the userlist\r\n";
                    Send(sendstring);
                }
            }
        }
    }
}

void ChannelBot::changelevel(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, int reqaccess, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int oldaccess = C.GetAccess(chan, reqauth);
        int access = C.GetAccess(chan, auth);
        if (((access > reqaccess) && (access > oldaccess)) || U.GetGod(nick) == 1)
        {
            std::string UserUuid = U.GetUid(reqnick);
            std::string ChannelUuid = C.GetCid(chan);
            if ((ChannelUuid != "NULL") && (UserUuid != "NULL") && (reqaccess > 0))
            {
                //std::string sqlstring = "UPDATE users SET access = '" + convertInt(reqaccess) + "' WHERE ChannelUuid = '" + ChannelUuid + "' AND UserUuid = '" + UserUuid + "';";
                C.SetAccess(chan, reqauth, reqaccess);
                DatabaseData::Instance().UpdateData("users", "access", BotLib::StringFromInt(reqaccess), "ChannelUuid = '" + ChannelUuid + "' AND UserUuid = '" + UserUuid + "';");
                //RawSql(sqlstring);
                std::string sendstring = "NOTICE " + nick + " :user " + reqnick + "[" + reqauth + "] changed access to " + convertInt(reqaccess) + " access\r\n";
                Send(sendstring);
            }
        }
    }
}

void ChannelBot::op(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(chan, auth);
    if (access >= C.GetGiveops(chan))
    {
        if (boost::iequals(reqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(chan, reqauth);
            if (tmpaccess < access || U.GetGod(nick) == 1)
            {
                std::string returnstring = "MODE " + chan + " +o " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + chan + " +o " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void ChannelBot::deop(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(chan, auth);
    if (access >= C.GetGiveops(chan))
    {
        if (boost::iequals(reqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(chan, reqauth);
            if (tmpaccess < access || U.GetGod(nick) == 1)
            {
                std::string returnstring = "MODE " + chan + " -o " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + chan + " -o " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void ChannelBot::voice(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(chan, auth);
    if (access >= C.GetGiveops(chan))
    {
        if (boost::iequals(reqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(chan, reqauth);
            if (tmpaccess < access || U.GetGod(nick) == 1)
            {
                std::string returnstring = "MODE " + chan + " +v " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + chan + " +v " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void ChannelBot::devoice(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(chan, auth);
    if (access >= C.GetGiveops(chan))
    {
        if (boost::iequals(reqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(chan, reqauth);
            if (tmpaccess < access || U.GetGod(nick) == 1)
            {
                std::string returnstring = "MODE " + chan + " -v " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + chan + " -v " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void ChannelBot::stats(std::string sNick, std::string sAuth, int iCa)
{
/*
-Centravi- There are currently...
-Centravi- ...4835 channel access entries.
-Centravi- ...234 channels registered.
-Centravi- ...7891 accounts in my database.
-Centravi- ...25 accounts which have staff-access.
-Centravi- ...278 language keys.
-Centravi- ...16 languages this bot is (being) translated to.
-Centravi- ...96 commands.
-Centravi- ...109 command aliasses.
-Centravi- ...2w52m38s this bot is running.
*/


    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int iChannelCount = 0;
    int iUserCount = 0;
    int iAuthCount = 0;
    //int iOperAccessUsers = 0;
    int iAccessCount = 0;

    iChannelCount = C.GetChannels().size();
    iAuthCount = U.GetAuths().size();
    std::vector< std::string > vChannels = C.GetChannels();
    for (unsigned int uiChannelsIndex = 0 ; uiChannelsIndex < vChannels.size(); uiChannelsIndex++ )
    {
        iUserCount = iUserCount + U.GetNicks(vChannels[uiChannelsIndex]).size();
        std::vector< std::string > vAuths = C.GetAuths(vChannels[uiChannelsIndex]);
        int access;
        for (unsigned int uiAuthsIndex = 0 ; uiAuthsIndex < vAuths.size(); uiAuthsIndex++ )
        {
            access = C.GetAccess(vChannels[uiChannelsIndex], vAuths[uiAuthsIndex]);
            if (access > 0)
            {
                iAccessCount++;
            }
        }
    }
    /*std::vector< std::string > vAuths = U.GetAuths();
    for (unsigned int uiAuthsIndex = 0; uiAuthsIndex < vAuths.size(); uiAuthsIndex++)
    {
        if ()
    }*/






    std::string sReplyString;
    // ChannelCount
    sReplyString = irc_reply("STATS_CHANNEL_COUNT", U.GetLanguage(sNick));
    sReplyString = irc_reply_replace(sReplyString, "$ChannelCount$", convertInt(iChannelCount));
    Send(Global::Instance().get_Reply().irc_notice(sNick, sReplyString));

    // UserCount
    /*sReplyString = irc_reply("STATS_USER_COUNT", U.GetLanguage(sNick));
    sReplyString = irc_reply_replace(sReplyString, "$UserCount$", convertInt(iUserCount));
    Send(Global::Instance().get_Reply().irc_notice(sNick, sReplyString));*/  // not working yet

    // UserCount
    sReplyString = irc_reply("STATS_AUTH_COUNT", U.GetLanguage(sNick));
    sReplyString = irc_reply_replace(sReplyString, "$AuthCount$", convertInt(iAuthCount));
    Send(Global::Instance().get_Reply().irc_notice(sNick, sReplyString));

    // UserCount
    sReplyString = irc_reply("STATS_ACCESS_COUNT", U.GetLanguage(sNick));
    sReplyString = irc_reply_replace(sReplyString, "$AccessCount$", convertInt(iAccessCount));
    Send(Global::Instance().get_Reply().irc_notice(sNick, sReplyString));


    // uptime

    sReplyString = irc_reply("STATS_UPTIME", U.GetLanguage(sNick));
    int uptime;
    time_t t = time(0);
    uptime = t - Global::Instance().get_StartTime();
    std::vector< int > _timevector = VectorTimeFromSecondsTime(uptime);
    if (_timevector.size() == 6)
    {
        int iYear = _timevector[0];
        int iWeek = _timevector[1];
        int iDay = _timevector[2];
        int iHour = _timevector[3];
        int iMinutes = _timevector[4];
        int iSeconds = _timevector[5];
        std::string sYear = convertInt(iYear);
        std::string sWeek = convertInt(iWeek);
        std::string sDay = convertInt(iDay);
        std::string sHour = convertInt(iHour);
        std::string sMinutes = convertInt(iMinutes);
        std::string sSeconds = convertInt(iSeconds);

        if (iYear > 0)
        {
            sReplyString = sReplyString + sYear + " Year";
            if (iYear > 1)
            {
                sReplyString = sReplyString + "s";
            }
            sReplyString = sReplyString + " ";
        }
        if (iWeek > 0)
        {
            sReplyString = sReplyString + sWeek + " Week";
            if (iWeek > 1)
            {
                sReplyString = sReplyString + "s";
            }
            sReplyString = sReplyString + " ";
        }
        if (iDay > 0)
        {
            sReplyString = sReplyString + sDay + " Day";
            if (iDay > 1)
            {
                sReplyString = sReplyString + "s";
            }
            sReplyString = sReplyString + " ";
        }
        if (iHour > 0)
        {
            sReplyString = sReplyString + sHour + " Hour";
            if (iHour > 1)
            {
                sReplyString = sReplyString + "s";
            }
            sReplyString = sReplyString + " ";
        }
        if (iMinutes > 0)
        {
            sReplyString = sReplyString + sMinutes + " Minute";
            if (iMinutes > 1)
            {
                sReplyString = sReplyString + "s";
            }
            sReplyString = sReplyString + " ";
        }
        if (iSeconds > 0)
        {
            sReplyString = sReplyString + sSeconds + " Second";
            if (iSeconds > 1)
            {
                sReplyString = sReplyString + "s";
            }
            sReplyString = sReplyString + " ";
        }
        Send(Global::Instance().get_Reply().irc_notice(sNick, sReplyString));
    }
    else
    {
        // output::Instance().add("_timevector.size() != 6", 1);
    }
}

void ChannelBot::users(std::string chan, std::string nick, std::string auth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::vector< std::string > auths = C.GetAuths(chan);
    sort(auths.begin(), auths.end());
    std::string returnstr;
    std::vector< std::string > sortauths;
    std::vector< int > sortaccess;
    for (unsigned int i = 0 ; i < auths.size(); i++ )
    {
        int access = C.GetAccess(chan, auths[i]);
        if (access > 0)
        {
            bool added = false;
            for (unsigned int j = 0; j < sortaccess.size(); j++)
            {
                if (sortaccess[j] < access)
                {
                    sortauths.insert(sortauths.begin()+j, auths[i]);
                    sortaccess.insert(sortaccess.begin()+j, access);
                    j = sortaccess.size();
                    added = true;
                }
            }
            if (!added)
            {
                sortauths.push_back(auths[i]);
                sortaccess.push_back(access);
            }
        }
    }

    unsigned int length = U.GetWidth(nick);
    std::string commandrpl = irc_reply("USERS", U.GetLanguage(nick));
    commandrpl = commandrpl + " " + chan;
    returnstr = "NOTICE " + nick + " :";
    for (unsigned int l = 0; l < (((length * 2) / 2) - commandrpl.size()/2); l++)
    {
        returnstr = returnstr + " ";
    }
    returnstr = returnstr + commandrpl + "\r\n";
    Send(returnstr);
    for (unsigned int i = 0; i < sortauths.size(); i++)
    {
        returnstr = "NOTICE " + nick + " :" + fillspace(convertInt(sortaccess[i]), length) + sortauths[i] + "\r\n";
        Send(returnstr);
    }
    std::string nousersrpl = irc_reply("NUMBER_USERS", U.GetLanguage(nick));
    nousersrpl = nousersrpl + " " + convertInt(sortauths.size());
    returnstr = "NOTICE " + nick + " :";
    for (unsigned int l = 0; l < (((length * 2) / 2) - nousersrpl.size()/2); l++)
    {
        returnstr = returnstr + " ";
    }
    returnstr = returnstr + nousersrpl + "\r\n";
    Send(returnstr);
}

void ChannelBot::readchannel(std::string chan, std::string nick, std::string auth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::vector< std::string > vNicks = C.GetNicks(chan);
    for (unsigned int i = 0 ; i < vNicks.size(); i++ )
    {
        int access = 0;
        if (C.GetVoice(chan, vNicks[i]))
        {
            access = C.GetGivevoice(chan);
        }
        if (C.GetOp(chan, vNicks[i]))
        {
            access = C.GetGiveops(chan);
        }
        if (access > 0)
        {
            adduser(chan, nick, auth, vNicks[i], U.GetAuth(vNicks[i]), access, ca);
        }
    }
}

void ChannelBot::kickuser(std::string chan, std::string nick, std::string auth, std::string reqnick, std::string reqauth, std::string reason, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int access = C.GetAccess(chan, auth);
        int tmpaccess = C.GetAccess(chan, reqauth);
        if (tmpaccess < access || U.GetGod(nick) == 1)
        {
            std::string returnstring = "KICK " + chan + " " + reqnick + " :" + reason + "\r\n";
            Send(returnstring);
        }
    }
    else
    {
        std::string returnstring = "KICK " + chan + " " + reqnick + " :" + reason + "\r\n";
        Send(returnstring);
    }
}

void ChannelBot::access(std::string chan, std::string nick, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(chan, reqauth);
    int oaccess = U.GetOaccess(reqnick);
    int god = U.GetGod(reqnick);
    std::string returnstr;
    if (access > 0)
    {
        returnstr = "NOTICE " + nick + " :" + reqnick + "[" + reqauth + "] has " + convertInt(access) + " access to " + chan + "\r\n";
        Send(returnstr);
    }
    else
    {
        returnstr = "NOTICE " + nick + " :" + reqnick + "[" + reqauth + "] lacks access to " + chan + "\r\n";
        Send(returnstr);
    }
    if (oaccess > 0)
    {
        returnstr = "NOTICE " + nick + " :" + reqnick + "[" + reqauth + "] has " + convertInt(oaccess) + " oaccess\r\n";
        Send(returnstr);
    }
    if (god > 0)
    {
        returnstr = "NOTICE " + nick + " :" + reqnick + "[" + reqauth + "] has God enabled\r\n";
        Send(returnstr);
    }
}

void ChannelBot::myaccess(std::string nick, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::string returnstring;
    unsigned int length = U.GetWidth(nick);
    unsigned int amount = 2;
    unsigned int channel_count = 0;
    std::string commandrpl = irc_reply("myaccess", U.GetLanguage(nick));
    commandrpl = irc_reply_replace(commandrpl, "$nick$", reqnick);
    commandrpl = irc_reply_replace(commandrpl, "$auth$", reqauth);
    returnstring = Global::Instance().get_Reply().irc_notice(nick, centre(commandrpl.size(), amount, length) + commandrpl);
    //returnstring = "NOTICE " + nick + " :" + centre(commandrpl.size(), amount, length) + commandrpl + "\r\n";
    Send(returnstring);

    std::string channels;
    std::vector< std::string > sortchannels = C.GetChannels();
    sort (sortchannels.begin(), sortchannels.end());
    for ( unsigned int i = 0 ; i < sortchannels.size(); i++ )
    {
        int access = C.GetAccess(sortchannels[i], reqauth);
        if (access > 0)
        {
            channel_count++;
            //std::string tmpstring = sortchannels[i];
            //tmpstring = fillspace(tmpstring, length) + convertInt(access);
            returnstring = Global::Instance().get_Reply().irc_notice(nick, fillspace(sortchannels[i], length) + convertInt(access));
            //returnstring = Global::Instance().get_Reply().irc_notice(nick, tmpstring);
            Send(returnstring);
        }
    }

    std::string nochannelsrpl = irc_reply("myaccess_number_channels", U.GetLanguage(nick));
    nochannelsrpl = irc_reply_replace(nochannelsrpl, "$channels$", convertInt(channel_count));
    returnstring = Global::Instance().get_Reply().irc_notice(nick, centre(nochannelsrpl.size(), amount, length) + nochannelsrpl);
    Send(returnstring);
}

void ChannelBot::up(std::string chan, std::string nick, std::string auth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::string reply_string;
    //bool giveop = false;
    //bool givevoice = false;
    int access = C.GetAccess(chan, auth);
    std::string sOutput;
    sOutput = "giveops" + C.GetGiveops(chan);
    Output::Instance().addOutput(sOutput, 3);
    sOutput = "access" + access;
    Output::Instance().addOutput(sOutput, 3);
    if (access >= C.GetGiveops(chan))
    {
        //if (C.GetOp(chan, nick) == false)
        {
            std::string returnstring = "MODE " + chan + " +o " + nick + "\r\n";
            Send(returnstring);
            reply_string = "NOTICE " + nick + " :" + irc_reply("up_op", U.GetLanguage(nick)) + "\r\n";
            reply_string = irc_reply_replace(reply_string, "$channel$", chan);
            Send(reply_string);
        }
        /*else
        {
            reply_string = "NOTICE " + nick + " :" + irc_reply("up_already_up", U.GetLanguage(nick)) + "\r\n";
            reply_string = irc_reply_replace(reply_string, "$channel$", chan);
            Send(reply_string);
        }*/
    }
    else if (access >= C.GetGivevoice(chan))
    {
        //if (C.GetVoice(chan, nick) == false)
        {
            std::string returnstring = "MODE " + chan + " +v " + nick + "\r\n";
            Send(returnstring);
            reply_string = "NOTICE " + nick + " :" + irc_reply("up_voice", U.GetLanguage(nick)) + "\r\n";
            reply_string = irc_reply_replace(reply_string, "$channel$", chan);
            Send(reply_string);
        }
        /*else
        {
            reply_string = "NOTICE " + nick + " :" + irc_reply("up_already_up", U.GetLanguage(nick)) + "\r\n";
            reply_string = irc_reply_replace(reply_string, "$channel$", chan);
            Send(reply_string);
        }*/
    }
    if (C.GetOp(chan, nick) == true && C.GetVoice(chan, nick) == true)
    {
        reply_string = "NOTICE " + nick + " :" + irc_reply("up_already_up", U.GetLanguage(nick)) + "\r\n";
        reply_string = irc_reply_replace(reply_string, "$channel$", chan);
        Send(reply_string);
    }
}

void ChannelBot::down(std::string chan, std::string nick, std::string auth, int ca)
{
    //ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::string reply_string;
    //if (C.GetOp(chan, nick) == true && C.GetVoice(chan, nick) == true)
    {
        std::string returnstring = "MODE " + chan + " -ov " + nick + " " + nick + "\r\n";
        Send(returnstring);
        reply_string = "NOTICE " + nick + " :" + irc_reply("down", U.GetLanguage(nick)) + "\r\n";
    }
    /*else if (C.GetVoice(chan, nick) == true)
    {
        std::string returnstring = "MODE " + chan + " -v " + nick + "\r\n";
        Send(returnstring);
        reply_string = "NOTICE " + nick + " :" + irc_reply("down_devoice", U.GetLanguage(nick)) + "\r\n";
    }
    else if (C.GetOp(chan, nick) == true)
    {
        std::string returnstring = "MODE " + chan + " -o " + nick + "\r\n";
        Send(returnstring);
        reply_string = "NOTICE " + nick + " :" + irc_reply("down_deop", U.GetLanguage(nick)) + "\r\n";
    }
    if (C.GetOp(chan, nick) == false && C.GetVoice(chan, nick) == false)
    {
        reply_string = "NOTICE " + nick + " :" + irc_reply("down_already_down", U.GetLanguage(nick)) + "\r\n";
    }*/
    reply_string = irc_reply_replace(reply_string, "$channel$", chan);
    Send(reply_string);
}

void ChannelBot::resync(std::string chan, std::string nick, std::string auth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::vector< std::string > nicks = C.GetNicks(chan);
    int access ;
    for (unsigned int i = 0; i < nicks.size(); i++)
    {
        if (!boost::iequals(Global::Instance().get_BotNick(), nicks[i]))
        {
            access = C.GetAccess(chan, U.GetAuth(nicks[i]));
            if (access >= C.GetGiveops(chan))
            {
                if (C.GetOp(chan, nicks[i]) == false)
                {
                    std::string returnstring = "MODE " + chan + " +o " + nicks[i] + "\r\n";
                    Send(returnstring);
                }
            }
            else if (access >= C.GetGivevoice(chan))
            {
                if (C.GetVoice(chan, nicks[i]) == false)
                {
                    std::string returnstring = "MODE " + chan + " +v " + nicks[i] + "\r\n";
                    Send(returnstring);
                }
            }
            if ((access < C.GetGiveops(chan)) && (access < C.GetGivevoice(chan)))
            {
                if (C.GetOp(chan, nicks[i]) == true || C.GetVoice(chan, nicks[i]) == true)
                {
                    std::string returnstring = "MODE " + chan + " -ov " + nicks[i] + " " + nicks[i] + "\r\n";
                    Send(returnstring);
                }
            }
            else if (access < C.GetGivevoice(chan))
            {
                if (C.GetVoice(chan, nicks[i]) == false)
                {
                    std::string returnstring = "MODE " + chan + " -v " + nicks[i] + "\r\n";
                    Send(returnstring);
                }
            }
            else if (access < C.GetGiveops(chan))
            {
                if (C.GetOp(chan, nicks[i]) == false)
                {
                    std::string returnstring = "MODE " + chan + " -o " + nicks[i] + "\r\n";
                    Send(returnstring);
                }
            }
        }
    }
}

// set List
void ChannelBot::set(std::string msChannel, std::string msNick, std::string msAuth, int miChannelAcess)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int iAccess = C.GetAccess(msChannel, msAuth);
    if (iAccess >= miChannelAcess)
    {

        int iGiveOps = C.GetGiveops(msChannel);
        int iGiveVoice = C.GetGivevoice(msChannel);

        std::string sReplyString;
        // GiveOps
        sReplyString = irc_reply("SET_GIVE_OPS", U.GetLanguage(msNick));
        sReplyString = fillspace(sReplyString, 20);
        sReplyString = sReplyString + convertInt(iGiveOps);
        Send(Global::Instance().get_Reply().irc_notice(msNick, sReplyString));

        // GiveVoice
        sReplyString = irc_reply("SET_GIVE_VOICE", U.GetLanguage(msNick));
        sReplyString = fillspace(sReplyString, 20);
        sReplyString = sReplyString + convertInt(iGiveVoice);
        Send(Global::Instance().get_Reply().irc_notice(msNick, sReplyString));
    }
    else
    {
        // need more access;
    }
}

// set Get Value
void ChannelBot::set(std::string msChannel, std::string msNick, std::string msAuth, std::string msSetting, int miChannelAcess)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int iAccess = C.GetAccess(msChannel, msAuth);
    if (iAccess >= miChannelAcess)
    {
        std::string sVariable;
        std::string sReplyString;

        // Show Setting
        sReplyString = irc_reply("SET_VALUE_VARIABLE", U.GetLanguage(msNick));
        sReplyString = irc_reply_replace(sReplyString, "$Variable$", sVariable);
        Send(Global::Instance().get_Reply().irc_notice(msNick, sReplyString));
    }
    else
    {
        // need more access;
    }
}

// set Setter
void ChannelBot::set(std::string msChannel, std::string msNick, std::string msAuth, std::string msSetting, std::string msValue, int miChannelAcess)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int iAccess = C.GetAccess(msChannel, msAuth);
    if (iAccess >= miChannelAcess)
    {
        if (boost::iequals(msSetting, "giveops") || boost::iequals(msSetting, "give_ops"))
        {
            C.SetSetting(msChannel, "giveops", msValue);
            /*DatabaseData::Instance().UpdateData("channels", "giveops", msValue, "`channel` = '" + msChannel +"'");
            C.SetGiveops(msChannel, );*/
        }
        if (boost::iequals(msSetting, "givevoice") || boost::iequals(msSetting, "give_voice"))
        {
            C.SetSetting(msChannel, "givevoice", msValue);
            /*DatabaseData::Instance().UpdateData("channels", "givevoice", msValue, "`channel` = '" + msChannel +"'");
            C.SetGivevoice(msChannel, BotLib::IntFromString(msValue));*/
        }
        if (boost::iequals(msSetting, "setters"))
        {
            C.SetSetting(msChannel, "setters", msValue);
            /*DatabaseData::Instance().UpdateData("channels", "setters", msValue, "`channel` = '" + msChannel +"'");
            C.SetSetters(msChannel, BotLib::IntFromString(msValue));*/
        }
    }
    else
    {
        // need more access;
    }
}

void ChannelBot::channelcommands(std::string mNick, std::string auth, int ca)
{
    UsersInterface& U = Global::Instance().get_Users();
    std::string returnstring;
    unsigned int length = U.GetWidth(mNick);
    unsigned int amount = U.GetWidthLength(mNick);
    std::string commandrpl = irc_reply("channelcommands", U.GetLanguage(mNick));
    returnstring = "NOTICE " + mNick + " :";
    for (unsigned int l = 0; l < (((length * amount) / 2) - commandrpl.size()/2); l++)
    {
        returnstring = returnstring + " ";
    }
    returnstring = returnstring + commandrpl + "\r\n";
    Send(returnstring);

    returnstring = "NOTICE " + mNick + " :";
    returnstring = returnstring + fillspace("bind", 20);
    returnstring = returnstring + fillspace("command", 20);
    returnstring = returnstring + "access\r\n";
    Send(returnstring);
    std::vector< std::string > binds = DatabaseData::Instance().GetBindVectorByBindName(command_table);
    sort (binds.begin(), binds.end());
    for (unsigned int binds_it = 0; binds_it < binds.size(); binds_it++)
    {
        std::string bind_access = convertInt(DatabaseData::Instance().GetAccessByBindNameAndBind(command_table, binds[binds_it]));
        std::string bind_command = DatabaseData::Instance().GetCommandByBindNameAndBind(command_table, binds[binds_it]);
        if (bind_command != "")
        {
            returnstring = "NOTICE " + mNick + " :";
            returnstring = returnstring + fillspace(binds[binds_it], 20);
            returnstring = returnstring + fillspace(bind_command, 20);
            returnstring = returnstring + bind_access + "\r\n";
            Send(returnstring);
        }
    }

    /*//vector<string> sortbinds = binds;
    std::vector< std::string > sortbinds = DatabaseData::Instance().GetBindVectorByBindName(command_table);
    sort (sortbinds.begin(), sortbinds.end());
    std::vector< std::string > command_reply_vector = lineout(sortbinds, amount, length);
    for (unsigned int h = 0; h < command_reply_vector.size(); h++)
    {
        returnstring = "NOTICE " + nick + " :" + command_reply_vector[h] + "\r\n";
        Send(returnstring);
    }*/
}

void ChannelBot::INVITE(std::vector< std::string > data)
{
    std::string returnstr = "JOIN " + data[3] + "\r\n";
    Send(returnstr);
}

void ChannelBot::JOIN(std::vector< std::string > data)
{
    std::string botnick = Global::Instance().get_BotNick();
    UsersInterface& U = Global::Instance().get_Users();
    std::string chan = data[2];
    boost::erase_all(chan, ":");
    std::string nick = HostmaskToNick(data);
    if (nick == botnick)
    {
    }
    else
    {
        if (U.GetUid(nick) != "NULL")
        {
            OnUserJoin(chan, nick);
        }
    }
}

void ChannelBot::PART(std::vector< std::string > data)
{/*
    vector<string> chan = Split(data[2], ":",true,true);
    string nick = HostmaskToNick(data);
    std::string sOutput;
    if (nick == botnick)
    {
        vector<string> chanusers = C->GetNicks(chan[0]);
        for ( unsigned int i = 0 ; i < chanusers.size(); i++ )
        {
            U->DelChannel(chanusers[i], chan[0]);
            C->DelNick(chan[0], chanusers[i]);
            if (U->GetChannels(chanusers[i])[0] == "NULL")
            {
                cout << "no channels left" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
                U->DelUser(chanusers[i]);
            }
        }
        C->DelChannel(chan[0]);
    }
    else
    {
        C->DelNick(chan[0], nick);
        U->DelChannel(nick, chan[0]);
        if (U->GetChannels(nick)[0] == "NULL")
        {
            cout << "no channels left" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
            U->DelUser(nick);
        }
        cout << "PART" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
    }*/
}

void ChannelBot::KICK(std::vector< std::string > data)
{/*
    string chan = data[2];
    string nick = data[3];
    std::string sOutput;
    if (nick == botnick)
    {
        vector<string> chanusers = C->GetNicks(chan);
        for ( unsigned int i = 0 ; i < chanusers.size(); i++ )
        {
            C->DelNick(chan, chanusers[i]);
            U->DelChannel(chanusers[i], chan);
            if (U->GetChannels(chanusers[i])[0] == "NULL")
            {
                cout << "no channels left" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
                U->DelUser(chanusers[i]);
            }
        }
        C->DelChannel(chan);
    }
    else
    {
        C->DelNick(chan, nick);
        U->DelChannel(nick, chan);
        if (U->GetChannels(nick)[0] == "NULL")
        {
            cout << "no channels left" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
            U->DelUser(nick);
        }
        cout << "KICK" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
    }*/
}

void ChannelBot::MODE(std::vector< std::string > data)
{
    //std::string nick = HostmaskToNick(data);
    /*if (nick == botnick)
    {

    }
    else
    {

    }*/
}

void ChannelBot::QUIT(std::vector< std::string > data)
{/*
    string nick = HostmaskToNick(data);
    if (nick == botnick)
    {
        U->~Users();
        C->~Channels();
    }
    else
    {
        vector<string> channels = U->GetChannels(nick);
        for ( unsigned int i = 0 ; i < channels.size(); i++ )
        {
            C->DelNick(channels[i], nick);
        }
        U->DelUser(nick);
    }*/
}

void ChannelBot::NICK(std::vector< std::string > data)
{/*
    string oldnick = HostmaskToNick(data);
    vector<string> nick = Split(data[2], ":",true,true);
    std::string sOutput;
    if (oldnick == botnick)
    {
        botnick = nick[0];
        U->ChangeNick(oldnick, nick[0]);
        vector<string> channels = U->GetChannels(nick[0]);
        for ( unsigned int i = 0 ; i < channels.size(); i++ )
        {
            C->DelNick(channels[i], oldnick);
            C->AddNick(channels[i], nick[0]);
        }
        cout << "NICK" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
    }
    else
    {
        U->ChangeNick(oldnick, nick[0]);
        vector<string> channels = U->GetChannels(nick[0]);
        for ( unsigned int i = 0 ; i < channels.size(); i++ )
        {
            C->DelNick(channels[i], oldnick);
            C->AddNick(channels[i], nick[0]);
        }
        cout << "NICK" << endl;
    sOutput = "void ChannelBot::WhoisLoop() nick: " + nick + " chan: " + chan;
    Output::Instance().addOutput(sOutput, 3);
    }*/
}

void ChannelBot::OnUserJoin(std::string chan, std::string nick)
{
    UsersInterface& U = Global::Instance().get_Users();
    up(chan, nick, U.GetAuth(nick), 0);
}

void ChannelBot::timerrun()
{
    std::string sOutput;
    sOutput = "channelbot::timerrun()";
    Output::Instance().addOutput(sOutput, 11);
    int Tijd;
    time_t t= time(0);
    Tijd = t;
    longtime++;
    if (longtime >= 100)
    {
        timerlong();
        longtime = 0;
    }
    for (int i = timer_sec.size() - 1; i >= 0; i--)
    {
        if (timer_sec[i] < Tijd)
        {
            sOutput = timer_command[i];
            Output::Instance().addOutput(sOutput, 6);
            timer_sec.erase(timer_sec.begin()+i);
            timer_command.erase(timer_command.begin()+i);
        }
    }
}

void ChannelBot::timerlong()
{
    std::string sOutput;
    int Tijd;
    time_t t= time(0);
    Tijd = t;
    Tijd = Tijd + 100;
    for (int i = timer_long_sec.size() - 1; i >= 0; i--)
    {
        if (timer_long_sec[i] < Tijd)
        {
            sOutput = "timer_long to timer " + timer_long_command[i];
            Output::Instance().addOutput(sOutput, 11);
            timer_sec.push_back(timer_long_sec[i]);
            timer_command.push_back(timer_long_command[i]);
            timer_long_sec.erase(timer_long_sec.begin()+i);
            timer_long_command.erase(timer_long_command.begin()+i);
        }
    }
}
