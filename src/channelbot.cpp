//
//
//  @ Project : ircppbot-channelbot
//  @ File Name : channelbot.cpp
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


#include "include/channelbot.h"
#include "include/config.h"

#include <ircppbot/irc.h>
#include <ircppbot/reply.h>
#include <ircppbot/binds.h>


#include <ircppbot/managementscontainer.h>

#include <gframe/output.h>
#include <gframe/versions.h>
#include <gframe/glib.h>
#include <gframe/configreader.h>

std::string mNAME = PACKAGE;
std::string mVERSION = VERSION;
std::string mGITVERSION = __GIT_VERSION;

extern "C" moduleinterface* create()
{
    return new channelbot;
}

extern "C" void destroy(moduleinterface* x)
{
    delete x;
}

/**
 * Constructor
 * Define what the booleans for parsing.
 *
 */
channelbot::channelbot()
{
    versions::instance().addVersion(mNAME + " " + mVERSION + " " + mGITVERSION);
}

/**
 * Destructor
 * stop the current threads.
 * Remove our vars from the consumer objects and then delete these vars.
 *
 */
channelbot::~channelbot()
{
    stop();
}

/**
 * sets run var to true, so while loops start to run
 * Starts the threads to parse
 *
 */
void channelbot::read()
{
    m_Run = true;
    m_ModesThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&channelbot::parseModes, this)));
    m_EventsThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&channelbot::parseEvents, this)));
    m_PrivmsgThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&channelbot::parsePrivmsg, this)));
}

/**
 * stops the running threads
 *
 */
void channelbot::stop()
{
    m_Run = false;
    m_IrcData->stop();
    output::instance().addOutput("channelbot::stop", 6);
    m_ModesThread->join();
    output::instance().addOutput("channelbot::stop m_ModesThread stopped", 6);
    m_EventsThread->join();
    output::instance().addOutput("channelbot::stop m_EventsThread stopped", 6);
    m_PrivmsgThread->join();
    output::instance().addOutput("channelbot::stop m_PrivmsgThread stopped", 6);
    irc::instance().delConsumer(m_IrcData);
    delete m_IrcData;
}

/**
 * Initialisation
 * Initialise some vars for later use
 * @param pData the value to give to mpDataInterface
 *
 */

void channelbot::init()
{
    /*mpWhoisDataContainerInterface = new WhoisDataContainer();
    mpWhoisDataContainerInterface->Init();
    Whois::Instance().AddConsumer(mpWhoisDataContainerInterface);*/
    m_IrcData = new ircdata();
    m_IrcData->setModes(true);
    m_IrcData->setEvents(true);
    m_IrcData->setPrivmsg(true);
    irc::instance().addConsumer(m_IrcData);

    /*channelbottrigger = Global::Instance().get_ConfigReader().GetString("channelbottrigger");
    command_table = Global::Instance().get_ConfigReader().GetString("channelbotcommandtable");
    command_table = "ChannelBotCommands";*/
    //longtime = 100;


    /*int Tijd;
    time_t t= time(0);
    Tijd = t;

    timer_long_sec.push_back(Tijd + 120);
    timer_long_command.push_back("time 120 from now");
    timer_long_sec.push_back(Tijd + 20);
    timer_long_command.push_back("time 20 from now");*/
    //timerlong();
    //DatabaseData::Instance().DatabaseData::AddBinds(command_table);
    binds::instance().setBind("version", "version", 0, mNAME);
    binds::instance().setBind("commands", "channelcommands", 0, mNAME);
    binds::instance().setBind("ping", "ping", 0, mNAME);
}

void channelbot::parseModes()
{
    output::instance().addOutput("void channelbot::parseModes()");
    std::vector< std::string > data;
    while(m_Run)
    {
        data = m_IrcData->getModesQueue();
        if (!m_Run)
        {
            return;
        }
        /*if (data.size() == 4)
        {
            if (data[1] == "MODE")      //KICK
            {
                mode(data);
            }
        }
        if (data.size() >= 4)
        {
            if (data[1] == "TOPIC")      //TOPIC
            {
                //topic(data);
            }
        }
        if (data.size() >= 5)
        {
            if (data[1] == "332")      //TOPIC
            {
                //topic(data);
            }
        }*/
    }
}

void channelbot::parseEvents()
{
    output::instance().addOutput("void channelbot::parseEvents()");
    std::vector< std::string > data;
    while(m_Run)
    {
        data = m_IrcData->getEventsQueue();
        if (!m_Run)
        {
            return;
        }
        /*if (data.size() == 3)
        {
            if (data[1] == "NICK")
            {
                userName(data);
            }
            if (data[1] == "JOIN")
            {
                join(data);
            }
        }
        if (data.size() >= 3)
        {
            if (data[1] == "QUIT")
            {
                quit(data);
            }
            if (data[1] == "PART")
            {
                part(data);
            }
        }
        if (data.size() >= 4)
        {
            if (data[1] == "KICK")
            {
                kick(data);
            }
        }*/
    }
}

/*
void channelbot::WhoisLoop()
{
    std::string sOutput;
    std::pair< std::string, std::string > data;
    while(run)
    {
        data = mpWhoisDataContainerInterface->GetWhoisQueue();
        std::string userName = data.first;
        std::string channelName = data.second;
        std::string botnick = Global::Instance().get_BotNick();
        UsersInterface& U = Global::Instance().get_Users();
        sOutput = "void channelbot::WhoisLoop() userName: " + userName + " channelName: " + channelName;
        Output::Instance().addOutput(sOutput, 3);
        if (userName == botnick)
        {
        }
        else
        {
            if (U.GetUid(userName) != "NULL")
            {
                OnUserJoin(channelName, userName);
            }
        }
    }
}
*/
void channelbot::parsePrivmsg()
{
    output::instance().addOutput("void channelbot::parsePrivmsg()");
    std::vector< std::string > data;
    while(m_Run)
    {
        data = m_IrcData->getPrivmsgQueue();
        if (!m_Run)
        {
            return;
        }
        int parsed = 0;
        std::string trigger = configreader::instance().getString("libchannelbot.trigger");
        std::vector< std::string > args;
        std::string firstWord;
        size_t chanpos1 = std::string::npos;
        size_t chanpos2 = std::string::npos;
        size_t chanpos3 = std::string::npos;
        size_t triggerpos = std::string::npos;
        chanpos1 = data[2].find("#");
        chanpos2 = data[3].find("#");
        std::string channelName = "";
        std::string command = "";
        std::string userName = data[0];
        nickFromHostmask(userName);
        if (data.size() >= 4)
        {
            firstWord = data[3];
            deleteFirst(firstWord, ":");
        }
        triggerpos = firstWord.substr(0, trigger.length()).find(trigger);
        if (firstWord.substr(0, trigger.length()) == trigger)
        {
            firstWord = firstWord.substr(trigger.length(), firstWord.length()-1);
        }
        if (data.size() >= 5)
        {
            chanpos3 = data[4].find("#");
        }
        if (firstWord != "")
        {
            // PRIVMSG ... :!;
            if (triggerpos != std::string::npos)
            {
                if (chanpos2 != std::string::npos && chanpos3 == std::string::npos)    // chanpos1 yes/no both valid
                {
                    // PRIVMSG userName #channelName :!#channelName command   ||   PRIVMSG userName bot :!#channelName command
                    if (data.size() >= 5)
                    {
                        channelName = firstWord;
                        command = data[4];
                        for (size_t i = 5 ; i < data.size() ; i++)
                        {
                            args.push_back(data[i]);
                        }
                        parsed++;
                        //ParsePrivmsg(userName, command, channelName, args, chantrigger);
                    }
                }
                else if (chanpos1 != std::string::npos && chanpos2 == std::string::npos && chanpos3 == std::string::npos)
                {
                    // PRIVMSG userName #channelName :!command
                    if (data.size() >= 4)
                    {
                        command = firstWord;
                        channelName = data[2];
                        for (size_t i = 4 ; i < data.size() ; i++)
                        {
                            args.push_back(data[i]);
                        }
                        parsed++;
                        //ParsePrivmsg(userName, command, channelName, args, chantrigger);
                    }
                }
                else if (chanpos1 == std::string::npos && chanpos2 == std::string::npos && chanpos3 == std::string::npos)
                {
                    // PRIVMSG userName bot :!command
                    if (data.size() >= 4)
                    {
                        command = firstWord;
                        channelName = ""; // not needed?
                        for (size_t i = 4 ; i < data.size() ; i++)
                        {
                            args.push_back(data[i]);
                        }
                        parsed++;
                        //ParsePrivmsg(userName, command, channelName, args, chantrigger);
                    }
                }
                else if (chanpos2 == std::string::npos && chanpos3 != std::string::npos)    // chanpos1 yes/no both valid
                {
                    // PRIVMSG userName #channelName :!command #channelName    ||      PRIVMSG userName bot :!command #channelName
                    if (data.size() >= 5)
                    {
                        command = firstWord;
                        channelName = data[4];
                        for (size_t i = 5; i < data.size() ; i++)
                        {
                            args.push_back(data[i]);
                        }
                        parsed++;
                        //ParsePrivmsg(userName, command, channelName, args, chantrigger);
                    }
                }
            }
            else
            {
                if (chanpos1 == std::string::npos && chanpos2 != std::string::npos && chanpos3 == std::string::npos)
                {
                    // PRIVMSG userName bot :#channelName command
                    if (data.size() >= 5)
                    {
                        channelName = firstWord;
                        command = data[4];
                        for (size_t i = 5 ; i < data.size() ; i++)
                        {
                            args.push_back(data[i]);
                        }
                        parsed++;
                        //ParsePrivmsg(userName, command, channelName, args, chantrigger);
                    }
                }
                else if (chanpos1 == std::string::npos && chanpos2 == std::string::npos && chanpos3 != std::string::npos)
                {
                    // PRIVMSG userName bot :command #channelName
                    if (data.size() >= 5)
                    {
                        channelName = data[4];
                        command = firstWord;
                        for (size_t i = 5; i < data.size() ; i++)
                        {
                            args.push_back(data[i]);
                        }
                        parsed++;
                        //ParsePrivmsg(userName, command, channelName, args, chantrigger);
                    }
                }
                else if (chanpos1 == std::string::npos && chanpos2 == std::string::npos && chanpos3 == std::string::npos)
                {
                    // PRIVMSG userName bot :command
                    if (data.size() >= 4)
                    {
                        channelName = ""; // not needed?
                        command = firstWord;
                        for (size_t i = 4 ; i < data.size() ; i++)
                        {
                            args.push_back(data[i]);
                        }
                        parsed++;
                        //ParsePrivmsg(userName, command, channelName, args, chantrigger);
                    }
                }
            }
        }
        if (parsed > 1)
        {
            output::instance().addStatus(false, "void channelbot::parsePrivmsg() parsed > 1???");
        }
        if (parsed != 1)
        {
            continue;
        }

        std::string userAuth = "";
        int userChannelAccess = -1;
        std::shared_ptr<user> l_User = users::instance().get(userName);
        if (l_User != nullptr)
        {
            userAuth = l_User->getAuth().first;
        }
        /*if (users::instance().findUser(userName))
        {
            userAuth = users::instance().getUser(userName).getAuth();
        }*/
        std::string overwatchString = "[" + output::instance().sFormatTime("%d-%m-%Y %H:%M:%S") + "] [" + userName + ":" + userAuth + "] ";

        // if there is a channel, add it to the string, including the amount of access the user has to this channel
        if (channelName != "")
        {
            //userChannelAccess = channelbotchannels.getChannel(channelName).getAccess(userAuth);
            overwatchString = overwatchString + "[" + channelName + ":" + glib::stringFromInt(userChannelAccess) + "] ";
        }
        else
        {
            overwatchString = overwatchString + "[no channel] ";
        }

        // if god bla bla
        if (false)
        {
            overwatchString = overwatchString + "[G] ";
        }
        // command is still the command alias
        overwatchString = overwatchString + command;

        // parse the irc command alias to a command
        int access = 1000;
        binds::bindelement bindElement;
        if (binds::instance().getBind(bindElement, command, mNAME))
        {
            command = bindElement.command;
            access = bindElement.access;
        }
        else
        {
            command = "";
            //return;
        }
        // command is now the alias parsed to a command
        overwatchString = overwatchString + ":" + command + "(" + glib::stringFromInt(access) + ")";
        // put all the remaining arguments in a string
        for (size_t argsIterator = 0; argsIterator < args.size(); argsIterator++)
        {
            overwatchString = overwatchString + " " + args[argsIterator];
        }
        irc::instance().addLowPrioritySendQueue(reply::instance().ircPrivmsg(configreader::instance().getString("overwatchchannel"), overwatchString));



        //authName
        if (command == "authName")
        {
            if (args.size() == 0)
            {
                irc::instance().addSendQueue(reply::instance().ircWhois(userName));
            }
            else
            {
                //help(command);
            }
        }

        //ping
        if (command == "ping")
        {
            if (args.size() == 0)
            {
                irc::instance().addSendQueue(reply::instance().ircPrivmsg(channelName, userName + " PONG"));
            }
            else
            {
                //help(command);
            }
        }

        //version
        if (command == "version")
        {
            if (args.size() == 0)
            {
                version(channelName, userName);
            }
            else
            {
                //help(command);
            }
        }

    /*    //uptime
        if (command == "uptime")
        {
            if (args.size() == 0)
            {
                uptime(channelName, userName, authName, bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //up
        if (command == "up")
        {
            if (args.size() == 0)
            {
                up(channelName, userName, authName, bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //down
        if (command == "down")
        {
            if (args.size() == 0)
            {
                down(channelName, userName, authName, bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //resync
        if (command == "resync")
        {
            if (args.size() == 0)
            {
                resync(channelName, userName, authName, bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //access
        if (command == "access")
        {
            if (args.size() == 0)
            {
                if (chantrigger >= 0)
                {
                    access(channelName, userName, userName, authName, bind_access);
                }
                else
                {
                    //help(command);
                }
            }
            else if (args.size() == 1)
            {
                if (chantrigger >= 0)
                {
                    access(channelName, userName, args[0], U.GetAuth(args[0]), bind_access);
                }
                else
                {
                    //help(command);
                }
            }
            else
            {
                //help(command);
            }
        }

        //myaccess
        if (command == "myaccess")
        {
            if (args.size() == 0)
            {
                myaccess(userName, userName, authName, bind_access);
            }
            else if (args.size() == 1)
            {
                if (U.GetGod(userName) == 1)
                {
                    myaccess(userName, args[0], U.GetAuth(args[0]), bind_access);
                }
                else
                {
                    std::string returnstring = "NOTICE " + userName + " :" + irc_reply("need_god", U.GetLanguage(userName)) + "\r\n";
                    Send(returnstring);
                }
            }
            else
            {
                //help(command);
            }
        }

        //users
        if (command == "users")
        {
            if (args.size() == 0)
            {
                users(channelName, userName, authName, bind_access);
            }
            else
            {
                //help(command);
            }
        }

        // readchannel
        if (command == "readchannel")
        {
            if (args.size() == 0)
            {
                readchannel(channelName, userName, authName, bind_access);
            }
            else
            {
                //help(command);
            }
        }
    */
        //channelcommands
        if (command == "channelcommands")
        {
            if (args.size() == 0)
            {
                channelCommands(userName, userAuth);
            }
            else
            {
                //help(command);
            }
        }
    /*
        //op
        if (command == "op")
        {
            if (args.size() >= 1)
            {
                op(channelName, userName, authName, args[0], U.GetAuth(args[0]), bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //deop
        if (command == "deop")
        {
            if (args.size() >= 1)
            {
                deop(channelName, userName, authName, args[0], U.GetAuth(args[0]), bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //voice
        if (command == "voice")
        {
            if (args.size() >= 1)
            {
                voice(channelName, userName, authName, args[0], U.GetAuth(args[0]), bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //devoice
        if (command == "devoice")
        {
            if (args.size() >= 1)
            {
                devoice(channelName, userName, authName, args[0], U.GetAuth(args[0]), bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //kickuser
        if (command == "kickuser")
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
                kickuser(channelName, userName, authName, args[0], U.GetAuth(args[0]), reason, bind_access);
            }
            else
            {
                //help(command);
            }
        }

        //deluser
        if (command == "deluser")
        {
            if (args.size() >= 1)
            {
                for (unsigned int args_it = 0; args_it < args.size(); args_it++)
                {
                    deluser(channelName, userName, authName, args[args_it], U.GetAuth(args[args_it]), bind_access);
                }
            }
            else
            {
                //help(command);
            }
        }

        //adduser
        if (command == "adduser")
        {
            if (args.size() >= 2)
            {
                unsigned int last_args_it = args.size() - 1;
                if (fmod(convertString(args[last_args_it]), 1) == 0)
                {
                    for (unsigned int args_it = 0; args_it < last_args_it; args_it++)
                    {
                        adduser(channelName, userName, authName, args[args_it], U.GetAuth(args[args_it]), convertString(args[last_args_it]), bind_access);
                    }
                }
                else
                {
                    //help(command);
                    //help(integer);
                }
            }
            else
            {
                //help(command);
            }
        }

        //changelevel
        if (command == "changelevel")
        {
            if (args.size() >= 2)
            {
                unsigned int last_args_it = args.size() - 1;
                for (unsigned int args_it = 0; args_it < last_args_it; args_it++)
                {
                    changelevel(channelName, userName, authName, args[args_it], U.GetAuth(args[args_it]), convertString(args[last_args_it]), bind_access);
                }
            }
            else
            {
                //help(command);
            }
        }

        // stats
        if (command == "stats")
        {
            if (args.size() == 0)
            {
                stats(userName, authName, bind_access);
            }
            else
            {
                //help(command);
            }
        }

        // set
        if (command == "set")
        {
            if (args.size() == 0)  // set list
            {
                set(channelName, userName, authName, bind_access);
            }
            else if (args.size() == 1)  // set get value
            {
                set(channelName, userName, authName, args[0], bind_access);
            }
            else if (args.size() == 2)  // set set value
            {
                set(channelName, userName, authName, args[0], args[1], bind_access);
            }
            else
            {
                //help(command);
            }
        }*/
    }
}

void channelbot::version(std::string channelName, std::string userName)
{
    std::vector< std::string > versionsVector;
    versionsVector = versions::instance().getVersions();

    for (size_t versionsVectorIterator = 0; versionsVectorIterator < versionsVector.size(); versionsVectorIterator++)
    {
        irc::instance().addSendQueue(reply::instance().ircPrivmsg(channelName, userName + ": " + versionsVector[versionsVectorIterator]));
    }
}
/*
void channelbot::uptime(std::string channelName, std::string userName, std::string authName, int ca)
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
        std::string returnstring = "PRIVMSG " + channelName + " :" + userName + ": Uptime: ";

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

void channelbot::adduser(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string reqauth, int reqaccess, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int access = C.GetAccess(channelName, authName);
        if (access > reqaccess || U.GetGod(userName) == 1)
        {
            if (C.AddAuth(channelName, reqauth))
            {
                std::string UserUuid = U.GetUid(reqnick);
                std::string ChannelUuid = C.GetCid(channelName);
                if ((ChannelUuid != "NULL") && (UserUuid != "NULL") && (reqaccess > 0))
                {
                    C.AddUserToChannel(ChannelUuid, UserUuid, reqaccess);
                    std::string sendstring = "NOTICE " + userName + " :user " + reqnick + "[" + reqauth + "] added to the userlist with " + convertInt(reqaccess) + " access\r\n";
                    Send(sendstring);
                }
            }
        }
    }
}

void channelbot::deluser(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int access = C.GetAccess(channelName, authName);
        int tmpaccess = C.GetAccess(channelName, reqauth);
        if (tmpaccess > 0)
        {
            if (tmpaccess < access || U.GetGod(userName) == 1)
            {
                std::string UserUuid = U.GetUid(reqnick);
                std::string ChannelUuid = C.GetCid(channelName);
                if ((ChannelUuid != "NULL") && (UserUuid != "NULL"))
                {
                    C.DeleteUserFromChannel(ChannelUuid, UserUuid);
                    std::string sendstring = "NOTICE " + userName + " :user " + reqnick + "[" + reqauth + "] deleted from the userlist\r\n";
                    Send(sendstring);
                }
            }
        }
    }
}

void channelbot::changelevel(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string reqauth, int reqaccess, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int oldaccess = C.GetAccess(channelName, reqauth);
        int access = C.GetAccess(channelName, authName);
        if (((access > reqaccess) && (access > oldaccess)) || U.GetGod(userName) == 1)
        {
            std::string UserUuid = U.GetUid(reqnick);
            std::string ChannelUuid = C.GetCid(channelName);
            if ((ChannelUuid != "NULL") && (UserUuid != "NULL") && (reqaccess > 0))
            {
                //std::string sqlstring = "UPDATE users SET access = '" + convertInt(reqaccess) + "' WHERE ChannelUuid = '" + ChannelUuid + "' AND UserUuid = '" + UserUuid + "';";
                C.SetAccess(channelName, reqauth, reqaccess);
                DatabaseData::Instance().UpdateData("users", "access", BotLib::StringFromInt(reqaccess), "ChannelUuid = '" + ChannelUuid + "' AND UserUuid = '" + UserUuid + "';");
                //RawSql(sqlstring);
                std::string sendstring = "NOTICE " + userName + " :user " + reqnick + "[" + reqauth + "] changed access to " + convertInt(reqaccess) + " access\r\n";
                Send(sendstring);
            }
        }
    }
}

void channelbot::op(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string msReqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(channelName, authName);
    if (access >= C.GetGiveops(channelName))
    {
        if (boost::iequals(msReqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(channelName, msReqauth);
            if (tmpaccess < access || U.GetGod(userName) == 1)
            {
                std::string returnstring = "MODE " + channelName + " +o " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + channelName + " +o " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void channelbot::deop(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string msReqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(channelName, authName);
    if (access >= C.GetGiveops(channelName))
    {
        if (boost::iequals(msReqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(channelName, msReqauth);
            if (tmpaccess < access || U.GetGod(userName) == 1)
            {
                std::string returnstring = "MODE " + channelName + " -o " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + channelName + " -o " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void channelbot::voice(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string msReqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(channelName, authName);
    if (access >= C.GetGiveops(channelName))
    {
        if (boost::iequals(msReqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(channelName, msReqauth);
            if (tmpaccess < access || U.GetGod(userName) == 1)
            {
                std::string returnstring = "MODE " + channelName + " +v " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + channelName + " +v " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void channelbot::devoice(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string msReqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(channelName, authName);
    if (access >= C.GetGiveops(channelName))
    {
        if (boost::iequals(msReqauth,"NULL") != true)
        {
            int tmpaccess = C.GetAccess(channelName, msReqauth);
            if (tmpaccess < access || U.GetGod(userName) == 1)
            {
                std::string returnstring = "MODE " + channelName + " -v " + reqnick + "\r\n";
                Send(returnstring);
            }
        }
        else
        {
            std::string returnstring = "MODE " + channelName + " -v " + reqnick + "\r\n";
            Send(returnstring);
        }
    }
}

void channelbot::stats(std::string sNick, std::string sAuth, int iCa)
{

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
    //std::vector< std::string > vAuths = U.GetAuths();
    //for (unsigned int uiAuthsIndex = 0; uiAuthsIndex < vAuths.size(); uiAuthsIndex++)
    //{
    //    if ()
    //}






    std::string sReplyString;
    // ChannelCount
    sReplyString = irc_reply("STATS_CHANNEL_COUNT", U.GetLanguage(sNick));
    sReplyString = irc_reply_replace(sReplyString, "$ChannelCount$", convertInt(iChannelCount));
    Send(Global::Instance().get_Reply().irc_notice(sNick, sReplyString));

    // UserCount
    //sReplyString = irc_reply("STATS_USER_COUNT", U.GetLanguage(sNick));
    //sReplyString = irc_reply_replace(sReplyString, "$UserCount$", convertInt(iUserCount));
    //Send(Global::Instance().get_Reply().irc_notice(sNick, sReplyString));  // not working yet

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

void channelbot::users(std::string channelName, std::string userName, std::string authName, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::vector< std::string > auths = C.GetAuths(channelName);
    sort(auths.begin(), auths.end());
    std::string returnstr;
    std::vector< std::string > sortauths;
    std::vector< int > sortaccess;
    for (unsigned int i = 0 ; i < auths.size(); i++ )
    {
        int access = C.GetAccess(channelName, auths[i]);
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

    unsigned int length = U.GetWidth(userName);
    std::string commandrpl = irc_reply("USERS", U.GetLanguage(userName));
    commandrpl = commandrpl + " " + channelName;
    returnstr = "NOTICE " + userName + " :";
    for (unsigned int l = 0; l < (((length * 2) / 2) - commandrpl.size()/2); l++)
    {
        returnstr = returnstr + " ";
    }
    returnstr = returnstr + commandrpl + "\r\n";
    Send(returnstr);
    for (unsigned int i = 0; i < sortauths.size(); i++)
    {
        returnstr = "NOTICE " + userName + " :" + fillspace(convertInt(sortaccess[i]), length) + sortauths[i] + "\r\n";
        Send(returnstr);
    }
    std::string nousersrpl = irc_reply("NUMBER_USERS", U.GetLanguage(userName));
    nousersrpl = nousersrpl + " " + convertInt(sortauths.size());
    returnstr = "NOTICE " + userName + " :";
    for (unsigned int l = 0; l < (((length * 2) / 2) - nousersrpl.size()/2); l++)
    {
        returnstr = returnstr + " ";
    }
    returnstr = returnstr + nousersrpl + "\r\n";
    Send(returnstr);
}

void channelbot::readchannel(std::string channelName, std::string userName, std::string authName, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::vector< std::string > vNicks = C.GetNicks(channelName);
    for (unsigned int i = 0 ; i < vNicks.size(); i++ )
    {
        int access = 0;
        if (C.GetVoice(channelName, vNicks[i]))
        {
            access = C.GetGivevoice(channelName);
        }
        if (C.GetOp(channelName, vNicks[i]))
        {
            access = C.GetGiveops(channelName);
        }
        if (access > 0)
        {
            adduser(channelName, userName, authName, vNicks[i], U.GetAuth(vNicks[i]), access, ca);
        }
    }
}

void channelbot::kickuser(std::string channelName, std::string userName, std::string authName, std::string reqnick, std::string reqauth, std::string reason, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (boost::iequals(reqauth,"NULL") != true)
    {
        int access = C.GetAccess(channelName, authName);
        int tmpaccess = C.GetAccess(channelName, reqauth);
        if (tmpaccess < access || U.GetGod(userName) == 1)
        {
            std::string returnstring = "KICK " + channelName + " " + reqnick + " :" + reason + "\r\n";
            Send(returnstring);
        }
    }
    else
    {
        std::string returnstring = "KICK " + channelName + " " + reqnick + " :" + reason + "\r\n";
        Send(returnstring);
    }
}

void channelbot::access(std::string channelName, std::string userName, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    int access = C.GetAccess(channelName, reqauth);
    int oaccess = U.GetOaccess(reqnick);
    int god = U.GetGod(reqnick);
    std::string returnstr;
    if (access > 0)
    {
        returnstr = "NOTICE " + userName + " :" + reqnick + "[" + reqauth + "] has " + convertInt(access) + " access to " + channelName + "\r\n";
        Send(returnstr);
    }
    else
    {
        returnstr = "NOTICE " + userName + " :" + reqnick + "[" + reqauth + "] lacks access to " + channelName + "\r\n";
        Send(returnstr);
    }
    if (oaccess > 0)
    {
        returnstr = "NOTICE " + userName + " :" + reqnick + "[" + reqauth + "] has " + convertInt(oaccess) + " oaccess\r\n";
        Send(returnstr);
    }
    if (god > 0)
    {
        returnstr = "NOTICE " + userName + " :" + reqnick + "[" + reqauth + "] has God enabled\r\n";
        Send(returnstr);
    }
}

void channelbot::myaccess(std::string userName, std::string reqnick, std::string reqauth, int ca)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    std::string returnstring;
    unsigned int length = U.GetWidth(userName);
    unsigned int amount = 2;
    unsigned int channel_count = 0;
    std::string commandrpl = irc_reply("myaccess", U.GetLanguage(userName));
    commandrpl = irc_reply_replace(commandrpl, "$userName$", reqnick);
    commandrpl = irc_reply_replace(commandrpl, "$authName$", reqauth);
    returnstring = Global::Instance().get_Reply().irc_notice(userName, centre(commandrpl.size(), amount, length) + commandrpl);
    //returnstring = "NOTICE " + userName + " :" + centre(commandrpl.size(), amount, length) + commandrpl + "\r\n";
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
            returnstring = Global::Instance().get_Reply().irc_notice(userName, fillspace(sortchannels[i], length) + convertInt(access));
            //returnstring = Global::Instance().get_Reply().irc_notice(userName, tmpstring);
            Send(returnstring);
        }
    }

    std::string nochannelsrpl = irc_reply("myaccess_number_channels", U.GetLanguage(userName));
    nochannelsrpl = irc_reply_replace(nochannelsrpl, "$channels$", convertInt(channel_count));
    returnstring = Global::Instance().get_Reply().irc_notice(userName, centre(nochannelsrpl.size(), amount, length) + nochannelsrpl);
    Send(returnstring);
}

// *
// * Command InviteMe
// * Invites user in the channel if user has enough access
// * @param msChannel the channel for his action
// * @param msNick the nickname of the user
// * @param msAuth the authname of the user
// * @param miChannelAccess ChannelAccess needed for this command
// *
// *
void channelbot::InviteMe(std::string msChannel, std::string msNick, std::string msAuth, int miChannelAcess)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    // UsersInterface& U = Global::Instance().get_Users();
    if (C.GetAccess(msChannel, msAuth) >= miChannelAcess)
    {
        std::vector< std::string > _vChannels = C.GetChannels();
        for ( unsigned int _uiChannelsIndex = 0 ; _uiChannelsIndex < _vChannels.size(); _uiChannelsIndex++ )
        {
            Send(Global::Instance().get_Reply().irc_invite(_vChannels[_uiChannelsIndex], msNick));
        }
    }
}
*/
void channelbot::up(std::string channelName, std::string userName, std::string authName, int commandAccess)
{
    std::string l_Reply;
    //bool giveop = false;
    //bool givevoice = false;
    //int access = C.GetAccess(channelName, authName);
//    if (access >= C.GetGiveops(channelName))
//    {
//        //if (C.GetOp(channelName, userName) == false)
//        {
//            std::string returnstring = "MODE " + channelName + " +o " + userName + "\r\n";
//            Send(returnstring);
//            reply_string = "NOTICE " + userName + " :" + irc_reply("up_op", U.GetLanguage(userName)) + "\r\n";
//            reply_string = irc_reply_replace(reply_string, "$channel$", channelName);
//            Send(reply_string);
//        }
//        //else
//        //{
//        //    reply_string = "NOTICE " + userName + " :" + irc_reply("up_already_up", U.GetLanguage(userName)) + "\r\n";
//        //    reply_string = irc_reply_replace(reply_string, "$channel$", channelName);
//        //    Send(reply_string);
//        //}
//    }
//    else if (access >= C.GetGivevoice(channelName))
//    {
//        //if (C.GetVoice(channelName, userName) == false)
//        {
//            std::string returnstring = "MODE " + channelName + " +v " + userName + "\r\n";
//            Send(returnstring);
//            reply_string = "NOTICE " + userName + " :" + irc_reply("up_voice", U.GetLanguage(userName)) + "\r\n";
//            reply_string = irc_reply_replace(reply_string, "$channel$", channelName);
//            Send(reply_string);
//        }
//        //else
//        //{
//        //    reply_string = "NOTICE " + userName + " :" + irc_reply("up_already_up", U.GetLanguage(userName)) + "\r\n";
//        //    reply_string = irc_reply_replace(reply_string, "$channel$", channelName);
//        //    Send(reply_string);
//        //}
//    }
//    if (C.GetOp(channelName, userName) == true && C.GetVoice(channelName, userName) == true)
//    {
//        reply_string = "NOTICE " + userName + " :" + irc_reply("up_already_up", U.GetLanguage(userName)) + "\r\n";
//        reply_string = irc_reply_replace(reply_string, "$channel$", channelName);
//        Send(reply_string);
//    }
}

void channelbot::down(std::string channelName, std::string userName, std::string authName, int commandAccess)
{
//    //ChannelsInterface& C = Global::Instance().get_Channels();
//    UsersInterface& U = Global::Instance().get_Users();
//    std::string reply_string;
//    //if (C.GetOp(channelName, userName) == true && C.GetVoice(channelName, userName) == true)
//    {
//        std::string returnstring = "MODE " + channelName + " -ov " + userName + " " + userName + "\r\n";
//        Send(returnstring);
//        reply_string = "NOTICE " + userName + " :" + irc_reply("down", U.GetLanguage(userName)) + "\r\n";
//    }
////    else if (C.GetVoice(channelName, userName) == true)
////    {
////        std::string returnstring = "MODE " + channelName + " -v " + userName + "\r\n";
////        Send(returnstring);
////        reply_string = "NOTICE " + userName + " :" + irc_reply("down_devoice", U.GetLanguage(userName)) + "\r\n";
////    }
////    else if (C.GetOp(channelName, userName) == true)
////    {
////        std::string returnstring = "MODE " + channelName + " -o " + userName + "\r\n";
////        Send(returnstring);
////        reply_string = "NOTICE " + userName + " :" + irc_reply("down_deop", U.GetLanguage(userName)) + "\r\n";
////    }
////    if (C.GetOp(channelName, userName) == false && C.GetVoice(channelName, userName) == false)
////    {
////        reply_string = "NOTICE " + userName + " :" + irc_reply("down_already_down", U.GetLanguage(userName)) + "\r\n";
////    }
//    reply_string = irc_reply_replace(reply_string, "$channel$", channelName);
//    Send(reply_string);
}
/*

// **
// * Command Resync
// * Syncs all users with their access in the channel
// * @param msChannel the channel for his action
// * @param msNick the nickname of the user
// * @param msAuth the authname of the user
// * @param miChannelAccess ChannelAccess needed for this command
// *
// *
void channelbot::resync(std::string msChannel, std::string msNick, std::string msAuth, int miChannelAcess)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    UsersInterface& U = Global::Instance().get_Users();
    if (C.GetAccess(msChannel, msAuth) >= miChannelAcess)
    {
        std::vector< std::string > _vNicks = C.GetNicks(msChannel);
        int _iAccess ;
        for (unsigned int i = 0; i < _vNicks.size(); i++)
        {
            if (!boost::iequals(Global::Instance().get_BotNick(), _vNicks[i]))
            {
                _iAccess = C.GetAccess(msChannel, U.GetAuth(_vNicks[i]));
                if (_iAccess >= C.GetGiveops(msChannel))
                {
                    //if (C.GetOp(msChannel, _vNicks[i]) == false)
                    {
                        Send(Global::Instance().get_Reply().irc_mode(msChannel, "+o " + _vNicks[i]));
                    }
                }
                else if (_iAccess >= C.GetGivevoice(msChannel))
                {
                    //if (C.GetVoice(msChannel, _vNicks[i]) == false)
                    {
                        Send(Global::Instance().get_Reply().irc_mode(msChannel, "+v " + _vNicks[i]));
                    }
                }
                if ((_iAccess < C.GetGiveops(msChannel)) && (_iAccess < C.GetGivevoice(msChannel)))
                {
                    //if (C.GetOp(msChannel, _vNicks[i]) == true || C.GetVoice(msChannel, _vNicks[i]) == true)
                    {
                        Send(Global::Instance().get_Reply().irc_mode(msChannel, "-ov " + _vNicks[i] + " " + _vNicks[i]));
                    }
                }
                else if (_iAccess < C.GetGivevoice(msChannel))
                {
                    //if (C.GetVoice(msChannel, _vNicks[i]) == false)
                    {
                        Send(Global::Instance().get_Reply().irc_mode(msChannel, "-v " + _vNicks[i]));
                    }
                }
                else if (_iAccess < C.GetGiveops(msChannel))
                {
                    //if (C.GetOp(msChannel, _vNicks[i]) == false)
                    {
                        Send(Global::Instance().get_Reply().irc_mode(msChannel, "-o " + _vNicks[i]));
                    }
                }
            }
        }
    }
}

// set List
void channelbot::set(std::string msChannel, std::string msNick, std::string msAuth, int miChannelAcess)
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
void channelbot::set(std::string msChannel, std::string msNick, std::string msAuth, std::string msSetting, int miChannelAcess)
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
void channelbot::set(std::string msChannel, std::string msNick, std::string msAuth, std::string msSetting, std::string msValue, int miChannelAcess)
{
    ChannelsInterface& C = Global::Instance().get_Channels();
    //UsersInterface& U = Global::Instance().get_Users();
    int iAccess = C.GetAccess(msChannel, msAuth);
    if (iAccess >= miChannelAcess)
    {
        if (boost::iequals(msSetting, "giveops") || boost::iequals(msSetting, "give_ops"))
        {
            C.SetSetting(msChannel, "giveops", msValue);
        }
        if (boost::iequals(msSetting, "givevoice") || boost::iequals(msSetting, "give_voice"))
        {
            C.SetSetting(msChannel, "givevoice", msValue);
        }
        if (boost::iequals(msSetting, "setters"))
        {
            C.SetSetting(msChannel, "setters", msValue);
        }
    }
    else
    {
        // need more access;
    }
}
*/


void channelbot::channelCommands(std::string userName, std::string userAuth)
{
    std::map< std::string, binds::bindelement > bindElement;
    std::map< std::string, binds::bindelement >::iterator bindElementIterator;
    if (binds::instance().getBinds(bindElement, mNAME))
    {
        for ( bindElementIterator = bindElement.begin() ; bindElementIterator != bindElement.end(); bindElementIterator++ )
        {
            irc::instance().addSendQueue(reply::instance().ircNotice(userName, bindElementIterator->first + " " + bindElementIterator->second.command + " " + glib::stringFromInt(bindElementIterator->second.access)));
        }
    }
}

/*
// *
// * Event Invite
// * when the bot gets invited join this channel
// * @param mvRawIrcData the raw irc data
// *
// *
void channelbot::INVITE(std::vector< std::string > mvRawIrcData)
{
    Send(Global::Instance().get_Reply().irc_join(mvRawIrcData[3]));
}



// *
// * Event User Join
// * when a user joins a channel process them.
// * @param msChan the channel the user joined
// * @param msNick the nickname of the joined user
// *
// *
void channelbot::OnUserJoin(std::string msChan, std::string msNick)
{
    UsersInterface& U = Global::Instance().get_Users();
    ChannelsInterface& C = Global::Instance().get_Channels();
    int _iAccess = C.GetAccess(msChan, U.GetAuth(msNick));

    if (_iAccess >= C.GetGiveops(msChan))
    {
        {
            Send(Global::Instance().get_Reply().irc_mode(msChan, "+o " + msNick));
        }
    }
    else if (_iAccess >= C.GetGivevoice(msChan))
    {
        {
            Send(Global::Instance().get_Reply().irc_mode(msChan, "+v " + msNick));
        }
    }

    // autoinvite
    if (U.FirstJoin(msNick))
    {
        std::vector< std::string > _vChannels = C.GetChannels();
        for ( unsigned int _uiChannelsIndex = 0 ; _uiChannelsIndex < _vChannels.size(); _uiChannelsIndex++ )
        {
            if (C.GetAccess(_vChannels[_uiChannelsIndex], U.GetAuth(msNick)) > 0)
            {
                if (true)   // if not in channel.
                {
                    // if (C.GetAutoInvite(_vChannels[_uiChannelsIndex], U.GetAuth(msNick)))
                    {
                        Send(Global::Instance().get_Reply().irc_invite(_vChannels[_uiChannelsIndex], msNick));
                    }
                }
            }
        }
    }
    if (_iAccess > 0)
    {
        if (true)   // setinfo
        {
            std::string dummy = "dummy";
            std::string _sInfoText = "[" + dummy + "] " + "setinfo test";
//            Send(Global::Instance().get_Reply().irc_privmsg(msChan, _sInfoText));
        }
    }
}

void channelbot::timerrun()
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

void channelbot::timerlong()
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
}*/

bool channelbot::nickFromHostmask(std::string& data)
{
    std::vector< std::string > who;
    who = glib::split(data);
    if (deleteFirst(who[0], ":"))
    {
        size_t pos;
        pos = who[0].find("!");
        data = who[0].substr(0, pos);
        return true;
    }
    else
    {
        return false;
    }
}


bool channelbot::deleteFirst(std::string& data, std::string character)
{
    size_t pos;
    std::string tmpstring = "";
    pos = data.find(character);
    if (pos > 2 && pos != std::string::npos)
    {
        tmpstring = data.substr(0, pos-1);
    }
    tmpstring = tmpstring + data.substr(pos+1);
    data = tmpstring;
    return true;
}
