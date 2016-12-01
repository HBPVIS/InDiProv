cd .\model

odb --database mysql --generate-query --generate-schema activity.hxx
odb --database mysql --generate-query --generate-schema agent.hxx
odb --database mysql --generate-query --generate-schema entity.hxx

mysql --user=root --password=password --database=provenance < activity.sql
mysql --user=root --password=password --database=provenance < agent.sql
mysql --user=root --password=password --database=provenance < entity.sql

cd ..

//.\Debug\driver.exe --user root --password password --database provenance