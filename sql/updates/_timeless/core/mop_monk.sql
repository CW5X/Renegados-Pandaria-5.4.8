-- Crackling Jade Lightning
DELETE FROM `spell_script_names` WHERE `spell_id` IN (117952, 117959);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(117952, "spell_monk_crackling_jade_lightning"),
(117959, "spell_monk_crackling_jade_lightning");

DELETE FROM `spell_proc_event` WHERE `entry` = 117959;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(117959, 0, 0, 0, 0, 0, 0, 0x00000028, 0, 0, 0, 0);

-- Provoke
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 115546;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(115546, 116189, 0, "Provoke");
