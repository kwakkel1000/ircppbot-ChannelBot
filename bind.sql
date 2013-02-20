SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
-- --------------------------------------------------------

--
-- Table structure for table `binds`
-- 

CREATE TABLE IF NOT EXISTS `binds` (
  `command` varchar(32) NOT NULL,
  `alias` varchar(32) NOT NULL,
  `who` varchar(32) NOT NULL,
  `access` int(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`alias`),
  UNIQUE KEY `alias` (`alias`,`who`) ) ENGINE=MyISAM DEFAULT CHARSET=latin1;

INSERT INTO `binds` (`command`, `alias`, `who`, `access`) VALUES 
('reloadchan', 'reloadchan', 'channelbot', 400), 
('channelcommands', 'commands', 'channelbot', 0), 
('adduser', 'adduser', 'channelbot', 300), 
('adduser', 'add', 'channelbot', 300), 
('users', 'users', 'channelbot', 1), 
('deluser', 'deluser', 'channelbot', 300), 
('deluser', 'del', 'channelbot', 300), 
('changelevel', 'changelevel', 'channelbot', 300), 
('changelevel', 'clvl', 'channelbot', 300), 
('op', 'op', 'channelbot', 300), 
('deop', 'deop', 'channelbot', 300), 
('voice', 'voice', 'channelbot', 250), 
('devoice', 'devoice', 'channelbot', 250), 
('auth', 'auth', 'channelbot', 0), 
('ping', 'ping', 'channelbot', 1), 
('up', 'up', 'channelbot', 1), 
('down', 'down', 'channelbot', 1), 
('resync', 'resync', 'channelbot', 300), 
('resync', 'sync', 'channelbot', 300), 
('version', 'version', 'channelbot', 0), 
('access', 'access', 'channelbot', 0), 
('access', 'a', 'channelbot', 0), 
('myaccess', 'myaccess', 'channelbot', 0), 
('myaccess', 'ma', 'channelbot', 0), 
('kickuser', 'kick', 'channelbot', 300), 
('kickuser', 'k', 'channelbot', 300), 
('kickban', 'kb', 'channelbot', 200), 
('uptime', 'uptime', 'channelbot', 1), 
('readchannel', 'readchannel', 'channelbot', 400), 
('stats', 'stats', 'channelbot', 0),
('set', 'set', 'channelbot', 501);
