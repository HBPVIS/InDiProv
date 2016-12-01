#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "model/agent.hxx"
#include "model/agent-odb.hxx"
#include "dbactions.hxx"

using namespace std;
using namespace odb::core;

int
main (int argc, char* argv[])
{
  try
  {
    auto_ptr<database> db (create_database (argc, argv));

    unsigned long john_id, joe_id;

	cout << createAgent(db, "cyremur");

    // Create a few persistent person objects.
    //
    {
      agent john ("John");
	  agent jane ("Jane");
	  agent joe ("Joe");

      transaction t (db->begin ());

      // Make objects persistent and save their ids for later use.
      //
      john_id = db->persist (john);
      db->persist (jane);
      joe_id = db->persist (joe);

      t.commit ();
    }

  }
  catch (const odb::exception& e)
  {
    cerr << e.what () << endl;
	string nope;
	cin >> nope;
    return 1;
  }
  string nope;
  cin >> nope;
  return 1;
}
