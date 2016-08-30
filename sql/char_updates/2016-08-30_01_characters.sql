--
-- Update event_tables columns
--
ALTER TABLE `characters` ADD COLUMN `firsttalenttree` INT AFTER `talentpoints`;

--
-- Update character_db_version
--
UPDATE `character_db_version` SET `LastUpdate` = '2016-08-30_01_characters' WHERE `LastUpdate` = '2015-11-14_01_characters';
