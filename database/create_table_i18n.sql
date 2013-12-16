
CREATE TABLE "konverter"."i18n" (
	id		int	UNIQUE NOT NULL REFERENCES "konverter"."contact"( id ) ON UPDATE CASCADE ON DELETE CASCADE,
	who		varchar,
	whe		varchar,
	ind		varchar,
	city	varchar
);

INSERT INTO "konverter"."i18n" ( id ) VALUES ( 0 );
