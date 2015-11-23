
DROP TABLE IF EXISTS `playercreateinfo`;
CREATE TABLE IF NOT EXISTS `playercreateinfo` (
  `Index` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `factiontemplate` int(10) unsigned NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mapID` int(10) unsigned NOT NULL DEFAULT '0',
  `zoneID` int(10) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  `displayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseStrength` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseAgility` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseStamina` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseIntellect` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseSpirit` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseHealth` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseMana` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseRage` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseFocus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `BaseEnergy` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attackpower` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mindmg` float NOT NULL DEFAULT '0',
  `maxdmg` float NOT NULL DEFAULT '0',
  `introid` tinyint(3) unsigned DEFAULT '0',
  `taximask` tinytext,
  PRIMARY KEY (`Index`)
) ENGINE=MyISAM AUTO_INCREMENT=95 DEFAULT CHARSET=latin1 COMMENT='Player System';

REPLACE INTO `playercreateinfo` (`Index`, `race`, `factiontemplate`, `class`, `mapID`, `zoneID`, `positionX`, `positionY`, `positionZ`, `displayID`, `BaseStrength`, `BaseAgility`, `BaseStamina`, `BaseIntellect`, `BaseSpirit`, `BaseHealth`, `BaseMana`, `BaseRage`, `BaseFocus`, `BaseEnergy`, `attackpower`, `mindmg`, `maxdmg`, `introid`, `taximask`) VALUES
	(1, 1, 1, 1, 0, 12, -8946.42, -131.35, 83.63, 49, 23, 20, 22, 20, 20, 60, 0, 1000, 0, 0, 29, 5, 6, 81, '2'),
	(2, 2, 2, 1, 1, 14, -618.51, -4251.66, 38.71, 51, 26, 17, 23, 17, 22, 70, 0, 1000, 0, 0, 35, 6, 7, 21, '4194304'),
	(3, 3, 3, 1, 0, 1, -6240.31, 331.03, 382.75, 53, 28, 16, 23, 19, 19, 70, 0, 1000, 0, 0, 39, 6, 7, 41, '32'),
	(4, 4, 4, 1, 1, 141, 10311.9, 832.25, 1326.39, 55, 19, 24, 22, 20, 20, 60, 0, 1000, 0, 0, 21, 4, 4, 61, '100663296'),
	(5, 5, 5, 1, 0, 85, 1704.22, 1703.54, 133.74, 57, 22, 18, 22, 18, 25, 60, 0, 1000, 0, 0, 27, 4, 5, 2, '1024'),
	(6, 6, 6, 1, 1, 215, -2915.55, -257.34, 59.269, 59, 28, 16, 23, 16, 22, 71, 0, 1000, 0, 0, 39, 6, 7, 141, '2097152'),
	(7, 7, 115, 1, 0, 1, -6240.31, 331.03, 382.75, 1563, 18, 22, 22, 24, 20, 60, 0, 1000, 0, 0, 19, 3, 4, 101, '32'),
	(8, 8, 116, 1, 1, 14, -1172.53, -5263.92, 0.85, 1478, 24, 22, 22, 16, 21, 60, 0, 1000, 0, 0, 31, 5, 6, 121, '4194304'),
	(9, 1, 1, 2, 0, 12, -8946.42, -131.35, 83.63, 49, 22, 20, 22, 20, 21, 58, 80, 0, 0, 0, 27, 4, 5, 81, '2'),
	(10, 3, 3, 2, 0, 1, -6240.31, 331.03, 382.75, 53, 27, 16, 23, 19, 20, 68, 79, 0, 0, 0, 37, 6, 7, 41, '32'),
	(11, 2, 2, 3, 1, 14, -618.51, -4251.66, 38.71, 51, 23, 20, 22, 27, 23, 66, 0, 0, 100, 0, 25, 4, 5, 21, '4194304'),
	(12, 3, 3, 3, 0, 1, -6240.31, 331.03, 382.75, 53, 25, 19, 22, 19, 20, 66, 0, 0, 100, 0, 26, 4, 5, 41, '32'),
	(15, 4, 4, 3, 1, 141, 10311.9, 832.25, 1326.39, 55, 16, 27, 21, 20, 21, 56, 0, 0, 100, 0, 25, 4, 5, 61, '100663296'),
	(16, 6, 6, 3, 1, 215, -2915.55, -257.34, 59.269, 59, 25, 19, 22, 16, 23, 67, 0, 0, 100, 0, 26, 4, 5, 141, '2097152'),
	(17, 8, 116, 3, 1, 14, -1172.53, -5263.92, 0.85, 1478, 21, 25, 21, 16, 22, 56, 0, 0, 100, 0, 28, 5, 5, 121, '4194304'),
	(18, 1, 1, 4, 0, 12, -8946.42, -131.35, 83.63, 49, 21, 23, 21, 20, 20, 55, 0, 0, 0, 100, 26, 4, 5, 81, '2'),
	(19, 2, 2, 4, 1, 14, -618.51, -4251.66, 38.71, 51, 24, 20, 22, 17, 22, 65, 0, 0, 0, 100, 26, 4, 5, 21, '4194304'),
	(20, 3, 3, 4, 0, 1, -6240.31, 331.03, 382.75, 53, 26, 19, 22, 19, 19, 65, 0, 0, 0, 100, 27, 4, 5, 41, '32'),
	(21, 4, 4, 4, 1, 141, 10311.9, 832.25, 1326.39, 55, 17, 27, 21, 20, 20, 55, 0, 0, 0, 100, 26, 4, 5, 61, '100663296'),
	(22, 5, 5, 4, 0, 85, 1704.22, 1703.54, 133.74, 57, 20, 21, 21, 18, 25, 55, 0, 0, 0, 100, 23, 4, 5, 2, '1024'),
	(23, 7, 115, 4, 0, 1, -6240.31, 331.03, 382.75, 1563, 16, 25, 21, 24, 20, 55, 0, 0, 0, 100, 23, 4, 5, 101, '32'),
	(24, 8, 116, 4, 1, 14, -1172.53, -5263.92, 0.85, 1478, 22, 25, 21, 16, 21, 55, 0, 0, 0, 100, 29, 5, 6, 121, '4194304'),
	(25, 1, 1, 5, 0, 12, -8946.42, -131.35, 83.63, 49, 20, 20, 20, 22, 23, 52, 160, 0, 0, 0, 20, 3, 4, 81, '2'),
	(26, 3, 3, 5, 0, 1, -6240.31, 331.03, 382.75, 53, 25, 16, 21, 21, 22, 62, 145, 0, 0, 0, 30, 5, 6, 41, '32'),
	(27, 4, 4, 5, 1, 141, 10311.9, 832.25, 1326.39, 55, 16, 24, 20, 22, 23, 52, 160, 0, 0, 0, 12, 2, 3, 61, '100663296'),
	(28, 5, 5, 5, 0, 85, 1704.22, 1703.54, 133.74, 57, 19, 18, 20, 20, 28, 52, 130, 0, 0, 0, 18, 3, 4, 2, '1024'),
	(29, 8, 116, 5, 1, 14, -1172.53, -5263.92, 0.85, 1478, 21, 22, 20, 18, 24, 52, 128, 0, 0, 0, 22, 4, 5, 121, '4194304'),
	(30, 2, 2, 7, 1, 14, -618.51, -4251.66, 38.71, 51, 24, 17, 22, 18, 24, 72, 73, 0, 0, 0, 23, 4, 5, 21, '4194304'),
	(31, 6, 6, 7, 1, 215, -2915.55, -257.34, 59.269, 59, 26, 16, 22, 17, 24, 69, 72, 0, 0, 0, 24, 4, 5, 141, '2097152'),
	(32, 8, 116, 7, 1, 14, -1172.53, -5263.92, 0.85, 1478, 22, 22, 21, 17, 23, 69, 72, 0, 0, 0, 26, 4, 5, 121, '4194304'),
	(34, 1, 1, 8, 0, 12, -8946.42, -131.35, 83.63, 49, 20, 20, 20, 23, 22, 52, 165, 0, 0, 0, 20, 3, 4, 81, '2'),
	(35, 5, 5, 8, 0, 85, 1704.22, 1703.54, 133.74, 57, 19, 18, 20, 21, 27, 52, 135, 0, 0, 0, 18, 3, 4, 2, '1024'),
	(36, 7, 115, 8, 0, 1, -6240.31, 331.03, 382.75, 1563, 15, 22, 20, 27, 22, 52, 225, 0, 0, 0, 11, 2, 3, 101, '32'),
	(37, 8, 116, 8, 1, 14, -1172.53, -5263.92, 0.85, 1478, 21, 22, 20, 19, 23, 52, 119, 0, 0, 0, 22, 4, 5, 121, '4194304'),
	(38, 1, 1, 9, 0, 12, -8946.42, -131.35, 83.63, 49, 20, 20, 21, 22, 22, 53, 140, 0, 0, 0, 20, 3, 4, 81, '2'),
	(39, 2, 2, 9, 1, 14, -618.51, -4251.66, 38.71, 51, 23, 17, 22, 19, 24, 63, 108, 0, 0, 0, 26, 4, 5, 21, '4194304'),
	(40, 5, 5, 9, 0, 85, 1704.22, 1703.54, 133.74, 57, 19, 18, 21, 20, 27, 53, 110, 0, 0, 0, 18, 3, 4, 2, '1024'),
	(41, 7, 115, 9, 0, 1, -6240.31, 331.03, 382.75, 1563, 15, 22, 21, 26, 22, 53, 200, 0, 0, 0, 10, 2, 3, 101, '32'),
	(42, 4, 4, 11, 1, 141, 10311.9, 832.25, 1326.39, 55, 17, 24, 20, 22, 22, 54, 100, 0, 0, 0, 14, 4, 3, 61, '100663296'),
	(43, 6, 6, 11, 1, 215, -2915.55, -257.34, 59.269, 59, 26, 16, 21, 18, 24, 65, 68, 0, 0, 0, 32, 5, 6, 141, '2097152'),
	(44, 11, 1629, 1, 530, 3526, -3961.63, -13931.2, 100.61, 16125, 24, 17, 22, 20, 22, 60, 0, 1000, 0, 0, 31, 5, 6, 163, '0 0 536870912'),
	(45, 11, 1629, 2, 530, 3526, -3961.63, -13931.2, 100.61, 16125, 23, 17, 22, 20, 23, 58, 80, 0, 0, 0, 29, 5, 6, 163, '0 0 536870912'),
	(46, 11, 1629, 3, 530, 3526, -3961.63, -13931.2, 100.61, 16125, 21, 20, 21, 20, 23, 56, 0, 0, 100, 0, 23, 4, 5, 163, '0 0 536870912'),
	(47, 11, 1629, 5, 530, 3526, -3961.63, -13931.2, 100.61, 16125, 21, 17, 20, 22, 25, 52, 160, 0, 0, 0, 22, 4, 5, 163, '0 0 536870912'),
	(48, 11, 1629, 7, 530, 3526, -3961.63, -13931.2, 100.61, 16125, 22, 17, 21, 21, 24, 59, 90, 0, 0, 0, 21, 4, 4, 163, '0 0 536870912'),
	(49, 11, 1629, 8, 530, 3526, -3961.63, -13931.2, 100.61, 16125, 21, 17, 20, 23, 24, 52, 165, 0, 0, 0, 22, 4, 5, 163, '0 0 536870912'),
	(50, 10, 1610, 2, 530, 3431, 10354.7, -6366.47, 35.51, 15476, 19, 22, 22, 23, 19, 58, 125, 0, 0, 0, 21, 4, 4, 162, '0 0 131072'),
	(51, 10, 1610, 3, 530, 3431, 10354.7, -6366.47, 35.51, 15476, 17, 25, 21, 23, 19, 56, 0, 0, 100, 0, 24, 4, 5, 162, '0 0 131072'),
	(52, 10, 1610, 4, 530, 3431, 10354.7, -6366.47, 35.51, 15476, 18, 25, 21, 16, 19, 55, 0, 0, 0, 100, 25, 4, 5, 162, '0 0 131072'),
	(53, 10, 1610, 5, 530, 3431, 10354.7, -6366.47, 35.51, 15476, 17, 22, 20, 25, 21, 52, 205, 0, 0, 0, 14, 3, 3, 162, '0 0 131072'),
	(54, 10, 1610, 8, 530, 3431, 10354.7, -6366.47, 35.51, 15476, 17, 22, 20, 26, 20, 52, 210, 0, 0, 0, 14, 3, 3, 162, '0 0 131072'),
	(55, 10, 1610, 9, 530, 3431, 10354.7, -6366.47, 35.51, 15476, 17, 22, 21, 25, 20, 53, 185, 0, 0, 0, 14, 3, 3, 162, '0 0 131072'),
	(56, 1, 1, 6, 609, 4298, 2355.23, -5661.32, 426.028, 49, 108, 73, 99, 29, 43, 2169, 0, 0, 0, 0, 29, 6, 7, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(57, 2, 2, 6, 609, 4298, 2355.23, -5661.32, 426.028, 51, 105, 75, 100, 32, 40, 2179, 0, 0, 0, 0, 29, 7, 8, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(58, 3, 3, 6, 609, 4298, 2355.23, -5661.32, 426.028, 53, 113, 69, 100, 28, 41, 2179, 0, 0, 0, 0, 29, 8, 9, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(59, 4, 4, 6, 609, 4298, 2355.23, -5661.32, 426.028, 55, 104, 77, 99, 29, 42, 2169, 0, 0, 0, 0, 29, 6, 7, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(60, 5, 5, 6, 609, 4298, 2355.23, -5661.32, 426.028, 57, 107, 71, 99, 27, 47, 2169, 0, 0, 0, 0, 29, 6, 7, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(61, 6, 6, 6, 609, 4298, 2355.23, -5661.32, 426.028, 59, 113, 69, 100, 25, 44, 2246, 0, 0, 0, 0, 29, 8, 9, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(62, 7, 115, 6, 609, 4298, 2355.23, -5661.32, 426.028, 1563, 103, 75, 99, 33, 42, 2169, 0, 0, 0, 0, 29, 6, 7, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(63, 8, 116, 6, 609, 4298, 2355.23, -5661.32, 426.028, 1478, 109, 75, 99, 25, 43, 2169, 0, 0, 0, 0, 29, 6, 7, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(64, 11, 1629, 6, 609, 4298, 2355.23, -5661.32, 426.028, 16125, 106, 70, 99, 29, 44, 2169, 0, 0, 0, 0, 29, 7, 8, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(65, 10, 1610, 6, 609, 4298, 2355.23, -5661.32, 426.028, 15476, 105, 75, 99, 32, 40, 2169, 0, 0, 0, 0, 29, 6, 7, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(66, 9, 2204, 1, 648, 4737, -618.51, -4251.66, 38.71, 6894, 23, 20, 22, 20, 20, 60, 0, 1000, 0, 0, 21, 3, 6, 172, '0'),
	(67, 9, 2204, 3, 648, 4737, -618.51, -4251.66, 38.71, 6894, 22, 20, 22, 20, 21, 56, 0, 0, 100, 0, 23, 4, 5, 172, '0'),
	(68, 9, 2204, 4, 648, 4737, -618.51, -4251.66, 38.71, 6894, 21, 23, 21, 20, 20, 55, 0, 0, 0, 100, 23, 4, 5, 172, '0'),
	(69, 9, 2204, 5, 648, 4737, -618.51, -4251.66, 38.71, 6894, 20, 20, 20, 22, 23, 52, 160, 0, 0, 0, 14, 3, 5, 172, '0'),
	(70, 9, 2204, 6, 609, 4298, 2355.23, -5661.32, 426.08, 6894, 105, 73, 99, 25, 43, 2169, 0, 0, 0, 0, 29, 6, 7, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(71, 9, 2204, 7, 648, 4737, -618.51, -4251.66, 38.71, 6894, 24, 17, 23, 18, 25, 60, 103, 0, 0, 0, 20, 4, 5, 172, '0'),
	(72, 9, 2204, 8, 648, 4737, -618.51, -4251.66, 38.71, 6894, 20, 20, 20, 23, 22, 52, 145, 0, 0, 0, 14, 3, 5, 172, '0'),
	(73, 9, 2204, 9, 648, 4737, -618.51, -4251.66, 38.71, 6894, 20, 20, 21, 22, 22, 52, 145, 0, 0, 0, 14, 2, 4, 172, '0'),
	(74, 22, 2203, 1, 654, 4714, -1443.62, 1409.88, 35.5561, 29422, 23, 20, 22, 20, 20, 60, 0, 1000, 0, 0, 27, 4, 6, 170, '0'),
	(75, 22, 2203, 3, 654, 4714, -1443.62, 1409.88, 35.5561, 29422, 22, 20, 22, 20, 21, 56, 0, 0, 100, 0, 24, 4, 5, 170, '0'),
	(76, 22, 2203, 4, 654, 4714, -1443.62, 1409.88, 35.5561, 29422, 21, 23, 21, 20, 20, 55, 0, 0, 0, 100, 25, 4, 5, 170, '0'),
	(77, 22, 2203, 5, 654, 4714, -1443.62, 1409.88, 35.5561, 29422, 20, 20, 20, 23, 22, 52, 160, 0, 0, 0, 14, 4, 5, 170, '0'),
	(78, 22, 2203, 6, 609, 4298, 2355.23, -5661.32, 426.028, 29422, 105, 73, 99, 25, 40, 2169, 0, 0, 0, 0, 29, 7, 8, 165, '4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952'),
	(79, 22, 2203, 8, 654, 4714, -1443.62, 1409.88, 35.5561, 29422, 20, 20, 20, 23, 22, 52, 145, 0, 0, 0, 14, 3, 4, 170, '0'),
	(80, 22, 2203, 9, 654, 4714, -1443.62, 1409.88, 35.5561, 29422, 20, 20, 21, 22, 22, 52, 145, 0, 0, 0, 14, 3, 4, 170, '0'),
	(81, 22, 2203, 11, 654, 4714, -1443.62, 1409.88, 35.5561, 29422, 26, 15, 22, 17, 24, 52, 70, 0, 0, 0, 22, 5, 6, 170, '0'),
	(82, 1, 1, 3, 0, 12, -8946.42, -131.35, 83.63, 49, 17, 28, 20, 20, 21, 56, 0, 0, 100, 0, 24, 4, 5, 81, '2'),
	(83, 10, 1610, 1, 530, 3431, 10354.7, -6366.47, 35.51, 15476, 23, 20, 22, 20, 20, 58, 0, 1000, 0, 0, 29, 6, 7, 162, '0 0 131072'),
	(84, 6, 6, 2, 1, 215, -2915.55, -257.34, 59.269, 59, 19, 22, 21, 21, 20, 65, 68, 0, 0, 0, 32, 8, 9, 141, '2097152'),
	(85, 5, 5, 3, 0, 85, 1704.22, 1703.54, 133.74, 57, 17, 25, 20, 24, 20, 55, 0, 0, 100, 0, 23, 4, 5, 2, '1024'),
	(86, 6, 6, 5, 1, 215, -2915.55, -257.34, 59.269, 59, 19, 18, 21, 20, 28, 65, 68, 0, 0, 0, 32, 4, 5, 141, '2097152'),
	(87, 7, 115, 5, 0, 1, -6240.31, 331.03, 382.75, 1563, 20, 20, 20, 22, 23, 53, 200, 0, 0, 0, 10, 2, 3, 101, '32'),
	(88, 3, 3, 7, 0, 1, -6240.31, 331.03, 382.75, 53, 20, 20, 20, 22, 23, 66, 84, 0, 0, 0, 26, 6, 7, 41, '32'),
	(89, 2, 2, 8, 1, 14, -618.51, -4251.66, 38.71, 51, 17, 22, 19, 27, 21, 63, 108, 0, 0, 0, 26, 4, 5, 21, '4194304'),
	(90, 3, 3, 8, 0, 1, -6240.31, 331.03, 382.75, 53, 20, 20, 20, 23, 22, 62, 145, 0, 0, 0, 30, 4, 5, 41, '32'),
	(91, 4, 4, 8, 1, 141, 10311.9, 832.25, 1326.39, 55, 20, 20, 20, 23, 22, 52, 160, 0, 0, 0, 13, 2, 3, 61, '100663296'),
	(92, 3, 3, 9, 0, 1, -6240.31, 331.03, 382.75, 53, 19, 18, 22, 20, 27, 62, 145, 0, 0, 0, 30, 4, 5, 41, '32'),
	(93, 8, 116, 9, 1, 14, -1172.53, -5263.92, 0.85, 1478, 19, 18, 22, 20, 27, 52, 128, 0, 0, 0, 22, 4, 5, 121, '4194304'),
	(94, 8, 116, 11, 1, 14, -1172.53, -5263.92, 0.85, 1478, 26, 15, 22, 17, 24, 52, 119, 0, 0, 0, 22, 5, 6, 121, '4194304');

UPDATE `world_db_version` SET `LastUpdate` = '2015-11-20_01_playercreateinfo' WHERE `LastUpdate` = '2015-11-06_03_event_spawns';