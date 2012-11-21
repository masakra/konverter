REVOKE ALL ON "konverter"."log" FROM "konverter_user";
REVOKE ALL ON "konverter"."contact" FROM "konverter_user";
REVOKE ALL ON SEQUENCE "konverter"."contact_id_seq" FROM "konverter_user";

REVOKE ALL ON "konverter"."log" FROM "konverter_reader";
REVOKE ALL ON "konverter"."contact" FROM "konverter_reader";

REVOKE ALL ON SCHEMA "konverter" FROM "konverter_reader";

REVOKE "konverter_reader" FROM "konverter_user";

DROP ROLE "konverter_user", "konverter_reader";

--- konverter_reader

CREATE ROLE "konverter_reader";

GRANT USAGE ON SCHEMA "konverter" TO "konverter_reader";

GRANT SELECT ON "konverter"."contact" TO "konverter_reader";

GRANT SELECT ON "konverter"."log" TO "konverter_reader";

--- konverter_user

CREATE ROLE "konverter_user";

GRANT "konverter_reader" TO "konverter_user";

GRANT INSERT, UPDATE, DELETE ON "konverter"."contact" TO "konverter_user";
GRANT SELECT, UPDATE ON SEQUENCE "konverter"."contact_id_seq" TO "konverter_user";

GRANT INSERT, UPDATE, DELETE ON "konverter"."log" TO "konverter_user";

