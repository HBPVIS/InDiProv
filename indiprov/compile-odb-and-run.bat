cd .\model

odb --database mysql --generate-query --generate-schema activity.hxx
odb --database mysql --generate-query --generate-schema agent.hxx
odb --database mysql --generate-query --generate-schema entity.hxx
odb --database mysql --generate-query --generate-schema --fkeys-deferrable-mode not_deferrable wasGeneratedBy.hxx
odb --database mysql --generate-query --generate-schema --fkeys-deferrable-mode not_deferrable wasDerivedFrom.hxx
odb --database mysql --generate-query --generate-schema --fkeys-deferrable-mode not_deferrable wasAttributedTo.hxx
odb --database mysql --generate-query --generate-schema --fkeys-deferrable-mode not_deferrable used.hxx
odb --database mysql --generate-query --generate-schema --fkeys-deferrable-mode not_deferrable wasInformedBy.hxx

mysql --user=root --password=password -e "DROP DATABASE provenance"
mysql --user=root --password=password -e "CREATE DATABASE provenance"
mysql --user=root --password=password --database=provenance < activity.sql
mysql --user=root --password=password --database=provenance < agent.sql
mysql --user=root --password=password --database=provenance < entity.sql
mysql --user=root --password=password --database=provenance < wasGeneratedBy.sql
mysql --user=root --password=password --database=provenance < wasDerivedFrom.sql
mysql --user=root --password=password --database=provenance < wasAttributedTo.sql
mysql --user=root --password=password --database=provenance < used.sql
mysql --user=root --password=password --database=provenance < wasInformedBy.sql

cd ..