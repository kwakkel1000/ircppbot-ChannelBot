SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
 -- --------------------------------------------------------

 --
 -- Table structure for table `channelbotauths`
 -- 

CREATE TABLE IF NOT EXISTS `channelbotauths` (
  `channel` varchar(64) NOT NULL,
  `auth` varchar(64) NOT NULL,
  `access` int(8) NOT NULL DEFAULT '0',
  `noautoop` varchar(8) NOT NULL DEFAULT 'false',
  `autoinvite` varchar(8) NOT NULL DEFAULT 'false',
  `info` varchar(256) NOT NULL,
  PRIMARY KEY (`channel`,`auth`) ) ENGINE=MyISAM DEFAULT CHARSET=latin1;

 -- --------------------------------------------------------

 --
 -- Table structure for table `channelbotchannels`
 -- 

CREATE TABLE IF NOT EXISTS `channelbotchannels` (
  `channel` varchar(64) NOT NULL,
  `giveops` int(11) NOT NULL DEFAULT '300',
  `givevoice` int(11) NOT NULL DEFAULT '100',
  `setters` int(8) NOT NULL DEFAULT '400',
  `inviteme` int(1) NOT NULL DEFAULT '100',
  `enforceops` int(8) NOT NULL DEFAULT '350',
  `greeting` varchar(64) NOT NULL,
  `usergreeting` varchar(64) NOT NULL,
  `modes` varchar(32) NOT NULL DEFAULT '+tn',
  `enforcemodes` int(8) NOT NULL DEFAULT '450',
  `publiccommands` int(8) NOT NULL DEFAULT '0',
  `toys` int(1) NOT NULL DEFAULT '2',
  `dynamiclimit` int(8) NOT NULL DEFAULT '0',
  `successor` int(8) NOT NULL DEFAULT '499',
  `nodelete` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`channel`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
