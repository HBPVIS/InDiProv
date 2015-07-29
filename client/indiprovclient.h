#ifndef INDIPROVCLIENT_H
#define INDIPROVCLIENT_H
#include "zmq.hpp"
#include <json/json.h>
#include "provutils.h"
using namespace std;

class InDiProvClient
{
public:
    InDiProvClient();
    string ESender(string msgstr);
    string serializeToJson(vector<string> commandVec);
    void json_parse(json_object * jobj);
    string getCurrentTime();
    //WF functions
    int createWF(string WFName , string password);
    int loadWF(int WFid , string password);
    int getWFID();
    int exportPROV();
    string getWFPass();
    //prov functions
    int setEntity(string label, string location, string type, string val);
    int setActivity(string startTime, string endTime, string label, string location, string type);
    int setAgent(string label, string location, string type);
    int setused(int actID,int entID,string usedTime,string label, string location, string role, string type);
    int setwasGeneratedBy(int entID,int actID, string generateTime, string label, string location, string role, string type);
    int setwasDerivedFrom(int genEntID, int usdEntID, int actID, int genID, int usgID, string label, string type);
    int setwasAttributedTo(int entID,int agentID,string label, string type);
    int setwasAssociatedWith(int actID, int agentID, int planID, string label, string role, string type);
    int setwasInformedBy(int informed, int informant, string label, string type);
    int setwasStartedBy(int actID, int entID, int starterActID, string sTime, string label, string location, string role, string type);
    int setwasEndedBy(int actID, int entID, int enderActID, string eTime, string label, string location, string role, string type);
    //prov query function
    string getEntities();
    string getActivities();
    string getAgents();
    string getUseds();
    string getWasGeneratedBys();
    string getWasDerivedFroms();
    string getWasAttributedTos();
    string getWasAssociatedWiths();
    string getwasInformedBys();
    string getwasStartedBys();
    string getwasEndedBys();

    //deserialize
    vector<ProvUtils::Entity> deSerializeEntities(char *entitiesStr);
    vector<ProvUtils::Activity> deSerializeActivities(char *activitiesStr);
    vector<ProvUtils::Agent> deSerializeAgents(char *agentsStr);    
    vector<ProvUtils::Used> deSerializeUseds(char *usedsStr);
    vector<ProvUtils::WasGeneratedBy> deSerializeWasGeneratedBys(char *wasGeneratedBysStr);
    vector<ProvUtils::WasDerivedFrom> deSerializeWasDerivedFroms(char *wasDerivedFromsStr);
    vector<ProvUtils::WasAttributedTo> deSerializeWasAttributedTos(char *wasAttributedToStr);
    vector<ProvUtils::WasAssociatedWith> deSerializeWasAssociatedWiths(char *wasAssociatedWithsStr);
    vector<ProvUtils::WasInformedBy>  deSerializeWasInformedBys(char *wasInformedByStr);
    vector<ProvUtils::WasStartedBy> deSerializeWasStartedBys(char *wasStartedBysStr);
    vector<ProvUtils::WasEndedBy> deSerializeWasEndedBys(char *WasEndedBysStr);


private:
    int WFID;
    string WFPass;
    zmq::context_t* context;
    zmq::socket_t* socket;
    timeval curTime;
};

#endif // INDIPROVCLIENT_H
