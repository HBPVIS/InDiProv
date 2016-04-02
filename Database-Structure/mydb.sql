-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Apr 02, 2016 at 06:52 AM
-- Server version: 5.6.28-0ubuntu0.14.04.1
-- PHP Version: 5.5.9-1ubuntu4.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `mydb`
--
CREATE DATABASE IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `mydb`;

-- --------------------------------------------------------

--
-- Table structure for table `Activity`
--

DROP TABLE IF EXISTS `Activity`;
CREATE TABLE IF NOT EXISTS `Activity` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `OPMGraphId` int(11) NOT NULL,
  `CauseId` int(11) NOT NULL,
  `EffectId` int(11) NOT NULL,
  `EdgeType` int(11) NOT NULL,
  `ActivityTime` datetime(4) NOT NULL,
  `CauseName` varchar(45) NOT NULL DEFAULT 'CauseName',
  `EffectName` varchar(45) NOT NULL DEFAULT 'EffectName',
  `Annotation` tinytext,
  PRIMARY KEY (`ID`,`OPMGraphId`,`CauseId`,`EffectId`,`EdgeType`,`ActivityTime`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=741 ;

-- --------------------------------------------------------

--
-- Table structure for table `Agent`
--

DROP TABLE IF EXISTS `Agent`;
CREATE TABLE IF NOT EXISTS `Agent` (
  `OPMGraphId` int(11) NOT NULL,
  `AgentId` int(11) NOT NULL,
  `Value` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`AgentId`,`OPMGraphId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `Artifact`
--

DROP TABLE IF EXISTS `Artifact`;
CREATE TABLE IF NOT EXISTS `Artifact` (
  `OPMGraphId` int(11) NOT NULL,
  `ArtifactId` int(11) NOT NULL,
  `Value` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ArtifactId`,`OPMGraphId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `Edges`
--

DROP TABLE IF EXISTS `Edges`;
CREATE TABLE IF NOT EXISTS `Edges` (
  `OPMGraphId` int(11) NOT NULL,
  `CauseId` int(11) NOT NULL,
  `EffectId` int(11) NOT NULL,
  `Role` varchar(45) NOT NULL,
  `EdgeType` int(11) NOT NULL,
  `CauseName` varchar(45) NOT NULL DEFAULT 'CauseName',
  `EffectName` varchar(45) NOT NULL DEFAULT 'EffectName',
  PRIMARY KEY (`OPMGraphId`,`CauseId`,`EffectId`,`EdgeType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `fractest`
--

DROP TABLE IF EXISTS `fractest`;
CREATE TABLE IF NOT EXISTS `fractest` (
  `c1` time(2) DEFAULT NULL,
  `c2` datetime(2) DEFAULT NULL,
  `c3` timestamp(2) NOT NULL DEFAULT CURRENT_TIMESTAMP(2) ON UPDATE CURRENT_TIMESTAMP(2)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `Process`
--

DROP TABLE IF EXISTS `Process`;
CREATE TABLE IF NOT EXISTS `Process` (
  `OPMGraphId` int(11) NOT NULL,
  `ProcessId` int(11) NOT NULL,
  `Value` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ProcessId`,`OPMGraphId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `Used`
--

DROP TABLE IF EXISTS `Used`;
CREATE TABLE IF NOT EXISTS `Used` (
  `OPMGraphId` int(11) NOT NULL,
  `ProcessId` int(11) NOT NULL,
  `Role` varchar(45) NOT NULL,
  `ArtifacId` int(11) NOT NULL,
  `OTimeLower` datetime(3) NOT NULL,
  `OTimeUpper` datetime DEFAULT NULL,
  PRIMARY KEY (`OPMGraphId`,`ProcessId`,`Role`,`ArtifacId`,`OTimeLower`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `WasControlledBy`
--

DROP TABLE IF EXISTS `WasControlledBy`;
CREATE TABLE IF NOT EXISTS `WasControlledBy` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `OPMGraphId` int(11) NOT NULL,
  `ProcessId` int(11) NOT NULL,
  `Role` varchar(45) NOT NULL,
  `AgentId` int(11) NOT NULL,
  `OTimeStartLower` datetime(3) NOT NULL,
  `OTimeStartUpper` datetime DEFAULT NULL,
  `OTimeEndLower` datetime DEFAULT NULL,
  `OTimeEndUpper` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`,`OPMGraphId`,`ProcessId`,`Role`,`AgentId`,`OTimeStartLower`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=371 ;

-- --------------------------------------------------------

--
-- Table structure for table `WasDrivedFrom`
--

DROP TABLE IF EXISTS `WasDrivedFrom`;
CREATE TABLE IF NOT EXISTS `WasDrivedFrom` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `OPMGraphId` int(11) NOT NULL,
  `EffectArtifactId` int(11) NOT NULL,
  `CauseArtifactId` int(11) NOT NULL,
  `OTimeLower` datetime DEFAULT NULL,
  `OTimeUpper` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`,`OPMGraphId`,`EffectArtifactId`,`CauseArtifactId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `WasGeneratedBy`
--

DROP TABLE IF EXISTS `WasGeneratedBy`;
CREATE TABLE IF NOT EXISTS `WasGeneratedBy` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `OPMGraphId` int(11) NOT NULL,
  `ArtifacId` int(11) NOT NULL,
  `Role` varchar(45) NOT NULL,
  `ProcessId` int(11) NOT NULL,
  `OTimeLower` datetime(3) NOT NULL,
  `OTimeUpper` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`,`OPMGraphId`,`ArtifacId`,`Role`,`ProcessId`,`OTimeLower`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=371 ;

-- --------------------------------------------------------

--
-- Table structure for table `WasTriggeredBy`
--

DROP TABLE IF EXISTS `WasTriggeredBy`;
CREATE TABLE IF NOT EXISTS `WasTriggeredBy` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `OPMGraphId` int(11) NOT NULL,
  `EffectProcessId` int(11) NOT NULL,
  `CauseProcessId` int(11) NOT NULL,
  `Account` int(11) NOT NULL,
  `OTimeLower` datetime NOT NULL,
  `OTimeUpper` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`,`OPMGraphId`,`EffectProcessId`,`CauseProcessId`,`Account`,`OTimeLower`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `Workflow`
--

DROP TABLE IF EXISTS `Workflow`;
CREATE TABLE IF NOT EXISTS `Workflow` (
  `GraphId` int(11) NOT NULL AUTO_INCREMENT,
  `GraphName` varchar(45) NOT NULL,
  `GraphVersion` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`GraphId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
