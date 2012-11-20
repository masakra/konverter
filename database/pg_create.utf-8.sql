\encoding UTF-8

DROP SCHEMA "konverter" CASCADE;

CREATE SCHEMA "konverter";

CREATE TABLE "konverter"."contact" (
	id		SERIAL UNIQUE,
	who		varchar,
	whe		varchar,
	ind		varchar,
	city	varchar,
	touch	timestamp without time zone NOT NULL DEFAULT "now"()
);

INSERT INTO "konverter"."contact" (
	id,
	who,
	whe,
	ind,
	city
) VALUES (
	0,
	'ЗАО «Нордавиа-РА»',
	'r. Архангельск, аэропорт Архангельск д.4, корп.1',
	'163053',
	'Архангельск' );

CREATE TABLE "konverter"."log" (
	contact_id	integer NOT NULL REFERENCES "konverter"."contact" ( id ) ON DELETE CASCADE,
	num_text	varchar,
	num			varchar,
	zakaz		integer NOT NULL DEFAULT 0,
	drec		timestamp without time zone NOT NULL DEFAULT "now"()
);

