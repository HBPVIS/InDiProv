cd .\model

odb --database mysql --generate-query --generate-schema activity.hxx
odb --database mysql --generate-query --generate-schema agent.hxx
odb --database mysql --generate-query --generate-schema entity.hxx
odb --database mysql --generate-query --generate-schema --fkeys-deferrable-mode not_deferrable wasAttributedTo.hxx
odb --database mysql --generate-query --generate-schema --fkeys-deferrable-mode not_deferrable wasGeneratedBy.hxx

mysql --user=root --password=password -e "DROP DATABASE provenance"
mysql --user=root --password=password -e "CREATE DATABASE provenance"
mysql --user=root --password=password --database=provenance < activity.sql
mysql --user=root --password=password --database=provenance < agent.sql
mysql --user=root --password=password --database=provenance < entity.sql
mysql --user=root --password=password --database=provenance < wasAttributedTo.sql
mysql --user=root --password=password --database=provenance < wasGeneratedBy.sql

cd ..