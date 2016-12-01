# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README hello-mysql-vc8.vcproj hello-sqlite-vc8.vcproj hello-pgsql-vc8.vcproj hello-oracle-vc8.vcproj hello-mssql-vc8.vcproj hello-mysql-vc9.vcproj hello-sqlite-vc9.vcproj hello-pgsql-vc9.vcproj hello-oracle-vc9.vcproj hello-mssql-vc9.vcproj hello-mysql-vc10.vcxproj hello-sqlite-vc10.vcxproj hello-pgsql-vc10.vcxproj hello-oracle-vc10.vcxproj hello-mssql-vc10.vcxproj hello-mysql-vc10.vcxproj.filters hello-sqlite-vc10.vcxproj.filters hello-pgsql-vc10.vcxproj.filters hello-oracle-vc10.vcxproj.filters hello-mssql-vc10.vcxproj.filters hello-mysql-vc11.vcxproj hello-sqlite-vc11.vcxproj hello-pgsql-vc11.vcxproj hello-oracle-vc11.vcxproj hello-mssql-vc11.vcxproj hello-mysql-vc11.vcxproj.filters hello-sqlite-vc11.vcxproj.filters hello-pgsql-vc11.vcxproj.filters hello-oracle-vc11.vcxproj.filters hello-mssql-vc11.vcxproj.filters hello-mysql-vc12.vcxproj hello-sqlite-vc12.vcxproj hello-pgsql-vc12.vcxproj hello-oracle-vc12.vcxproj hello-mssql-vc12.vcxproj hello-mysql-vc12.vcxproj.filters hello-sqlite-vc12.vcxproj.filters hello-pgsql-vc12.vcxproj.filters hello-oracle-vc12.vcxproj.filters hello-mssql-vc12.vcxproj.filters

noinst_PROGRAMS = driver
driver_SOURCES = driver.cxx database.hxx  

AM_CPPFLAGS = -I'$(builddir)' -I'$(srcdir)'

if DATABASE_MYSQL
AM_CPPFLAGS += -DDATABASE_MYSQL
endif

if DATABASE_SQLITE
AM_CPPFLAGS += -DDATABASE_SQLITE
endif

if DATABASE_PGSQL
AM_CPPFLAGS += -DDATABASE_PGSQL
endif

if DATABASE_ORACLE
AM_CPPFLAGS += -DDATABASE_ORACLE
endif

if DATABASE_MSSQL
AM_CPPFLAGS += -DDATABASE_MSSQL
endif

TESTS=$(top_builddir)/tester
TESTS_ENVIRONMENT=top_builddir=$(top_builddir); export top_builddir;

# ODB compilation.
#
driver_SOURCES += person.hxx
nodist_driver_SOURCES = person-odb.cxx
BUILT_SOURCES = person-odb.hxx
CLEANFILES = person-odb.hxx person-odb.ixx person-odb.cxx

ODB = @ODB@
ODBFLAGS = @ODBFLAGS@
ODBCPPFLAGS = @ODBCPPFLAGS@

if HAVE_CXX11
ODBFLAGS += --std c++11
endif

person-odb.hxx: person.hxx
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-schema $<
