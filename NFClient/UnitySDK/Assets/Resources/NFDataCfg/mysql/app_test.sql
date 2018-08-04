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
-- Table structure for `AccountInfo`
-- ----------------------------
DROP TABLE IF EXISTS `AccountInfo`;
CREATE TABLE `AccountInfo` (
  `ID` varchar(128) NOT NULL,
  `Password` varchar(128) NOT NULL DEFAULT '',
  `RoleID` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of AccountInfo
-- ----------------------------
INSERT INTO `AccountInfo` VALUES ('test1', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test10', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test11', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test12', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test13', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test14', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test15', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test16', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test17', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test18', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test19', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test2', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test20', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test21', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test22', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test23', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test24', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test25', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test26', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test27', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test28', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test29', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test3', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test30', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test31', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test32', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test33', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test34', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test35', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test36', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test37', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test38', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test39', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test4', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test40', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test41', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test42', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test43', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test44', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test45', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test46', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test47', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test48', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test49', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test5', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test50', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test51', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test52', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test53', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test54', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test55', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test56', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test57', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test58', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test59', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test6', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test60', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test61', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test62', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test63', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test64', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test65', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test66', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test67', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test68', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test69', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test7', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test70', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test71', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test72', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test73', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test74', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test75', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test76', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test77', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test78', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test79', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test8', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test80', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test81', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test82', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test83', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test84', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test85', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test86', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test87', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test88', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test89', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test9', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test90', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test91', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test92', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test93', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test94', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test95', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test96', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test97', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test98', '123456', '');
INSERT INTO `AccountInfo` VALUES ('test99', '123456', '');

-- ----------------------------
-- Table structure for `RoleInfo`
-- ----------------------------
