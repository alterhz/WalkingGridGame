/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50620
Source Host           : localhost:3306
Source Database       : gamedb_gather

Target Server Type    : MYSQL
Target Server Version : 50620
File Encoding         : 65001

Date: 2015-07-07 18:20:45
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for accountreg
-- ----------------------------
DROP TABLE IF EXISTS `accountreg`;
CREATE TABLE `accountreg` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `platform` varchar(255) DEFAULT NULL,
  `account` varchar(255) DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for arena
-- ----------------------------
DROP TABLE IF EXISTS `arena`;
CREATE TABLE `arena` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `roleid` int(11) DEFAULT NULL,
  `rolename` varchar(255) DEFAULT NULL,
  `rolelevel` int(11) DEFAULT NULL,
  `result` int(11) DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for bagextend
-- ----------------------------
DROP TABLE IF EXISTS `bagextend`;
CREATE TABLE `bagextend` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `roleid` int(11) DEFAULT NULL COMMENT '角色id',
  `rolename` varchar(255) DEFAULT NULL COMMENT '角色名称',
  `rolesn` int(11) DEFAULT NULL COMMENT '角色种族',
  `type` int(11) DEFAULT NULL COMMENT '扩充背包类型，道具扩充为特殊类型',
  `extendnum` int(11) DEFAULT '0' COMMENT '扩充格子数量',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for buycoin
-- ----------------------------
DROP TABLE IF EXISTS `buycoin`;
CREATE TABLE `buycoin` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `roleid` int(11) DEFAULT NULL COMMENT '角色Id',
  `rolename` varchar(255) DEFAULT NULL COMMENT '角色名称',
  `rolesn` int(11) DEFAULT NULL,
  `rolelevel` int(11) DEFAULT NULL COMMENT '角色等级',
  `viplevel` int(11) DEFAULT NULL COMMENT 'vip等级',
  `spenddiamond` int(11) DEFAULT NULL COMMENT '花费砖石数量',
  `coin` int(11) DEFAULT NULL COMMENT '购买游戏币数量',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for buyinstance
-- ----------------------------
DROP TABLE IF EXISTS `buyinstance`;
CREATE TABLE `buyinstance` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `roleid` int(11) DEFAULT NULL COMMENT '角色Id',
  `rolename` varchar(255) DEFAULT NULL COMMENT '角色名称',
  `rolesn` int(11) DEFAULT NULL COMMENT '角色种族',
  `rolelevel` int(11) DEFAULT NULL COMMENT '角色等级',
  `viplevel` int(11) DEFAULT NULL COMMENT 'vip等级',
  `instancesn` int(11) DEFAULT NULL COMMENT '副本SN',
  `buycount` int(11) DEFAULT NULL COMMENT '购买数量',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for coin
-- ----------------------------
DROP TABLE IF EXISTS `coin`;
CREATE TABLE `coin` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `roleid` int(11) DEFAULT NULL,
  `rolename` varchar(255) DEFAULT NULL,
  `rolelevel` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `subtype` int(11) DEFAULT NULL,
  `changecoin` int(11) DEFAULT NULL COMMENT '变化游戏币',
  `totalcoin` int(11) DEFAULT NULL COMMENT '剩余游戏币',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for deposit
-- ----------------------------
DROP TABLE IF EXISTS `deposit`;
CREATE TABLE `deposit` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `platform` varchar(32) DEFAULT NULL,
  `account` varchar(64) DEFAULT NULL,
  `roleid` int(11) DEFAULT NULL,
  `rolename` varchar(64) DEFAULT NULL,
  `rolesn` int(11) DEFAULT NULL,
  `rolelevel` int(11) DEFAULT NULL,
  `viplevel` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `diamond` int(11) DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for diamond
-- ----------------------------
DROP TABLE IF EXISTS `diamond`;
CREATE TABLE `diamond` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account` varchar(255) DEFAULT NULL COMMENT '账号',
  `roleid` int(11) DEFAULT NULL COMMENT '角色ID',
  `rolename` varchar(255) DEFAULT NULL COMMENT '角色名称',
  `rolelevel` varchar(255) DEFAULT NULL COMMENT '角色等级',
  `type` int(11) DEFAULT NULL COMMENT '产出|消耗',
  `subtype` int(11) DEFAULT NULL COMMENT '产出途径（充值、游戏内部奖励等）|消耗途径（扩容、购买等）',
  `changediamond` int(11) DEFAULT NULL COMMENT '数量',
  `totaldiamond` int(11) DEFAULT NULL COMMENT '改变后的钻石',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for equip
-- ----------------------------
DROP TABLE IF EXISTS `equip`;
CREATE TABLE `equip` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `roleid` int(11) DEFAULT NULL COMMENT '角色id',
  `rolename` varchar(255) DEFAULT NULL COMMENT '角色名称',
  `equipsn` int(11) DEFAULT NULL COMMENT '装备SN',
  `equipid` int(11) DEFAULT NULL COMMENT '装备Id',
  `maketype` int(11) DEFAULT NULL COMMENT '打造类型',
  `databefore` varchar(1024) DEFAULT NULL COMMENT '打造前装备数据',
  `dataafter` varchar(1024) DEFAULT NULL COMMENT '打造后装备数据',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for instance
-- ----------------------------
DROP TABLE IF EXISTS `instance`;
CREATE TABLE `instance` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `instancesn` int(11) DEFAULT NULL COMMENT '副本sn',
  `result` int(11) DEFAULT NULL COMMENT '副本结果：通关，失败',
  `passtime` int(11) DEFAULT NULL COMMENT '通关用时',
  `deadtimes` int(11) DEFAULT NULL COMMENT '死亡次数',
  `roleidlist` varchar(255) DEFAULT NULL COMMENT '副本角色列表',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for item
-- ----------------------------
DROP TABLE IF EXISTS `item`;
CREATE TABLE `item` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `itemsn` int(11) DEFAULT NULL COMMENT '道具SN',
  `itemtype` int(11) DEFAULT NULL COMMENT '道具类型',
  `num` int(11) DEFAULT NULL COMMENT '数量',
  `type` int(11) DEFAULT NULL COMMENT '产出|消耗',
  `way` int(11) DEFAULT NULL COMMENT '产出途径|消耗途径',
  `roleid` int(11) DEFAULT NULL COMMENT '角色Id',
  `rolename` varchar(255) DEFAULT NULL,
  `rolelevel` int(11) DEFAULT NULL COMMENT '角色等级',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `itemattr` varchar(1024) DEFAULT NULL COMMENT '装备类道具属性',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for login
-- ----------------------------
DROP TABLE IF EXISTS `login`;
CREATE TABLE `login` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `platform` varchar(255) DEFAULT NULL,
  `account` varchar(255) DEFAULT NULL,
  `rolesn` int(11) DEFAULT NULL,
  `rolelevel` int(11) unsigned zerofill DEFAULT NULL,
  `rolename` varchar(255) DEFAULT NULL,
  `onlinetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `offlinetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for relive
-- ----------------------------
DROP TABLE IF EXISTS `relive`;
CREATE TABLE `relive` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `roleid` int(11) DEFAULT NULL COMMENT '角色ID',
  `rolename` varchar(255) DEFAULT NULL COMMENT '角色名称',
  `rolelevel` int(11) DEFAULT NULL COMMENT '角色等级',
  `instancesn` int(11) DEFAULT NULL COMMENT '场景SN',
  `wastediamond` int(11) DEFAULT NULL COMMENT '消耗砖石',
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for revision
-- ----------------------------
DROP TABLE IF EXISTS `revision`;
CREATE TABLE `revision` (
  `workdb_revision` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for rolecreate
-- ----------------------------
DROP TABLE IF EXISTS `rolecreate`;
CREATE TABLE `rolecreate` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `platform` varchar(255) DEFAULT NULL,
  `account` varchar(255) DEFAULT NULL,
  `rolesn` int(11) DEFAULT NULL,
  `rolename` varchar(255) DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
