#ifndef INDIPROV_H
#define INDIPROV_H

#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "provutils.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
using namespace std;
class InDiProv
{
public:
    InDiProv();
    virtual ~InDiProv();
    //Prov function
    int setEntity(int wfid, string label, string location, string type, string val);
    int setActivity(int wfid, string startTime, string endTime, string label, string location, string type);
    int setAgent(int wfid, string label, string location, string type);
    int setused(int wfid, int actID,int entID,string usedTime,string label, string location, string role, string type);
    int setwasGeneratedBy(int wfid, int entID,int actID, string generateTime, string label, string location, string role, string type);
    int setwasDerivedFrom(int wfid, int genEntID, int usdEntID, int actID, int genID, int usgID, string label, string type);
    int setwasAttributedTo(int wfid, int entID,int agentID,string label, string type);
    int setwasAssociatedWith(int wfid, int actID, int agentID, int planID, string label, string role, string type);
    int setwasInformedBy(int wfid, int informed, int informant, string label, string type);
    int setwasStartedBy(int wfid, int actID, int entID, int starterActID, string sTime, string label, string location, string role, string type);
    int setwasEndedBy(int wfid, int actID, int entID, int enderActID, string eTime, string label, string location, string role, string type);
    //prov Query
    vector<ProvUtils::Entity> getEntities(int wfid);
    vector<ProvUtils::Agent> getAgents(int wfid);
    vector<ProvUtils::Activity> getActivities(int wfid);
    vector<ProvUtils::Used> getUseds(int wfid);
    vector<ProvUtils::WasGeneratedBy> getWasGeneratedBys(int wfid);
    vector<ProvUtils::WasDerivedFrom> getWasDerivedFroms(int wfid);
    vector<ProvUtils::WasAttributedTo> getWasAttributedTos(int wfid);
    vector<ProvUtils::WasAssociatedWith> getWasAssociatedWiths(int wfid);
    vector<ProvUtils::WasInformedBy>  getWasInformedBys(int wfid);
    vector<ProvUtils::WasStartedBy> getwasStartedBys(int wfid);
    vector<ProvUtils::WasEndedBy> getwasEndedBys(int wfid);

    //workflow function
    int createWF(string WFName , string password);
    int checkPass(int WFid , string password);
    int exportPROV(int wfid);
    //prov utils
    int getEntityID(int wfid, string pName);
    int getAgentID(int wfid, string aName);
    //utils
    string getCurrentTime();
    string queryParsing(char *tmp);
    //serialization
    string serializeEntities(vector<ProvUtils::Entity>);
    string serializeActivities(vector<ProvUtils::Activity> activities);
    string serializeAgents(vector<ProvUtils::Agent> agents);
    string serializeUseds(vector<ProvUtils::Used> useds);
    string serializeWasGeneratedBys(vector<ProvUtils::WasGeneratedBy> wasGeneratedBys);
    string serializeWasDerivedFroms(vector<ProvUtils::WasDerivedFrom> wasDerivedFroms);
    string serializeWasAttributedTos(vector<ProvUtils::WasAttributedTo> wasAttributedTos);
    string serializeWasAssociatedWiths(vector<ProvUtils::WasAssociatedWith> wasAssociatedWiths);
    string serializeWasInformedBys(vector<ProvUtils::WasInformedBy> wasInformedBys);
    string serializeWasStartedBys(vector<ProvUtils::WasStartedBy> wasStartedBys);
    string serializeWasEndedBys(vector<ProvUtils::WasEndedBy> wasEndedBys);

private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    timeval curTime;
};

#endif // INDIPROV_H
