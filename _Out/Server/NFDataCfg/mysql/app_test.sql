/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50543
Source Host           : localhost:3306
Source Database       : app_test

Target Server Type    : MYSQL
Target Server Version : 50543
File Encoding         : 65001

Date: 2015-06-28 15:44:21
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `accountinfo`
-- ----------------------------
DROP TABLE IF EXISTS `accountinfo`;
CREATE TABLE `accountinfo` (
  `ID` varchar(128) NOT NULL,
  `Password` varchar(128) NOT NULL DEFAULT '',
  `RoleID` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of accountinfo
-- ----------------------------
INSERT INTO `accountinfo` VALUES ('test1', '123456', '');
INSERT INTO `accountinfo` VALUES ('test10', '123456', '');
INSERT INTO `accountinfo` VALUES ('test11', '123456', '');
INSERT INTO `accountinfo` VALUES ('test12', '123456', '');
INSERT INTO `accountinfo` VALUES ('test13', '123456', '');
INSERT INTO `accountinfo` VALUES ('test14', '123456', '');
INSERT INTO `accountinfo` VALUES ('test15', '123456', '');
INSERT INTO `accountinfo` VALUES ('test16', '123456', '');
INSERT INTO `accountinfo` VALUES ('test17', '123456', '');
INSERT INTO `accountinfo` VALUES ('test18', '123456', '');
INSERT INTO `accountinfo` VALUES ('test19', '123456', '');
INSERT INTO `accountinfo` VALUES ('test2', '123456', '');
INSERT INTO `accountinfo` VALUES ('test20', '123456', '');
INSERT INTO `accountinfo` VALUES ('test21', '123456', '');
INSERT INTO `accountinfo` VALUES ('test22', '123456', '');
INSERT INTO `accountinfo` VALUES ('test23', '123456', '');
INSERT INTO `accountinfo` VALUES ('test24', '123456', '');
INSERT INTO `accountinfo` VALUES ('test25', '123456', '');
INSERT INTO `accountinfo` VALUES ('test26', '123456', '');
INSERT INTO `accountinfo` VALUES ('test27', '123456', '');
INSERT INTO `accountinfo` VALUES ('test28', '123456', '');
INSERT INTO `accountinfo` VALUES ('test29', '123456', '');
INSERT INTO `accountinfo` VALUES ('test3', '123456', '');
INSERT INTO `accountinfo` VALUES ('test30', '123456', '');
INSERT INTO `accountinfo` VALUES ('test31', '123456', '');
INSERT INTO `accountinfo` VALUES ('test32', '123456', '');
INSERT INTO `accountinfo` VALUES ('test33', '123456', '');
INSERT INTO `accountinfo` VALUES ('test34', '123456', '');
INSERT INTO `accountinfo` VALUES ('test35', '123456', '');
INSERT INTO `accountinfo` VALUES ('test36', '123456', '');
INSERT INTO `accountinfo` VALUES ('test37', '123456', '');
INSERT INTO `accountinfo` VALUES ('test38', '123456', '');
INSERT INTO `accountinfo` VALUES ('test39', '123456', '');
INSERT INTO `accountinfo` VALUES ('test4', '123456', '');
INSERT INTO `accountinfo` VALUES ('test40', '123456', '');
INSERT INTO `accountinfo` VALUES ('test41', '123456', '');
INSERT INTO `accountinfo` VALUES ('test42', '123456', '');
INSERT INTO `accountinfo` VALUES ('test43', '123456', '');
INSERT INTO `accountinfo` VALUES ('test44', '123456', '');
INSERT INTO `accountinfo` VALUES ('test45', '123456', '');
INSERT INTO `accountinfo` VALUES ('test46', '123456', '');
INSERT INTO `accountinfo` VALUES ('test47', '123456', '');
INSERT INTO `accountinfo` VALUES ('test48', '123456', '');
INSERT INTO `accountinfo` VALUES ('test49', '123456', '');
INSERT INTO `accountinfo` VALUES ('test5', '123456', '');
INSERT INTO `accountinfo` VALUES ('test50', '123456', '');
INSERT INTO `accountinfo` VALUES ('test51', '123456', '');
INSERT INTO `accountinfo` VALUES ('test52', '123456', '');
INSERT INTO `accountinfo` VALUES ('test53', '123456', '');
INSERT INTO `accountinfo` VALUES ('test54', '123456', '');
INSERT INTO `accountinfo` VALUES ('test55', '123456', '');
INSERT INTO `accountinfo` VALUES ('test56', '123456', '');
INSERT INTO `accountinfo` VALUES ('test57', '123456', '');
INSERT INTO `accountinfo` VALUES ('test58', '123456', '');
INSERT INTO `accountinfo` VALUES ('test59', '123456', '');
INSERT INTO `accountinfo` VALUES ('test6', '123456', '');
INSERT INTO `accountinfo` VALUES ('test60', '123456', '');
INSERT INTO `accountinfo` VALUES ('test61', '123456', '');
INSERT INTO `accountinfo` VALUES ('test62', '123456', '');
INSERT INTO `accountinfo` VALUES ('test63', '123456', '');
INSERT INTO `accountinfo` VALUES ('test64', '123456', '');
INSERT INTO `accountinfo` VALUES ('test65', '123456', '');
INSERT INTO `accountinfo` VALUES ('test66', '123456', '');
INSERT INTO `accountinfo` VALUES ('test67', '123456', '');
INSERT INTO `accountinfo` VALUES ('test68', '123456', '');
INSERT INTO `accountinfo` VALUES ('test69', '123456', '');
INSERT INTO `accountinfo` VALUES ('test7', '123456', '');
INSERT INTO `accountinfo` VALUES ('test70', '123456', '');
INSERT INTO `accountinfo` VALUES ('test71', '123456', '');
INSERT INTO `accountinfo` VALUES ('test72', '123456', '');
INSERT INTO `accountinfo` VALUES ('test73', '123456', '');
INSERT INTO `accountinfo` VALUES ('test74', '123456', '');
INSERT INTO `accountinfo` VALUES ('test75', '123456', '');
INSERT INTO `accountinfo` VALUES ('test76', '123456', '');
INSERT INTO `accountinfo` VALUES ('test77', '123456', '');
INSERT INTO `accountinfo` VALUES ('test78', '123456', '');
INSERT INTO `accountinfo` VALUES ('test79', '123456', '');
INSERT INTO `accountinfo` VALUES ('test8', '123456', '');
INSERT INTO `accountinfo` VALUES ('test80', '123456', '');
INSERT INTO `accountinfo` VALUES ('test81', '123456', '');
INSERT INTO `accountinfo` VALUES ('test82', '123456', '');
INSERT INTO `accountinfo` VALUES ('test83', '123456', '');
INSERT INTO `accountinfo` VALUES ('test84', '123456', '');
INSERT INTO `accountinfo` VALUES ('test85', '123456', '');
INSERT INTO `accountinfo` VALUES ('test86', '123456', '');
INSERT INTO `accountinfo` VALUES ('test87', '123456', '');
INSERT INTO `accountinfo` VALUES ('test88', '123456', '');
INSERT INTO `accountinfo` VALUES ('test89', '123456', '');
INSERT INTO `accountinfo` VALUES ('test9', '123456', '');
INSERT INTO `accountinfo` VALUES ('test90', '123456', '');
INSERT INTO `accountinfo` VALUES ('test91', '123456', '');
INSERT INTO `accountinfo` VALUES ('test92', '123456', '');
INSERT INTO `accountinfo` VALUES ('test93', '123456', '');
INSERT INTO `accountinfo` VALUES ('test94', '123456', '');
INSERT INTO `accountinfo` VALUES ('test95', '123456', '');
INSERT INTO `accountinfo` VALUES ('test96', '123456', '');
INSERT INTO `accountinfo` VALUES ('test97', '123456', '');
INSERT INTO `accountinfo` VALUES ('test98', '123456', '');
INSERT INTO `accountinfo` VALUES ('test99', '123456', '');

-- ----------------------------
-- Table structure for `roleinfo`
-- ----------------------------
DROP TABLE IF EXISTS `roleinfo`;
CREATE TABLE `roleinfo` (
  `ID` varchar(128) NOT NULL,
  `Property` blob,
  `Record` blob,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;