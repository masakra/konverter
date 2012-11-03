
PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS contact;

CREATE TABLE contact (
	id		INTEGER PRIMARY KEY,
	who		TEXT,		--- who
	whe		TEXT,		--- where
	ind		TEXT,		--- post index
	touch	TEXT NOT NULL DEFAULT '1990-01-01 00:00:00'
);

CREATE UNIQUE INDEX "contact_who_idx" ON "contact" ( who );

INSERT INTO contact (
	id,
	who,
	whe,
	ind
) VALUES (
	0,
	'ЗАО "Нордавиа-РА"',
	'r. Архангельск, аэропорт Архангельск д.4, корп.1',
	'163053' );

