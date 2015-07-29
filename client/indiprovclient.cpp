#include "indiprovclient.h"
#include <boost/thread.hpp>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "zmq.hpp"
#include <json/json.h>
#include "provutils.h"
InDiProvClient::InDiProvClient()
{
    WFID=-1;
    WFPass="NoPass";
    context = new zmq::context_t(1);
    socket = new zmq::socket_t(*context, ZMQ_REQ);
    for (int j = 5556; j < 5560; ++j) {
        string address(("tcp://localhost:"+std::to_string(j)));
        socket->connect(address.c_str());
    }
}

string InDiProvClient::getCurrentTime(){
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 100;
    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));
    char currentTime[84] = "";
    sprintf(currentTime, "%s.%d", buffer, milli);
    return string(currentTime);
}
string InDiProvClient::ESender(string msgstr)
{
    zmq::message_t msg (msgstr.size()+1);
    memcpy ((void *)msg.data(), msgstr.c_str(), msgstr.size()+1);
    socket->send (msg);
    cout << msgstr << " sent . . ." << endl;
    zmq::message_t reply;
    socket->recv(&reply);
    char *tmp = (char*)reply.data();
    std::string str(tmp);
    return str;
    //cout << str << endl;
    //sleep(1);
}
string InDiProvClient::serializeToJson(vector<string> commandVec)
{
    json_object * jobj = json_object_new_object();

    /*Creating a json array*/
    json_object *jarray = json_object_new_array();

    for (int i = 1; i < commandVec.size(); ++i) {
        json_object *jstring1 = json_object_new_string(commandVec[i].c_str());
        json_object_array_add(jarray,jstring1);
    }
    json_object *jstrcmd = json_object_new_string(commandVec[0].c_str());
    json_object *jWFID = json_object_new_int(this->WFID);
    json_object *jWFPass = json_object_new_string((this->WFPass).c_str());

    json_object_object_add(jobj,"Command", jstrcmd);
    json_object_object_add(jobj,"WFID", jWFID);
    json_object_object_add(jobj,"WFPass", jWFPass);
    json_object_object_add(jobj,"Arguments", jarray);

    /*Now printing the json object*/
    //printf ("The json object created: %sn",json_object_to_json_string(jobj));
    //this->json_parse(jobj);
    return json_object_to_json_string(jobj);    
}
void InDiProvClient::json_parse(json_object * jobj) {
  enum json_type type;
  vector <string> commandVec;
  json_object_object_foreach(jobj, key, val) {
    type = json_object_get_type(val);
    //cout<<"type: "<<type<<endl;
    if (strcmp(key, "Command") == 0)
        commandVec.push_back(json_object_get_string(val));
    else if (strcmp(key, "Arguments") == 0){
        //void json_parse(json_object * jobj); /*Forward Declaration*/
        enum json_type type;
        json_object *jarray = json_object_object_get(jobj, key);

        int arraylen = json_object_array_length(jarray);
        cout<<"Array Length: "<< arraylen << endl;
        for (int i=0; i< arraylen; i++){
        commandVec.push_back(json_object_get_string(json_object_array_get_idx(jarray, i)));
        }
    }
  }
  for (int j = 0;  j < commandVec.size(); j++) {
      cout << commandVec[j] << endl;
  }
}
int InDiProvClient::createWF(string WFName , string password){
    vector<string> myCommand;
    myCommand.push_back("createWF");
    myCommand.push_back(WFName);
    myCommand.push_back(password);
    //boost::thread (&InDiProvClient::ESender,this,serializeToJson(myCommand));
    return atoi((ESender(serializeToJson(myCommand))).c_str());

}
int InDiProvClient::loadWF(int WFid, string password){
    vector<string> myCommand;
    myCommand.push_back("loadWF");
    myCommand.push_back(std::to_string(WFid));
    myCommand.push_back(password);
    int tmpWFid = atoi((ESender(serializeToJson(myCommand))).c_str()) ;
    if ( WFid == tmpWFid ){
        this->WFID = WFid;
        this->WFPass = password;
        return tmpWFid;
    }
    else
        return -1;
}

int InDiProvClient::exportPROV(){
    vector<string> myCommand;
    myCommand.push_back("exportPROV");
    myCommand.push_back("Reserve");
    return atoi((ESender(serializeToJson(myCommand))).c_str());

}
int InDiProvClient::getWFID(){
    return this->WFID ;
}
string InDiProvClient::getWFPass(){
    return this->WFPass;
}
int InDiProvClient::setEntity(string label, string location, string type, string val){
    vector<string> myCommand;
    myCommand.push_back("setEntity");
    myCommand.push_back(label);
    myCommand.push_back(location);
    myCommand.push_back(type);
    myCommand.push_back(val);
    return atoi((ESender(serializeToJson(myCommand))).c_str());

}

int InDiProvClient::setActivity(string startTime, string endTime, string label, string location, string type){
    vector<string> myCommand;
    myCommand.push_back("setActivity");
    myCommand.push_back(startTime);
    myCommand.push_back(endTime);
    myCommand.push_back(label);
    myCommand.push_back(location);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());

}
int InDiProvClient::setAgent(string label, string location, string type){
    vector<string> myCommand;
    myCommand.push_back("setAgent");
    myCommand.push_back(label);
    myCommand.push_back(location);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}
int InDiProvClient::setused(int actID,int entID,string usedTime,string label, string location, string role, string type){
    vector<string> myCommand;
    myCommand.push_back("setused");
    myCommand.push_back(std::to_string(actID));
    myCommand.push_back(std::to_string(entID));
    myCommand.push_back(usedTime);
    myCommand.push_back(label);
    myCommand.push_back(location);
    myCommand.push_back(role);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}
int InDiProvClient::setwasGeneratedBy(int entID,int actID, string generateTime, string label, string location, string role, string type){
    vector<string> myCommand;
    myCommand.push_back("setwasGeneratedBy");
    myCommand.push_back(std::to_string(entID));
    myCommand.push_back(std::to_string(actID));
    myCommand.push_back(generateTime);
    myCommand.push_back(label);
    myCommand.push_back(location);
    myCommand.push_back(role);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}
int InDiProvClient::setwasDerivedFrom(int genEntID, int usdEntID, int actID, int genID, int usgID, string label, string type){
    vector<string> myCommand;
    myCommand.push_back("setwasDerivedFrom");
    myCommand.push_back(std::to_string(genEntID));
    myCommand.push_back(std::to_string(usdEntID));
    myCommand.push_back(std::to_string(actID));
    myCommand.push_back(std::to_string(genID));
    myCommand.push_back(std::to_string(usgID));
    myCommand.push_back(label);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}
int InDiProvClient::setwasAttributedTo(int entID,int agentID,string label, string type){
    vector<string> myCommand;
    myCommand.push_back("setwasAttributedTo");
    myCommand.push_back(std::to_string(entID));
    myCommand.push_back(std::to_string(agentID));
    myCommand.push_back(label);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}
int InDiProvClient::setwasAssociatedWith(int actID, int agentID, int planID, string label, string role, string type){
    vector<string> myCommand;
    myCommand.push_back("setwasAssociatedWith");
    myCommand.push_back(std::to_string(actID));
    myCommand.push_back(std::to_string(agentID));
    myCommand.push_back(std::to_string(planID));
    myCommand.push_back(label);
    myCommand.push_back(role);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}
int InDiProvClient::setwasInformedBy(int informed, int informant, string label, string type){
    vector<string> myCommand;
    myCommand.push_back("setwasInformedBy");
    myCommand.push_back(std::to_string(informed));
    myCommand.push_back(std::to_string(informant));
    myCommand.push_back(label);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}
int InDiProvClient::setwasStartedBy(int actID, int entID, int starterActID, string sTime, string label, string location, string role, string type){
    vector<string> myCommand;
    myCommand.push_back("setwasStartedBy");
    myCommand.push_back(std::to_string(actID));
    myCommand.push_back(std::to_string(entID));
    myCommand.push_back(std::to_string(starterActID));
    myCommand.push_back(sTime);
    myCommand.push_back(label);
    myCommand.push_back(location);
    myCommand.push_back(role);
    myCommand.push_back(type);
std::cout<<" setwasStartedBy vector created successfuly" << std::endl;
    return atoi((ESender(serializeToJson(myCommand))).c_str());

}
int InDiProvClient::setwasEndedBy(int actID, int entID, int enderActID, string eTime, string label, string location, string role, string type){
    vector<string> myCommand;
    myCommand.push_back("setwasEndedBy");
    myCommand.push_back(std::to_string(actID));
    myCommand.push_back(std::to_string(entID));
    myCommand.push_back(std::to_string(enderActID));
    myCommand.push_back(eTime);
    myCommand.push_back(label);
    myCommand.push_back(location);
    myCommand.push_back(role);
    myCommand.push_back(type);
    return atoi((ESender(serializeToJson(myCommand))).c_str());
}

string InDiProvClient::getEntities(){
    vector<string> myCommand;
    myCommand.push_back("getEntities");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}

string InDiProvClient::getActivities(){
    vector<string> myCommand;
    myCommand.push_back("getActivities");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}

string InDiProvClient::getAgents(){
    vector<string> myCommand;
    myCommand.push_back("getAgents");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}

string InDiProvClient::getUseds(){
    vector<string> myCommand;
    myCommand.push_back("getUseds");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}
string InDiProvClient::getWasGeneratedBys(){
    vector<string> myCommand;
    myCommand.push_back("getWasGeneratedBys");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}
string InDiProvClient::getWasDerivedFroms(){
    vector<string> myCommand;
    myCommand.push_back("getWasDerivedFroms");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}
string InDiProvClient::getWasAttributedTos(){
    vector<string> myCommand;
    myCommand.push_back("getWasAttributedTos");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}
string InDiProvClient::getWasAssociatedWiths(){
    vector<string> myCommand;
    myCommand.push_back("getWasAssociatedWiths");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}
string InDiProvClient::getwasInformedBys(){
    vector<string> myCommand;
    myCommand.push_back("getwasInformedBys");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}
string InDiProvClient::getwasStartedBys(){
    vector<string> myCommand;
    myCommand.push_back("getwasStartedBys");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}
string InDiProvClient::getwasEndedBys(){
    vector<string> myCommand;
    myCommand.push_back("getwasEndedBys");
    myCommand.push_back("Reserve");
    return ESender(serializeToJson(myCommand));
}

//deserialize
vector<ProvUtils::Entity> InDiProvClient::deSerializeEntities(char *entitiesStr){
    json_object * jobj = json_tokener_parse(entitiesStr);
    vector<ProvUtils::Entity> entities;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "entities") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::Entity tmpEntity;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "label") == 0)
                        tmpEntity.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "location") == 0)
                        tmpEntity.location=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpEntity.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "value") == 0)
                        tmpEntity.value=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpEntity.ID=json_object_get_string(tmpval);
                }
                entities.push_back(tmpEntity);
            }
        }
    }
    return entities;
}
vector<ProvUtils::Activity> InDiProvClient::deSerializeActivities(char *activitiesStr){
    json_object * jobj = json_tokener_parse(activitiesStr);
    vector<ProvUtils::Activity> activities;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "activities") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::Activity tmpActivity;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "startTime") == 0)
                        tmpActivity.startTime=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "endTime") == 0)
                        tmpActivity.endTime=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpActivity.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "location") == 0)
                        tmpActivity.location=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpActivity.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpActivity.ID=json_object_get_string(tmpval);
                }
                activities.push_back(tmpActivity);
            }
        }
    }
    return activities;
}
vector<ProvUtils::Agent> InDiProvClient::deSerializeAgents(char *agentsStr){
    json_object * jobj = json_tokener_parse(agentsStr);
    vector<ProvUtils::Agent> agents;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "agents") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::Agent tmpAgent;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "label") == 0)
                        tmpAgent.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "location") == 0)
                        tmpAgent.location=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpAgent.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpAgent.ID=json_object_get_string(tmpval);
                }
                agents.push_back(tmpAgent);
            }
        }
    }
    return agents;
}
vector<ProvUtils::Used> InDiProvClient::deSerializeUseds(char *usedsStr){
    json_object * jobj = json_tokener_parse(usedsStr);
    vector<ProvUtils::Used> useds;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "useds") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::Used tmpUsed;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "actID") == 0)
                        tmpUsed.actID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "entID") == 0)
                        tmpUsed.entID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "usedTime") == 0)
                        tmpUsed.usedTime=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpUsed.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "location") == 0)
                        tmpUsed.location=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "role") == 0)
                        tmpUsed.role=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpUsed.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpUsed.ID=json_object_get_string(tmpval);
                }
                useds.push_back(tmpUsed);
            }
        }
    }
    return useds;

}
vector<ProvUtils::WasGeneratedBy> InDiProvClient::deSerializeWasGeneratedBys(char *wasGeneratedBysStr){
    json_object * jobj = json_tokener_parse(wasGeneratedBysStr);
    vector<ProvUtils::WasGeneratedBy> wasGeneratedBys;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "wasGeneratedBys") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::WasGeneratedBy tmpWasGeneratedBy;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "actID") == 0)
                        tmpWasGeneratedBy.actID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "entID") == 0)
                        tmpWasGeneratedBy.entID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "generateTime") == 0)
                        tmpWasGeneratedBy.generateTime=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpWasGeneratedBy.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "location") == 0)
                        tmpWasGeneratedBy.location=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "role") == 0)
                        tmpWasGeneratedBy.role=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpWasGeneratedBy.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpWasGeneratedBy.ID=json_object_get_string(tmpval);
                }
                wasGeneratedBys.push_back(tmpWasGeneratedBy);
            }
        }
    }
    return wasGeneratedBys;
}
vector<ProvUtils::WasDerivedFrom> InDiProvClient::deSerializeWasDerivedFroms(char *wasDerivedFromsStr){
    json_object * jobj = json_tokener_parse(wasDerivedFromsStr);
    vector<ProvUtils::WasDerivedFrom> wasDerivedFroms;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "wasDerivedFroms") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::WasDerivedFrom tmpWasDerivedFrom;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "genEntID") == 0)
                        tmpWasDerivedFrom.genEntID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "usdEntID") == 0)
                        tmpWasDerivedFrom.usdEntID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "actID") == 0)
                        tmpWasDerivedFrom.actID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "genID") == 0)
                        tmpWasDerivedFrom.genID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "usgID") == 0)
                        tmpWasDerivedFrom.usgID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpWasDerivedFrom.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpWasDerivedFrom.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpWasDerivedFrom.ID=json_object_get_string(tmpval);
                }
                wasDerivedFroms.push_back(tmpWasDerivedFrom);
            }
        }
    }
    return wasDerivedFroms;
}
vector<ProvUtils::WasAttributedTo> InDiProvClient::deSerializeWasAttributedTos(char *wasAttributedTosStr){
    json_object * jobj = json_tokener_parse(wasAttributedTosStr);
    vector<ProvUtils::WasAttributedTo> wasAttributedTos;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "wasAttributedTos") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::WasAttributedTo tmpWasAttributedTo;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "entID") == 0)
                        tmpWasAttributedTo.entID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "agentID") == 0)
                        tmpWasAttributedTo.agentID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpWasAttributedTo.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpWasAttributedTo.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpWasAttributedTo.ID=json_object_get_string(tmpval);
                }
                wasAttributedTos.push_back(tmpWasAttributedTo);
            }
        }
    }
    return wasAttributedTos;
}
vector<ProvUtils::WasAssociatedWith> InDiProvClient::deSerializeWasAssociatedWiths(char *wasAssociatedWithsStr){
    json_object * jobj = json_tokener_parse(wasAssociatedWithsStr);
    vector<ProvUtils::WasAssociatedWith> wasAssociatedWiths;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "wasAssociatedWiths") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::WasAssociatedWith tmpWasAssociatedWith;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "actID") == 0)
                        tmpWasAssociatedWith.actID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "agentID") == 0)
                        tmpWasAssociatedWith.agentID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "planID") == 0)
                        tmpWasAssociatedWith.planID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpWasAssociatedWith.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "role") == 0)
                        tmpWasAssociatedWith.role=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpWasAssociatedWith.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpWasAssociatedWith.ID=json_object_get_string(tmpval);
                }
                wasAssociatedWiths.push_back(tmpWasAssociatedWith);
            }
        }
    }
    return wasAssociatedWiths;
}
vector<ProvUtils::WasInformedBy> InDiProvClient::deSerializeWasInformedBys(char *wasInformedByStr){
    json_object * jobj = json_tokener_parse(wasInformedByStr);
    vector<ProvUtils::WasInformedBy> wasInformedBys;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "wasInformedBys") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::WasInformedBy tmpWasInformedBy;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "informed") == 0)
                        tmpWasInformedBy.informed=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "informant") == 0)
                        tmpWasInformedBy.informant=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpWasInformedBy.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpWasInformedBy.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpWasInformedBy.ID=json_object_get_string(tmpval);
                }
                wasInformedBys.push_back(tmpWasInformedBy);
            }
        }
    }
    return wasInformedBys;
}
vector<ProvUtils::WasStartedBy> InDiProvClient::deSerializeWasStartedBys(char *wasStartedBysStr){
    json_object * jobj = json_tokener_parse(wasStartedBysStr);
    vector<ProvUtils::WasStartedBy> wasStartedBys;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "wasStartedBys") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::WasStartedBy tmpWasStartedBy;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "actID") == 0)
                        tmpWasStartedBy.actID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "entID") == 0)
                        tmpWasStartedBy.entID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "starterActID") == 0)
                        tmpWasStartedBy.starterActID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "sTime") == 0)
                        tmpWasStartedBy.sTime=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpWasStartedBy.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "location") == 0)
                        tmpWasStartedBy.location=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "role") == 0)
                        tmpWasStartedBy.role=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpWasStartedBy.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpWasStartedBy.ID=json_object_get_string(tmpval);
                }
                wasStartedBys.push_back(tmpWasStartedBy);
            }
        }
    }
    return wasStartedBys;
}
vector<ProvUtils::WasEndedBy> InDiProvClient::deSerializeWasEndedBys(char *WasEndedBysStr){
    json_object * jobj = json_tokener_parse(WasEndedBysStr);
    vector<ProvUtils::WasEndedBy> wasEndedBys;
    json_object_object_foreach(jobj, key, val) {
        if (strcmp(key, "wasEndedBys") == 0){
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            for (int i=0; i< arraylen; i++){
                json_object * tmpjobj = json_object_array_get_idx(jarray, i);
                ProvUtils::WasEndedBy tmpWasEndedBy;
                json_object_object_foreach(tmpjobj, tmpkey, tmpval){
                    if (strcmp(tmpkey, "actID") == 0)
                        tmpWasEndedBy.actID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "entID") == 0)
                        tmpWasEndedBy.entID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "enderActID") == 0)
                        tmpWasEndedBy.enderActID=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "eTime") == 0)
                        tmpWasEndedBy.eTime=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "label") == 0)
                        tmpWasEndedBy.label=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "location") == 0)
                        tmpWasEndedBy.location=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "role") == 0)
                        tmpWasEndedBy.role=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "type") == 0)
                        tmpWasEndedBy.type=json_object_get_string(tmpval);
                    else if (strcmp(tmpkey, "ID") == 0)
                        tmpWasEndedBy.ID=json_object_get_string(tmpval);
                }
                wasEndedBys.push_back(tmpWasEndedBy);
            }
        }
    }
    return wasEndedBys;
}
