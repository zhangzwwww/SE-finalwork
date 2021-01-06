CREATE table if not exists `patient` (
		`id`     VARCHAR(255),
		`name`   VARCHAR(255),
		`gender` VARCHAR(255),
		`birth`  DATE,
		`age`    INTEGER,
		PRIMARY KEY (`id`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE table if not exists `mark` (
	`id` VARCHAR(255),
	`imageId` VARCHAR(255),
	`view` INTEGER,
	`topX` INTEGER,
	`topY` INTEGER,
	`layer` INTEGER,
	`bottomX` INTEGER,
	`bottomY` INTEGER,
	PRIMARY KEY (`id`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
