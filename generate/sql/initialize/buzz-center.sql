/*
MySQL Data Transfer
Source Host: 192.168.0.208
Source Database: buzz-center
Target Host: 192.168.0.208
Target Database: buzz-center
Date: 2013-7-3 ÉÏÎç 09:33:07
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for actor-0
-- ----------------------------
DROP TABLE IF EXISTS `actor-0`;
CREATE TABLE `actor-0` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `open_id` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `zone_id` smallint(6) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `online_status` tinyint(4) NOT NULL DEFAULT '0',
  `session_status` tinyint(4) NOT NULL DEFAULT '0',
  `session` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `session_time` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `open_id+zone_id` (`open_id`,`zone_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for actor-1
-- ----------------------------
DROP TABLE IF EXISTS `actor-1`;
CREATE TABLE `actor-1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `open_id` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `zone_id` smallint(6) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `online_status` tinyint(4) NOT NULL DEFAULT '0',
  `session_status` tinyint(4) NOT NULL DEFAULT '0',
  `session` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `session_time` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `open_id+zone_id` (`open_id`,`zone_id`)
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for actor-2
-- ----------------------------
DROP TABLE IF EXISTS `actor-2`;
CREATE TABLE `actor-2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `open_id` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `zone_id` smallint(6) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `online_status` tinyint(4) NOT NULL DEFAULT '0',
  `session_status` tinyint(4) NOT NULL DEFAULT '0',
  `session` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `session_time` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `open_id+zone_id` (`open_id`,`zone_id`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for actor-3
-- ----------------------------
DROP TABLE IF EXISTS `actor-3`;
CREATE TABLE `actor-3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `open_id` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `zone_id` smallint(6) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `online_status` tinyint(4) NOT NULL DEFAULT '0',
  `session_status` tinyint(4) NOT NULL DEFAULT '0',
  `session` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `session_time` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `open_id+zone_id` (`open_id`,`zone_id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for actor-4
-- ----------------------------
DROP TABLE IF EXISTS `actor-4`;
CREATE TABLE `actor-4` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `open_id` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `zone_id` smallint(6) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `online_status` tinyint(4) NOT NULL DEFAULT '0',
  `session_status` tinyint(4) NOT NULL DEFAULT '0',
  `session` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `session_time` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `open_id+zone_id` (`open_id`,`zone_id`)
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Procedure structure for CENTER_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_ACTOR_LOGIN`(IN `open_id` varchar(255), IN `zone_id` smallint, IN `open_id_hash` int, IN `session_key` varchar(255), IN `session_status` tinyint)
BEGIN

  DECLARE max_table_number int DEFAULT 5;

  SET @param_open_id = open_id;
  SET @param_zone_id = zone_id;
  SET @param_open_id_hash = open_id_hash;
  SET @param_session_key = session_key;
  SET @param_session_status = session_status;

  SET @ret_id = 0;
  SET @ret_name = NULL;
  SET @ret_code = 0;
  SET @ret_table_number = 0;

  SET @count = 0;

  SET @table_number = @param_open_id_hash % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);

  START TRANSACTION;

  SET @sql = CONCAT('select count(1), `id`, `name` into @count, @ret_id, @ret_name from `', @table_name, '` where `open_id` = ? and `zone_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_open_id, @param_zone_id;

  IF @count != 0 THEN
	SET @sql = CONCAT('update `', @table_name, '` set `session` = ?, `session_time` = current_timestamp(), `session_status` = ? where `open_id` = ? and `zone_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_session_key, @param_session_status, @param_open_id, @param_zone_id;
    SET @ret_code = 0;
    SET @ret_table_number = @table_number;
  ELSE
    SET @ret_code = -1;
  END IF;

  COMMIT;

  SELECT @ret_code, @ret_table_number, @ret_id, @ret_name;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CENTER_ACTOR_LOGOUT
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_ACTOR_LOGOUT`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_ACTOR_LOGOUT`(IN `table_id` smallint, IN `id` int)
BEGIN

  DECLARE max_table_number int DEFAULT 5;

  SET @param_table_id = table_id;
  SET @param_id = id;


  IF @param_table_id < max_table_number THEN

    SET @table_name = CONCAT('actor-', @param_table_id);

    START TRANSACTION;

    SET @sql = CONCAT('update `', @table_name, '` set `online_status` = 0 where `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id;

    COMMIT;

  END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CENTER_ACTOR_ONLINE
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_ACTOR_ONLINE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_ACTOR_ONLINE`(IN `table_id` smallint, IN `id` int)
BEGIN

  DECLARE max_table_number int DEFAULT 5;

  SET @param_table_id = table_id;
  SET @param_id = id;

  SET @ret_online_status = 0;

  SET @count = 0;

  IF @param_table_id >= max_table_number THEN
    SET @ret_online_status = -1;
  ELSE
    SET @table_name = CONCAT('actor-', @param_table_id);
    SET @sql = CONCAT('select count(1), `online_status` into @count, @ret_online_status from `', @table_name, '` where `id` = ? limit 1');
    PREPARE stmt FROM @sql;

    START TRANSACTION;

    EXECUTE stmt USING @param_id;

    IF @count = 1 THEN
      IF @ret_online_status = 0 THEN
        SET @sql = CONCAT('update `', @table_name, '` set `online_status` = 1 where `id` = ?');
        PREPARE stmt FROM @sql;
        EXECUTE stmt USING @param_id;
      END IF;
    ELSE
      SET @ret_online_status = -1;
    END IF;

    COMMIT;

  END IF;

  SELECT @ret_online_status;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CENTER_CHECK_SESSION
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_CHECK_SESSION`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_CHECK_SESSION`(IN `table_id` smallint, IN `id` int, IN `session_key` varchar(255), IN `session_status` tinyint, IN `session_effective_time` bigint)
BEGIN

  DECLARE max_table_number int DEFAULT 5;

  SET @param_table_id = table_id;
  SET @param_id = id;
  SET @param_session_key = session_key;
  SET @param_session_status = session_status;
  SET @param_session_effective_time = session_effective_time;

  SET @ret_code = 0;

  SET @time_now = unix_timestamp(now());
  SET @session_time = 0;
  SET @session_status = 0;
  SET @online_status = 0;
  SET @count = 0;

  IF @param_table_id >= max_table_number THEN
    SET @ret_code = -1;
  ELSE
    SET @table_name = CONCAT('actor-', @param_table_id);
    SET @sql = CONCAT('select count(1), `online_status`, `session_status`, `session_time` into @count, @online_status, @session_status, @session_time from `', @table_name, '` where `id` = ? and `session` = ? limit 1');
    PREPARE stmt FROM @sql;

    START TRANSACTION;

    EXECUTE stmt USING @param_id, @param_session_key;

    SET @unix_session_time = unix_timestamp(@session_time) + @param_session_effective_time;

    IF @count = 1 and @session_status = @param_session_status and @unix_session_time > @time_now THEN

      SET @ret_code = 0;
      SET @session_status = @session_status + 1;

      SET @update_online_status = @online_status;
      IF @update_online_status = 0 THEN
        SET @update_online_status = 1;
      END IF;

      SET @sql = CONCAT('update `', @table_name, '` set `online_status` = ?, `session_status` = ? where `id` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @update_online_status, @session_status, @param_id;
    ELSE
      SET @ret_code = -1;
    END IF;

    COMMIT;

  END IF;

  SELECT @ret_code, @online_status;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CENTER_CREATE_ACTOR
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_CREATE_ACTOR`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_CREATE_ACTOR`(IN `open_id` varchar(255), IN `zone_id` smallint, IN `open_id_hash` int, IN `name` varchar(255), IN `session_key` varchar(255))
BEGIN

  DECLARE max_table_number int DEFAULT 5;

  SET @param_open_id = open_id;
  SET @param_zone_id = zone_id;
  SET @param_open_id_hash = open_id_hash;
  SET @param_name = name;
  SET @param_session_key = session_key;

  SET @ret_code = 0;
  SET @ret_table_id = 0;
  SET @ret_id = 0;

  SET @count = 0;
  SET @i = 0;

  START TRANSACTION;

  WHILE @i < max_table_number and @count = 0 DO
    SET @table_name = CONCAT('actor-', @i);
    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `name` = ? limit 1');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_name;
    SET @i = @i + 1;
  END WHILE;

  SET @table_number = @param_open_id_hash % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);


  IF @count = 0 THEN
    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `open_id`=? and `zone_id` = ? limit 1');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_open_id, @param_zone_id;

    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`open_id`, `zone_id`, `name`, `session`, `session_time`) values(?, ?, ?, ?, current_timestamp())');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_open_id, @param_zone_id, @param_name, @param_session_key;
      SET @ret_code = 0;

      SET @sql = CONCAT('select count(1), `id` into @count, @ret_id from `', @table_name, '` where `open_id` = ? and `zone_id` = ? limit 1');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_open_id, @param_zone_id;

      IF @count = 1 THEN
        SET @ret_code = 0;
        SET @ret_table_id = @table_number;
      ELSE
        /* unknown error */
        SET @ret_code = -3;
      END IF;

    ELSE
      /* open id has created actor in this zone */
      SET @ret_code = -2;
    END IF;
  ELSE
    /* name has created actor */
    SET @ret_code = -1;
  END IF;

  COMMIT;

  SELECT @ret_code, @ret_table_id, @ret_id;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CENTER_CREATE_ACTORS
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_CREATE_ACTORS`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_CREATE_ACTORS`(IN `open_id` varchar(255), IN `zone_id` smallint, IN `open_id_hash` int, IN `name` varchar(255), IN `session_key` varchar(255))
BEGIN
  
  DECLARE max_table_number int DEFAULT 5;

  SET @param_open_id = open_id;
  SET @param_zone_id = zone_id;
  SET @param_open_id_hash = open_id_hash;
  SET @param_name = name;
  SET @param_session_key = session_key;
  
  SET @ret_code = 0;
  SET @count = 0;
  SET @i = 0;

  START TRANSACTION;

  WHILE @i < max_table_number and @count = 0 DO
    SET @table_name = CONCAT('actor-', @i);
    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `name` = ? limit 1');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_name;
    SET @i = @i + 1;
  END WHILE;

  SET @table_number = @param_open_id_hash % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);

  IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`open_id`, `zone_id`, `name`, `session`, `session_time`) values(?, ?, ?, ?, current_timestamp())');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_open_id, @param_zone_id, @param_name, @param_session_key;
      SET @ret_code = 0;
    ELSE
      SET @ret_code = -1;
  END IF;

  COMMIT;

  SELECT @ret_code;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CENTER_GET_ACTORS
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_GET_ACTORS`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_GET_ACTORS`(IN `open_id` varchar(255), IN `zone_id` smallint, IN `open_id_hash` int, IN `session_key` varchar(255))
BEGIN

  DECLARE max_table_number int DEFAULT 5;

  SET @param_open_id = open_id;
  SET @param_zone_id = zone_id;
  SET @param_open_id_hash = open_id_hash;
  SET @param_session_key = session_key;

  SET @ret_id = 0;
  SET @ret_name = NULL;

  SET @count = 0;

  SET @table_number = @param_open_id_hash % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `open_id` = ? and `zone_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_open_id, @param_zone_id;

  IF @count != 0
    THEN
	  SET @sql = CONCAT('update `', @table_name, '` set `session` = ?, `session_time` = current_timestamp() where `open_id` = ? and `zone_id` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_session_key, @param_open_id, @param_zone_id;
  END IF;

  SET @sql = CONCAT('select ? as `table_number`, `id`, `name` from `', @table_name, '` where `open_id` = ? and `zone_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @table_number, @param_open_id, @param_zone_id;

  COMMIT;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CENTER_CHECK_ACTOR_EXIST
-- ----------------------------
DROP PROCEDURE IF EXISTS `CENTER_CHECK_ACTOR_EXIST`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `CENTER_CHECK_ACTOR_EXIST`(IN `open_id` varchar(255), IN `zone_id` smallint, IN `open_id_hash` int)
BEGIN

  DECLARE max_table_number int DEFAULT 5;

  SET @param_open_id = open_id;
  SET @param_zone_id = zone_id;
  SET @param_open_id_hash = open_id_hash;

  SET @count = 0;
  SET @ret_code = 0;

  SET @table_number = @param_open_id_hash % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `open_id` = ? and `zone_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_open_id, @param_zone_id;

  IF @count != 0
    THEN
    SET @ret_code = 1;
  END IF;

  SELECT @ret_code;

END;;
DELIMITER ;

