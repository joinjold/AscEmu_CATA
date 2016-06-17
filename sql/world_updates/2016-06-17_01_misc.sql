--
-- Set proper x-y-z values for goblins
--
UPDATE `playercreateinfo` SET `positionX` = -8423.81, `positionY` = 1361.3, `positionZ` = 104.671 WHERE `Index` IN(66,67,68,69,70,71,72,73);

--
-- Add Gilneas and Kezan to recall
--
REPLACE INTO `recall`
   (`id`, `name`, `MapId`, `positionX`, `positionY`, `positionZ`, `Orientation`)
VALUES
   (801, 'Gilneas', 654, -1451.53, 1403.35, 35.5561, 0),
   (802, 'Kezan', 648, -8423.81, 1361.3, 104.671, 0);
   
--
-- Add Gilneas and Kezan to worldmap_info
--
REPLACE INTO `worldmap_info`
   (`entry`, `screenid`, `type`, `maxplayers`, `minlevel`, `minlevel_heroic`, `repopx`, `repopy`, `repopz`, `repopentry`, `area_name`, `flags`, `cooldown`, `lvl_mod_a`, `required_quest_A`, `required_quest_H`, `required_item`, `heroic_keyid_1`, `heroic_keyid_2`, `viewingDistance`, `required_checkpoint`)
VALUES
   (648, 0, 0, 0, 1, 0, 0, 0, 0, 0, 'Kezan', 1, 0, 0, 0, 0, 0, 0, 0, 500, 0),
   (654, 0, 0, 0, 1, 0, 0, 0, 0, 0, 'Gilneas', 1, 0, 0, 0, 0, 0, 0, 0, 500, 0);
   
--
-- Update world_db_version. This is the first version of our cata_db (check out our forum)
--
UPDATE `world_db_version` SET `LastUpdate`='2016-06-17_01_misc';
