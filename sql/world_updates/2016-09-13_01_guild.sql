/*
Navicat MySQL Data Transfer

Source Server         : localhost_3308
Source Server Version : 50045
Source Host           : localhost:3308
Source Database       : cata_characters

Target Server Type    : MYSQL
Target Server Version : 50045
File Encoding         : 65001

Date: 2016-09-13 23:43:56
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `guild`
-- ----------------------------
DROP TABLE IF EXISTS `guild`;
CREATE TABLE `guild` (
  `guildid` int(10) unsigned NOT NULL default '0',
  `name` varchar(24) NOT NULL default '',
  `leaderguid` int(10) unsigned NOT NULL default '0',
  `EmblemStyle` tinyint(3) unsigned NOT NULL default '0',
  `EmblemColor` tinyint(3) unsigned NOT NULL default '0',
  `BorderStyle` tinyint(3) unsigned NOT NULL default '0',
  `BorderColor` tinyint(3) unsigned NOT NULL default '0',
  `BackgroundColor` tinyint(3) unsigned NOT NULL default '0',
  `info` text NOT NULL,
  `motd` varchar(128) NOT NULL default '',
  `createdate` int(10) unsigned NOT NULL default '0',
  `BankMoney` bigint(20) unsigned NOT NULL default '0',
  `level` int(10) unsigned default '1',
  `experience` bigint(20) unsigned default '0',
  `todayExperience` bigint(20) unsigned default '0',
  PRIMARY KEY  (`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- ----------------------------
-- Records of guild
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_achievement`
-- ----------------------------
DROP TABLE IF EXISTS `guild_achievement`;
CREATE TABLE `guild_achievement` (
  `guildId` int(10) unsigned NOT NULL,
  `achievement` smallint(5) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL default '0',
  `guids` text NOT NULL,
  PRIMARY KEY  (`guildId`,`achievement`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guild_achievement
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_achievement_progress`
-- ----------------------------
DROP TABLE IF EXISTS `guild_achievement_progress`;
CREATE TABLE `guild_achievement_progress` (
  `guildId` int(10) unsigned NOT NULL,
  `criteria` smallint(5) unsigned NOT NULL,
  `counter` int(10) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL default '0',
  `completedGuid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`guildId`,`criteria`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guild_achievement_progress
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_bank_eventlog`
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_eventlog`;
CREATE TABLE `guild_bank_eventlog` (
  `guildid` int(10) unsigned NOT NULL default '0' COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL default '0' COMMENT 'Log record identificator - auxiliary column',
  `TabId` tinyint(3) unsigned NOT NULL default '0' COMMENT 'Guild bank TabId',
  `EventType` tinyint(3) unsigned NOT NULL default '0' COMMENT 'Event type',
  `PlayerGuid` int(10) unsigned NOT NULL default '0',
  `ItemOrMoney` int(10) unsigned NOT NULL default '0',
  `ItemStackCount` smallint(5) unsigned NOT NULL default '0',
  `DestTabId` tinyint(3) unsigned NOT NULL default '0' COMMENT 'Destination Tab Id',
  `TimeStamp` int(10) unsigned NOT NULL default '0' COMMENT 'Event UNIX time',
  PRIMARY KEY  (`guildid`,`LogGuid`,`TabId`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_PlayerGuid` (`PlayerGuid`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guild_bank_eventlog
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_bank_item`
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_item`;
CREATE TABLE `guild_bank_item` (
  `guildid` int(10) unsigned NOT NULL default '0',
  `TabId` tinyint(3) unsigned NOT NULL default '0',
  `SlotId` tinyint(3) unsigned NOT NULL default '0',
  `item_guid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`guildid`,`TabId`,`SlotId`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_item_guid` (`item_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guild_bank_item
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_bank_right`
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_right`;
CREATE TABLE `guild_bank_right` (
  `guildid` int(10) unsigned NOT NULL default '0',
  `TabId` tinyint(3) unsigned NOT NULL default '0',
  `rid` tinyint(3) unsigned NOT NULL default '0',
  `gbright` tinyint(3) unsigned NOT NULL default '0',
  `SlotPerDay` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`guildid`,`TabId`,`rid`),
  KEY `guildid_key` (`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guild_bank_right
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_bank_tab`
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_tab`;
CREATE TABLE `guild_bank_tab` (
  `guildid` int(10) unsigned NOT NULL default '0',
  `TabId` tinyint(3) unsigned NOT NULL default '0',
  `TabName` varchar(16) NOT NULL default '',
  `TabIcon` varchar(100) NOT NULL default '',
  `TabText` varchar(500) default NULL,
  PRIMARY KEY  (`guildid`,`TabId`),
  KEY `guildid_key` (`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guild_bank_tab
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_eventlog`
-- ----------------------------
DROP TABLE IF EXISTS `guild_eventlog`;
CREATE TABLE `guild_eventlog` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL COMMENT 'Log record identificator - auxiliary column',
  `EventType` tinyint(3) unsigned NOT NULL COMMENT 'Event type',
  `PlayerGuid1` int(10) unsigned NOT NULL COMMENT 'Player 1',
  `PlayerGuid2` int(10) unsigned NOT NULL COMMENT 'Player 2',
  `NewRank` tinyint(3) unsigned NOT NULL COMMENT 'New rank(in case promotion/demotion)',
  `TimeStamp` int(10) unsigned NOT NULL COMMENT 'Event UNIX time',
  PRIMARY KEY  (`guildid`,`LogGuid`),
  KEY `Idx_PlayerGuid1` (`PlayerGuid1`),
  KEY `Idx_PlayerGuid2` (`PlayerGuid2`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Guild Eventlog';

-- ----------------------------
-- Records of guild_eventlog
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_finder_applicant`
-- ----------------------------
DROP TABLE IF EXISTS `guild_finder_applicant`;
CREATE TABLE `guild_finder_applicant` (
  `guildId` int(10) unsigned default NULL,
  `playerGuid` int(10) unsigned default NULL,
  `availability` tinyint(3) unsigned default '0',
  `classRole` tinyint(3) unsigned default '0',
  `interests` tinyint(3) unsigned default '0',
  `comment` varchar(255) default NULL,
  `submitTime` int(10) unsigned default NULL,
  UNIQUE KEY `guildId` (`guildId`,`playerGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of guild_finder_applicant
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_finder_guild_settings`
-- ----------------------------
DROP TABLE IF EXISTS `guild_finder_guild_settings`;
CREATE TABLE `guild_finder_guild_settings` (
  `guildId` int(10) unsigned NOT NULL,
  `availability` tinyint(3) unsigned NOT NULL default '0',
  `classRoles` tinyint(3) unsigned NOT NULL default '0',
  `interests` tinyint(3) unsigned NOT NULL default '0',
  `level` tinyint(3) unsigned NOT NULL default '1',
  `listed` tinyint(3) unsigned NOT NULL default '0',
  `comment` varchar(255) default NULL,
  PRIMARY KEY  (`guildId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of guild_finder_guild_settings
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_member`
-- ----------------------------
DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE `guild_member` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `playerguid` int(10) unsigned NOT NULL,
  `rank` tinyint(3) unsigned NOT NULL,
  `pnote` varchar(31) NOT NULL default '',
  `offnote` varchar(31) NOT NULL default '',
  UNIQUE KEY `guid_key` (`playerguid`),
  KEY `guildid_key` (`guildid`),
  KEY `guildid_rank_key` (`guildid`,`rank`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- ----------------------------
-- Records of guild_member
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_member_withdraw`
-- ----------------------------
DROP TABLE IF EXISTS `guild_member_withdraw`;
CREATE TABLE `guild_member_withdraw` (
  `guid` int(10) unsigned NOT NULL,
  `tab0` int(10) unsigned NOT NULL default '0',
  `tab1` int(10) unsigned NOT NULL default '0',
  `tab2` int(10) unsigned NOT NULL default '0',
  `tab3` int(10) unsigned NOT NULL default '0',
  `tab4` int(10) unsigned NOT NULL default '0',
  `tab5` int(10) unsigned NOT NULL default '0',
  `tab6` int(10) unsigned NOT NULL default '0',
  `tab7` int(10) unsigned NOT NULL default '0',
  `money` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Guild Member Daily Withdraws';

-- ----------------------------
-- Records of guild_member_withdraw
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_newslog`
-- ----------------------------
DROP TABLE IF EXISTS `guild_newslog`;
CREATE TABLE `guild_newslog` (
  `guildid` int(10) unsigned NOT NULL default '0' COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL default '0' COMMENT 'Log record identificator - auxiliary column',
  `EventType` tinyint(3) unsigned NOT NULL default '0' COMMENT 'Event type',
  `PlayerGuid` int(10) unsigned NOT NULL default '0',
  `Flags` int(10) unsigned NOT NULL default '0',
  `Value` int(10) unsigned NOT NULL default '0',
  `TimeStamp` int(10) unsigned NOT NULL default '0' COMMENT 'Event UNIX time',
  PRIMARY KEY  (`guildid`,`LogGuid`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_PlayerGuid` (`PlayerGuid`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guild_newslog
-- ----------------------------

-- ----------------------------
-- Table structure for `guild_rank`
-- ----------------------------
DROP TABLE IF EXISTS `guild_rank`;
CREATE TABLE `guild_rank` (
  `guildid` int(10) unsigned NOT NULL default '0',
  `rid` tinyint(3) unsigned NOT NULL,
  `rname` varchar(20) NOT NULL default '',
  `rights` mediumint(8) unsigned NOT NULL default '0',
  `BankMoneyPerDay` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`guildid`,`rid`),
  KEY `Idx_rid` (`rid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- ----------------------------
-- Records of guild_rank
-- ----------------------------

UPDATE `world_db_version` SET `LastUpdate`='2016-09-13_01_guild' WHERE (`LastUpdate`='2016-06-26_01_trainers');
