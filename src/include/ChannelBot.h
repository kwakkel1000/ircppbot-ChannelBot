#ifndef ChannelBot_h
#define ChannelBot_h
#include <core/ModuleBase.h>
#include <interfaces/WhoisDataContainerInterface.h>
#include <interfaces/DataInterface.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

class DataInterface;
class WhoisDataContainerInterface;
class ChannelBot : public ModuleBase
{
public:
    ChannelBot();
    ~ChannelBot();
    void read();
    void stop();
    void Init(DataInterface* pData);
    void timerrun();

private:

    DataInterface* mpDataInterface;
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

    void adduser(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int reqaccess, int ca);
    void deluser(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int ca);
    void changelevel(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int reqaccess, int ca);
    void access(std::string chan, std::string mNick, std::string reqnick, std::string reqauth, int ca);
    void myaccess(std::string mNick, std::string reqnick, std::string reqauth, int ca);
    void ping(std::string chan, std::string mNick, std::string auth, int ca);
    void version(std::string chan, std::string mNick, std::string auth, int ca);
    void uptime(std::string chan, std::string mNick, std::string auth, int ca);
    void authme(std::string mNick, int ca);
    void up(std::string chan, std::string mNick, std::string auth, int ca);
    void down(std::string chan, std::string mNick, std::string auth, int ca);
    void resync(std::string chan, std::string mNick, std::string auth, int ca);
    void users(std::string chan, std::string mNick, std::string auth, int ca);
    void readchannel(std::string chan, std::string nick, std::string auth, int ca);
    void kickuser(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, std::string reason, int ca);

    void op(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int ca);
    void deop(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int ca);
    void voice(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int ca);
    void devoice(std::string chan, std::string mNick, std::string auth, std::string reqnick, std::string reqauth, int ca);

    void channelcommands(std::string mNick, std::string auth, int ca);

    void OnUserJoin(std::string chan, std::string mNick);

    //void DBreplyinit();

    void timerlong();
    std::vector< int > timer_sec;
    std::vector< std::string > timer_command;
    std::vector< int > timer_long_sec;
    std::vector< std::string > timer_long_command;
    int longtime;

    boost::mutex parse_mutex;
    bool run;
    std::string channelbottrigger;
    std::string command_table;

    boost::shared_ptr< boost::thread > raw_parse_thread;
    boost::shared_ptr< boost::thread > privmsg_parse_thread;
    boost::shared_ptr< boost::thread > whois_loop_thread;
};

#endif // ChannelBot_h
