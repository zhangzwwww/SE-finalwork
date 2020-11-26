CREATE table if not exists `patient` (
		`id`     VARCHAR(255),
		`name`   VARCHAR(255),
		`gender` VARCHAR(255),
		`birth`  DATE,
		`age`    INTEGER,
		PRIMARY KEY (`id`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
