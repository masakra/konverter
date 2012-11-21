
PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS contact;

CREATE TABLE contact (
	id		INTEGER PRIMARY KEY,
	who		TEXT,		--- who
	whe		TEXT,		--- where
	ind		TEXT,		--- post index
	touch	TEXT NOT NULL DEFAULT '1990-01-01 00:00:00',
	city	TEXT
);

----CREATE UNIQUE INDEX "contact_who_idx" ON "contact" ( who );

INSERT INTO contact (
	id,
	who,
	whe,
	ind
) VALUES (
	0,
	'ЗАО «Нордавиа-РА»',
	'r. Архангельск, аэропорт Архангельск д.4, корп.1',
	'163053' );

DROP TABLE IF EXISTS log;

CREATE TABLE log (
	contact_id	INTEGER NOT NULL REFERENCES contact ( id ) ON DELETE CASCADE,
	num_text	TEXT,
	num			TEXT,
	zakaz		INTEGER NOT NULL DEFAULT 0,
	timestamp	TEXT NOT NULL DEFAULT ( "current_timestamp"() )
);

