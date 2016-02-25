#include "indiprov.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <json/json.h>
#include "sys/time.h"
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "provutils.h"
#include "provc.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
using namespace std;
InDiProv::InDiProv(){
    driver=sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "pass");
}
InDiProv::~InDiProv()
{
    delete con;
}
int InDiProv::getEntityID(int wfid, string eName){
    int rowcount=0;
    int eID;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;

    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Entity where WFID=? and inDiProv.Entity.label=?");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setString(2,eName);
    res = prep_stmt->executeQuery();

    while (res->next()) {
        eID=res->getInt("ID");
        rowcount++;
    }

    if (rowcount==1){
        delete res;
        delete prep_stmt;
        return eID;
    }
    else if (rowcount==0) {
        //Reading max Artifact ID from Artifact table
        prep_stmt = con->prepareStatement("select MAX(inDiProv.Entity.ID) as eID from inDiProv.Entity");
        res = prep_stmt->executeQuery();
        while (res->next()) {
            eID=res->getInt("eID");
        }
        prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Entity (ID, WFID, label) VALUES (?,?,?)");
        prep_stmt->setInt(1, eID+1);
        prep_stmt->setInt(2, wfid);
        prep_stmt->setString(3, eName);
        prep_stmt->execute();
        delete res;
        delete prep_stmt;
        return eID+1;
    }
    else {
        std::cout<<"Error : Entity ID should be unique "<<std::endl;
        delete res;
        delete prep_stmt;
        return -1;
    }
}
int InDiProv::getAgentID(int wfid, string aName){
    int rowcount=0;
    int aID;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;

    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Agent where WFID=? and inDiProv.Agent.label=?");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setString(2,aName);
    res = prep_stmt->executeQuery();

    while (res->next()) {
        aID=res->getInt("ID");
        rowcount++;
    }

    if (rowcount==1){
        delete res;
        delete prep_stmt;
        return aID;
    }
    else if (rowcount==0) {
        //Reading max Artifact ID from Artifact table
        prep_stmt = con->prepareStatement("select MAX(inDiProv.Agent.ID) as aID from inDiProv.Agent");
        res = prep_stmt->executeQuery();
        while (res->next()) {
            aID=res->getInt("aID");
        }
        prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Agent (ID, WFID, label) VALUES (?,?,?)");
        prep_stmt->setInt(1, aID+1);
        prep_stmt->setInt(2, wfid);
        prep_stmt->setString(3, aName);
        prep_stmt->execute();
        delete res;
        delete prep_stmt;
        return aID+1;
    }
    else {
        std::cout<<"Error : Agent ID should be unique "<<std::endl;
        delete res;
        delete prep_stmt;
        return -1;
    }
}
int InDiProv::setEntity(int wfid, string label, string location, string type, string val){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;

    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Entity (WFID, label, location, type, value) VALUES (?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setString(2, label);
    prep_stmt->setString(3, location);
    prep_stmt->setString(4, type);
    prep_stmt->setString(5, val);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;

    return lastID;
}
int InDiProv::setActivity(int wfid, string startTime, string endTime, string label, string location, string type){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Activity (WFID, startTime, endTime, label, location, type) VALUES (?,?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setDateTime(2, startTime);
    prep_stmt->setDateTime(3, endTime);
    prep_stmt->setString(4, label);
    prep_stmt->setString(5, location);
    prep_stmt->setString(6, type);
    prep_stmt->execute();
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;

    return lastID;
}
int InDiProv::setAgent(int wfid, string label, string location, string type){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Agent (WFID, label, location, type) VALUES (?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setString(2, label);
    prep_stmt->setString(3, location);
    prep_stmt->setString(4, type);
    prep_stmt->execute();
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    return lastID;
}
int InDiProv::setused(int wfid, int actID, int entID, string usedTime, string label, string location, string role, string type){
    int lastID;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Usage(WFID, activity, entity, time, label, location, role, type) VALUES (?,?,?,?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    //prep_stmt->setInt(2, getActivityID(wfid,actID));
    prep_stmt->setInt(2, actID);
    //prep_stmt->setInt(3, getEntityID(wfid,entID));
    prep_stmt->setInt(3, entID);
    // change the parametere to original time value
    prep_stmt->setDateTime(4,usedTime);
    prep_stmt->setString(5, label);
    prep_stmt->setString(6, location);
    prep_stmt->setString(7, role);
    prep_stmt->setString(8, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();

    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    return lastID;
    //this->InsertActivity(aName, pName, dt ,1,Annotation);

}
int InDiProv::setwasGeneratedBy(int wfid, int entID, int actID, string generateTime, string label, string location, string role, string type){

    int lastID;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;

    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Generation (WFID,entity, activity, time, label, location, role, type) VALUES (?,?,?,?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setInt(2, entID);
    prep_stmt->setInt(3, actID);
    // change the parametere to original time value
    prep_stmt->setDateTime(4,generateTime);
    prep_stmt->setString(5, label);
    prep_stmt->setString(6, location);
    prep_stmt->setString(7, role);
    prep_stmt->setString(8, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();

    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    return lastID;
}
int InDiProv::setwasDerivedFrom(int wfid, int genEntID, int usdEntID, int actID, int genID, int usgID, string label, string type){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Derivation (WFID, generatedEntity, usedEntity, activity, generation, `usage`, label, type) VALUES (?,?,?,?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setInt(2, genEntID);
    prep_stmt->setInt(3, usdEntID);
    prep_stmt->setInt(4, actID);
    prep_stmt->setInt(5, genID);
    prep_stmt->setInt(6, usgID);
    prep_stmt->setString(7, label);
    prep_stmt->setString(8, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    cout << "derivation: " << lastID << endl;
    return lastID;

}
int InDiProv::setwasAssociatedWith(int wfid, int actID, int agentID, int planID, string label,string role, string type){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Association (WFID, activity, agent, plan, label, role, type) VALUES (?,?,?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setInt(2, actID);
    prep_stmt->setInt(3, agentID);
    prep_stmt->setInt(4, planID);
    prep_stmt->setString(5, label);
    prep_stmt->setString(6, role);
    prep_stmt->setString(7, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    return lastID;
}
int InDiProv::setwasAttributedTo(int wfid, int entID,int agentID,string label, string type){
    int lastID;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Attribution (WFID, entity, agent, label, type) VALUES (?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setInt(2, entID);
    prep_stmt->setInt(3, agentID);
    prep_stmt->setString(4, label);
    prep_stmt->setString(5, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;

    return lastID;
}
int InDiProv::setwasInformedBy(int wfid, int informed, int informant, string label, string type){
    int lastID;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Communication (WFID, informed, informant, label, type) VALUES (?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setInt(2, informed);
    prep_stmt->setInt(3, informant);
    prep_stmt->setString(4, label);
    prep_stmt->setString(5, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;

    return lastID;
}
int InDiProv::setwasStartedBy(int wfid, int actID, int entID, int starterActID, string sTime, string label, string location, string role, string type){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;

    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Start (WFID, activity, `trigger`, starter, time, label, location, role, type) VALUES ( ?,?,?,?,?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setInt(2, actID);
    prep_stmt->setInt(3, entID);
    prep_stmt->setInt(4, starterActID);
    prep_stmt->setDateTime(5,sTime);
    prep_stmt->setString(6, label);
    prep_stmt->setString(7, location);
    prep_stmt->setString(8, role);
    prep_stmt->setString(9, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    return lastID;

}
int InDiProv::setwasEndedBy(int wfid, int actID, int entID, int enderActID, string eTime, string label, string location, string role, string type){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.End (WFID, activity, `trigger`, ender, time, label, location, role, type) VALUES ( ?,?,?,?,?,?,?,?,?)");
    prep_stmt->setInt(1, wfid);
    prep_stmt->setInt(2, actID);
    prep_stmt->setInt(3, entID);
    prep_stmt->setInt(4, enderActID);
    prep_stmt->setDateTime(5,eTime);
    prep_stmt->setString(6, label);
    prep_stmt->setString(7, location);
    prep_stmt->setString(8, role);
    prep_stmt->setString(9, type);
    prep_stmt->execute();
    //
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    return lastID;

}
//prov query
vector<ProvUtils::Entity> InDiProv::getEntities(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::Entity> entities;
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Entity where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        entities.push_back(ProvUtils::Entity{res->getString("label"),res->getString("location"),res->getString("type"),res->getString("value"),std::to_string(res->getInt("ID"))});
    /*debug vector
    cout << "getEntities debug . . . "<<endl;
    for (int i = 0; i < entities.size(); ++i) {        
        cout << entities[i].label << " ," <<  entities[i].location << " ," <<  entities[i].type<< " ," <<  entities[i].value << " ," <<  entities[i].ID << endl ;
    }

    cout << "getEntities serialization . . . "<<endl;
    cout << serializeEntities(entities) << endl;*/
    return entities;
}
vector<ProvUtils::Activity> InDiProv::getActivities(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::Activity> activities;
    //WFID, startTime, endTime, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Activity where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        activities.push_back(ProvUtils::Activity{res->getString("startTime"),res->getString("endTime"),res->getString("label"),res->getString("location"),res->getString("type"),std::to_string(res->getInt("ID"))});
    return activities;
}
vector<ProvUtils::Agent> InDiProv::getAgents(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::Agent> agents;
    //WFID, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Agent where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        agents.push_back(ProvUtils::Agent{res->getString("label"),res->getString("location"),res->getString("type"),std::to_string(res->getInt("ID"))});
    return agents;
}
vector<ProvUtils::Used> InDiProv::getUseds(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::Used> useds;
    //WFID, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Usage where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        useds.push_back(ProvUtils::Used{std::to_string(res->getInt("activity")),std::to_string(res->getInt("entity")),res->getString("time"),res->getString("label"),res->getString("location"),res->getString("role"),res->getString("type"),std::to_string(res->getInt("ID"))});
    delete prep_stmt;
    return useds;
}
vector<ProvUtils::WasGeneratedBy> InDiProv::getWasGeneratedBys(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::WasGeneratedBy> wasGeneratedBys;
    //WFID, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Generation where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        wasGeneratedBys.push_back(ProvUtils::WasGeneratedBy{std::to_string(res->getInt("activity")),std::to_string(res->getInt("entity")),res->getString("time"),res->getString("label"),res->getString("location"),res->getString("role"),res->getString("type"),std::to_string(res->getInt("ID"))});
    return wasGeneratedBys;
}
vector<ProvUtils::WasDerivedFrom> InDiProv::getWasDerivedFroms(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::WasDerivedFrom> wasDerivedFroms;
    //WFID, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Derivation where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        wasDerivedFroms.push_back(ProvUtils::WasDerivedFrom{std::to_string(res->getInt("generatedEntity")),std::to_string(res->getInt("usedEntity")),std::to_string(res->getInt("activity")),std::to_string(res->getInt("generation")),std::to_string(res->getInt("usage")),res->getString("label"),res->getString("type"),std::to_string(res->getInt("ID"))});
    return wasDerivedFroms;
}
vector<ProvUtils::WasAttributedTo> InDiProv::getWasAttributedTos(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::WasAttributedTo> wasAttributedTos;
    //WFID, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Attribution where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        wasAttributedTos.push_back(ProvUtils::WasAttributedTo{std::to_string(res->getInt("entity")),std::to_string(res->getInt("agent")),res->getString("label"),res->getString("type"),std::to_string(res->getInt("ID"))});
    return wasAttributedTos;
}
vector<ProvUtils::WasAssociatedWith> InDiProv::getWasAssociatedWiths(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::WasAssociatedWith> wasAssociatedWiths;
    //WFID, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Association where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        wasAssociatedWiths.push_back(ProvUtils::WasAssociatedWith{std::to_string(res->getInt("activity")),std::to_string(res->getInt("agent")),std::to_string(res->getInt("plan")),res->getString("label"),res->getString("role"),res->getString("type"),std::to_string(res->getInt("ID"))});
    return wasAssociatedWiths;
}
vector<ProvUtils::WasInformedBy> InDiProv::getWasInformedBys(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::WasInformedBy> wasInformedBys;
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Communication where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        wasInformedBys.push_back(ProvUtils::WasInformedBy{std::to_string(res->getInt("informed")),std::to_string(res->getInt("informant")), res->getString("label"), res->getString("type"),std::to_string(res->getInt("ID"))});
    return wasInformedBys;
}
vector<ProvUtils::WasStartedBy> InDiProv::getwasStartedBys(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::WasStartedBy> wasStartedBys;
    //WFID, label, location, type
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Start where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        wasStartedBys.push_back(ProvUtils::WasStartedBy{std::to_string(res->getInt("activity")),std::to_string(res->getInt("trigger")),std::to_string(res->getInt("starter")),res->getString("time"),res->getString("label"), res->getString("location"), res->getString("role") ,res->getString("type"),std::to_string(res->getInt("ID"))});
    return wasStartedBys;
}
vector<ProvUtils::WasEndedBy> InDiProv::getwasEndedBys(int wfid){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    vector<ProvUtils::WasEndedBy> wasEndedBys;
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.End where WFID=?");
    prep_stmt->setInt(1, wfid);
    res = prep_stmt->executeQuery();
    while (res->next())
        wasEndedBys.push_back(ProvUtils::WasEndedBy{std::to_string(res->getInt("activity")),std::to_string(res->getInt("trigger")),std::to_string(res->getInt("ender")),res->getString("time"),res->getString("label"), res->getString("location"), res->getString("role") ,res->getString("type"),std::to_string(res->getInt("ID"))});
    return wasEndedBys;

}

//WF functions
int InDiProv::createWF(string WFName, string password){
    int lastID=-1;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("INSERT INTO inDiProv.Workflows (WFName, WFPass) VALUES (?,?)");

    prep_stmt->setString(1, WFName);
    prep_stmt->setString(2, password);
    prep_stmt->execute();
    prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID() as ID");
    res = prep_stmt->executeQuery();
    while (res->next())
        lastID=res->getInt("ID");
    delete prep_stmt;
    return lastID;
}
int InDiProv::checkPass(int WFid , string password){

    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = con->prepareStatement("SELECT * FROM inDiProv.Workflows where Workflows.WFID=?");
    prep_stmt->setInt(1, WFid);
    res = prep_stmt->executeQuery();
    while (res->next())
        if (password.compare(res->getString("WFPass"))==0){
            delete prep_stmt;
            return WFid;
        }
    delete prep_stmt;
    return -1;
}
string InDiProv::getCurrentTime(){
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 100;
    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));
    char currentTime[84] = "";
    sprintf(currentTime, "%s.%d", buffer, milli);
    return string(currentTime);
}
string InDiProv::queryParsing(char *tmpMsg){
    json_object * jobj = json_tokener_parse(tmpMsg);
    cout << tmpMsg << endl;
    int WFID;
    string WFPass;
    string Command;
    enum json_type type;
    vector <string> commandVec;
    json_object_object_foreach(jobj, key, val) {
        type = json_object_get_type(val);
        //cout<<"type: "<<type<<endl;
        if (strcmp(key, "Command") == 0)
            //commandVec.push_back(json_object_get_string(val));
            Command=json_object_get_string(val);
        else if (strcmp(key, "WFID") == 0){
            WFID=json_object_get_int(val);
        }
        else if (strcmp(key, "WFPass") == 0){
            WFPass =json_object_get_string(val);
        }
        else if (strcmp(key, "Arguments") == 0){
            //void json_parse(json_object * jobj); /*Forward Declaration*/
            enum json_type type;
            json_object *jarray = json_object_object_get(jobj, key);

            int arraylen = json_object_array_length(jarray);
            //cout<<"Array Length: "<< arraylen << endl;
            for (int i=0; i< arraylen; i++){
                commandVec.push_back(json_object_get_string(json_object_array_get_idx(jarray, i)));
            }
        }
    }    
    if (checkPass(WFID,WFPass) == WFID){
        //cout << "prov function recieved, Command: " << Command << " , NO. arguments: "<< commandVec.size()<<endl ;

        if (Command.compare("setEntity")==0 && commandVec.size()==4)
            return std::to_string(setEntity(WFID,commandVec[0],commandVec[1],commandVec[2],commandVec[3]));
        else if (Command.compare("setActivity")==0 && commandVec.size()==5)
            return std::to_string(setActivity(WFID,commandVec[0],commandVec[1],commandVec[2],commandVec[3],commandVec[4]));
        else if (Command.compare("setAgent")==0 && commandVec.size()==3)
            return std::to_string(setAgent(WFID,commandVec[0],commandVec[1],commandVec[2]));
        else if (Command.compare("setused")==0 && commandVec.size()==7)
            return std::to_string(setused(WFID,atoi(commandVec[0].c_str()),atoi(commandVec[1].c_str()),commandVec[2],commandVec[3],commandVec[4],commandVec[5],commandVec[6]));
        else if (Command.compare("setwasGeneratedBy")==0 && commandVec.size()==7)
            return std::to_string(setwasGeneratedBy(WFID,atoi(commandVec[0].c_str()) ,atoi(commandVec[1].c_str()) ,commandVec[2],commandVec[3],commandVec[4],commandVec[5],commandVec[6]));
        else if (Command.compare("setwasDerivedFrom")==0 && commandVec.size()==7)
            return std::to_string(setwasDerivedFrom(WFID,atoi(commandVec[0].c_str()) ,atoi(commandVec[1].c_str()) ,atoi(commandVec[2].c_str()) ,atoi(commandVec[3].c_str()) ,atoi(commandVec[4].c_str()) ,commandVec[5],commandVec[6]));
        else if (Command.compare("setwasAttributedTo")==0 && commandVec.size()==4)
            return std::to_string(setwasAttributedTo(WFID,atoi(commandVec[0].c_str()) ,atoi(commandVec[1].c_str()) ,commandVec[2],commandVec[3]));
        else if (Command.compare("setwasAssociatedWith")==0 && commandVec.size()==6)
            return std::to_string(setwasAssociatedWith(WFID,atoi(commandVec[0].c_str()) ,atoi(commandVec[1].c_str()) ,atoi(commandVec[2].c_str()) ,commandVec[3],commandVec[4],commandVec[5]));
        else if (Command.compare("setwasInformedBy")==0 && commandVec.size()==4)
            return std::to_string(setwasInformedBy(WFID,atoi(commandVec[0].c_str()) ,atoi(commandVec[1].c_str()) ,commandVec[2],commandVec[3]));
        else if (Command.compare("setwasStartedBy")==0 && commandVec.size()==8)
            return std::to_string(setwasStartedBy(WFID,atoi(commandVec[0].c_str()) ,atoi(commandVec[1].c_str()) ,atoi(commandVec[2].c_str()) ,commandVec[3],commandVec[4],commandVec[5],commandVec[6],commandVec[7]));
        else if (Command.compare("setwasEndedBy")==0 && commandVec.size()==8)
            return std::to_string(setwasEndedBy(WFID,atoi(commandVec[0].c_str()) ,atoi(commandVec[1].c_str()) ,atoi(commandVec[2].c_str()) ,commandVec[3],commandVec[4],commandVec[5],commandVec[6],commandVec[7]));
        else if (Command.compare("getEntities")==0)
            return serializeEntities(getEntities(WFID));
        else if (Command.compare("getActivities")==0)
            return serializeActivities(getActivities(WFID));
        else if (Command.compare("getAgents")==0)
            return serializeAgents(getAgents(WFID));
        else if (Command.compare("getUseds")==0)
            return serializeUseds(getUseds(WFID));
        else if (Command.compare("getWasGeneratedBys")==0)
            return serializeWasGeneratedBys(getWasGeneratedBys(WFID));
        else if (Command.compare("getWasDerivedFroms")==0)
            return serializeWasDerivedFroms(getWasDerivedFroms(WFID));
        else if (Command.compare("getWasAttributedTos")==0)
            return serializeWasAttributedTos(getWasAttributedTos(WFID));
        else if (Command.compare("getWasAssociatedWiths")==0)
            return serializeWasAssociatedWiths(getWasAssociatedWiths(WFID));
        else if (Command.compare("getwasInformedBys")==0)
            return serializeWasInformedBys(getWasInformedBys(WFID));
        else if (Command.compare("getwasStartedBys")==0)
            return serializeWasStartedBys(getwasStartedBys(WFID));
        else if (Command.compare("getwasEndedBys")==0)
            return serializeWasEndedBys(getwasEndedBys(WFID));
        else if (Command.compare("exportPROV")==0)
            return std::to_string(exportPROV(WFID));

    }
    if (Command.compare("createWF")==0)
        return std::to_string(createWF(commandVec[0], commandVec[1]));
    if (Command.compare("loadWF")==0)
        return std::to_string(checkPass(atoi(commandVec[0].c_str()),commandVec[1]));
    return "-1";
}

int InDiProv::exportPROV(int wfid){
    PROVC* provhandeler = new PROVC();
    vector<ProvUtils::Entity> entities = getEntities(wfid);
    vector<ProvUtils::Agent> agents = getAgents(wfid);
    vector<ProvUtils::Activity> activities = getActivities(wfid);
    vector<ProvUtils::Used> useds = getUseds(wfid);
    vector<ProvUtils::WasGeneratedBy> wasGeneratedBys = getWasGeneratedBys(wfid);
    //vector<ProvUtils::WasDerivedFrom> wasDerivedFroms = getWasDerivedFroms(wfid);
    //vector<ProvUtils::WasAttributedTo> wasAttributedTos = getWasAttributedTos(wfid);
    //vector<ProvUtils::WasAssociatedWith> wasAssociatedWiths = getWasAssociatedWiths(wfid);
    provhandeler->setEntities(&entities);
    provhandeler->setAgents(&agents);
    provhandeler->setActivities(&activities);
    provhandeler->setUseds(&useds);
    provhandeler->setwasGeneratedBys(&wasGeneratedBys);
    //provhandeler->setwasDerivedFroms(&wasDerivedFroms);
    //provhandeler->setwasAttributedTos(&wasAttributedTos);
    //provhandeler->setwasAssociatedWiths(&wasAssociatedWiths);
    provhandeler->writeProv();
    return 1;
}

//Serialization

string InDiProv::serializeEntities(vector<ProvUtils::Entity> entities){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < entities.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();

        json_object *labelstr = json_object_new_string(entities[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *locationstr = json_object_new_string(entities[i].location.c_str());
        json_object_object_add(tmpjobj,"location", locationstr);

        json_object *typestr = json_object_new_string(entities[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *valuestr = json_object_new_string(entities[i].value.c_str());
        json_object_object_add(tmpjobj,"value", valuestr);

        json_object *IDstr = json_object_new_string(entities[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"entities", jarray);
    return json_object_to_json_string(jobj);
}

string InDiProv::serializeActivities(vector<ProvUtils::Activity> activities){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < activities.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *startTimestr = json_object_new_string(activities[i].startTime.c_str());
        json_object_object_add(tmpjobj,"startTime", startTimestr);

        json_object *endTimestr = json_object_new_string(activities[i].endTime.c_str());
        json_object_object_add(tmpjobj,"endTime", endTimestr);

        json_object *labelstr = json_object_new_string(activities[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *locationstr = json_object_new_string(activities[i].location.c_str());
        json_object_object_add(tmpjobj,"location", locationstr);

        json_object *typestr = json_object_new_string(activities[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(activities[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"activities", jarray);
    return json_object_to_json_string(jobj);
}

string InDiProv::serializeAgents(vector<ProvUtils::Agent> agents){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < agents.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();

        json_object *labelstr = json_object_new_string(agents[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *locationstr = json_object_new_string(agents[i].location.c_str());
        json_object_object_add(tmpjobj,"location", locationstr);

        json_object *typestr = json_object_new_string(agents[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(agents[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"agents", jarray);
    return json_object_to_json_string(jobj);
}

string InDiProv::serializeUseds(vector<ProvUtils::Used> useds){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < useds.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *actIDstr = json_object_new_string(useds[i].actID.c_str());
        json_object_object_add(tmpjobj,"actID", actIDstr);

        json_object *entIDstr = json_object_new_string(useds[i].entID.c_str());
        json_object_object_add(tmpjobj,"entID", entIDstr);

        json_object *usedTimestr = json_object_new_string(useds[i].usedTime.c_str());
        json_object_object_add(tmpjobj,"usedTime", usedTimestr);

        json_object *labelstr = json_object_new_string(useds[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *locationstr = json_object_new_string(useds[i].location.c_str());
        json_object_object_add(tmpjobj,"location", locationstr);

        json_object *rolestr = json_object_new_string(useds[i].role.c_str());
        json_object_object_add(tmpjobj,"role", rolestr);

        json_object *typestr = json_object_new_string(useds[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(useds[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"useds", jarray);
    return json_object_to_json_string(jobj);

}

string InDiProv::serializeWasGeneratedBys(vector<ProvUtils::WasGeneratedBy> wasGeneratedBys){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < wasGeneratedBys.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *actIDstr = json_object_new_string(wasGeneratedBys[i].actID.c_str());
        json_object_object_add(tmpjobj,"actID", actIDstr);

        json_object *entIDstr = json_object_new_string(wasGeneratedBys[i].entID.c_str());
        json_object_object_add(tmpjobj,"entID", entIDstr);

        json_object *generateTimestr = json_object_new_string(wasGeneratedBys[i].generateTime.c_str());
        json_object_object_add(tmpjobj,"generateTime", generateTimestr);

        json_object *labelstr = json_object_new_string(wasGeneratedBys[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *locationstr = json_object_new_string(wasGeneratedBys[i].location.c_str());
        json_object_object_add(tmpjobj,"location", locationstr);

        json_object *rolestr = json_object_new_string(wasGeneratedBys[i].role.c_str());
        json_object_object_add(tmpjobj,"role", rolestr);

        json_object *typestr = json_object_new_string(wasGeneratedBys[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(wasGeneratedBys[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"wasGeneratedBys", jarray);
    return json_object_to_json_string(jobj);
}

string InDiProv::serializeWasDerivedFroms(vector<ProvUtils::WasDerivedFrom> wasDerivedFroms){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < wasDerivedFroms.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *genEntIDstr = json_object_new_string(wasDerivedFroms[i].genEntID.c_str());
        json_object_object_add(tmpjobj,"genEntID", genEntIDstr);

        json_object *usdEntIDstr = json_object_new_string(wasDerivedFroms[i].usdEntID.c_str());
        json_object_object_add(tmpjobj,"usdEntID", usdEntIDstr);

        json_object *actIDstr = json_object_new_string(wasDerivedFroms[i].actID.c_str());
        json_object_object_add(tmpjobj,"actID", actIDstr);

        json_object *genIDstr = json_object_new_string(wasDerivedFroms[i].genID.c_str());
        json_object_object_add(tmpjobj,"genID", genIDstr);

        json_object *usgIDstr = json_object_new_string(wasDerivedFroms[i].usgID.c_str());
        json_object_object_add(tmpjobj,"usgID", usgIDstr);

        json_object *labelstr = json_object_new_string(wasDerivedFroms[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *typestr = json_object_new_string(wasDerivedFroms[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(wasDerivedFroms[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"wasDerivedFroms", jarray);
    return json_object_to_json_string(jobj);
}

string InDiProv::serializeWasAttributedTos(vector<ProvUtils::WasAttributedTo> wasAttributedTos){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < wasAttributedTos.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *entIDstr = json_object_new_string(wasAttributedTos[i].entID.c_str());
        json_object_object_add(tmpjobj,"entID", entIDstr);

        json_object *agentIDstr = json_object_new_string(wasAttributedTos[i].agentID.c_str());
        json_object_object_add(tmpjobj,"agentID", agentIDstr);

        json_object *labelstr = json_object_new_string(wasAttributedTos[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *typestr = json_object_new_string(wasAttributedTos[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(wasAttributedTos[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"wasAttributedTos", jarray);
    return json_object_to_json_string(jobj);

}

string InDiProv::serializeWasAssociatedWiths(vector<ProvUtils::WasAssociatedWith> wasAssociatedWiths){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < wasAssociatedWiths.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *actIDstr = json_object_new_string(wasAssociatedWiths[i].actID.c_str());
        json_object_object_add(tmpjobj,"actID", actIDstr);

        json_object *agentIDstr = json_object_new_string(wasAssociatedWiths[i].agentID.c_str());
        json_object_object_add(tmpjobj,"agentID", agentIDstr);

        json_object *planIDstr = json_object_new_string(wasAssociatedWiths[i].planID.c_str());
        json_object_object_add(tmpjobj,"planID", planIDstr);

        json_object *labelstr = json_object_new_string(wasAssociatedWiths[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *rolestr = json_object_new_string(wasAssociatedWiths[i].role.c_str());
        json_object_object_add(tmpjobj,"role", rolestr);

        json_object *typestr = json_object_new_string(wasAssociatedWiths[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(wasAssociatedWiths[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"wasAssociatedWiths", jarray);
    return json_object_to_json_string(jobj);
}

string InDiProv::serializeWasInformedBys(vector<ProvUtils::WasInformedBy> wasInformedBys){
    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < wasInformedBys.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *informedstr = json_object_new_string(wasInformedBys[i].informed.c_str());
        json_object_object_add(tmpjobj,"informed", informedstr);

        json_object *informantstr = json_object_new_string(wasInformedBys[i].informant.c_str());
        json_object_object_add(tmpjobj,"informant", informantstr);

        json_object *labelstr = json_object_new_string(wasInformedBys[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *typestr = json_object_new_string(wasInformedBys[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(wasInformedBys[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"wasInformedBys", jarray);
    return json_object_to_json_string(jobj);
}
string InDiProv::serializeWasStartedBys(vector<ProvUtils::WasStartedBy> wasStartedBys){

    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < wasStartedBys.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *actIDstr = json_object_new_string(wasStartedBys[i].actID.c_str());
        json_object_object_add(tmpjobj,"actID", actIDstr);

        json_object *entIDstr = json_object_new_string(wasStartedBys[i].entID.c_str());
        json_object_object_add(tmpjobj,"entID", entIDstr);

        json_object *starterActIDstr = json_object_new_string(wasStartedBys[i].starterActID.c_str());
        json_object_object_add(tmpjobj,"starterActID", starterActIDstr);

        json_object *sTimestr = json_object_new_string(wasStartedBys[i].sTime.c_str());
        json_object_object_add(tmpjobj,"sTime", sTimestr);

        json_object *labelstr = json_object_new_string(wasStartedBys[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *locationstr = json_object_new_string(wasStartedBys[i].location.c_str());
        json_object_object_add(tmpjobj,"location", locationstr);

        json_object *rolestr = json_object_new_string(wasStartedBys[i].role.c_str());
        json_object_object_add(tmpjobj,"role", rolestr);

        json_object *typestr = json_object_new_string(wasStartedBys[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(wasStartedBys[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"wasStartedBys", jarray);
    return json_object_to_json_string(jobj);
}

string InDiProv::serializeWasEndedBys(vector<ProvUtils::WasEndedBy> wasEndedBys){

    json_object * jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < wasEndedBys.size(); ++i) {
        json_object * tmpjobj = json_object_new_object();


        json_object *actIDstr = json_object_new_string(wasEndedBys[i].actID.c_str());
        json_object_object_add(tmpjobj,"actID", actIDstr);

        json_object *entIDstr = json_object_new_string(wasEndedBys[i].entID.c_str());
        json_object_object_add(tmpjobj,"entID", entIDstr);

        json_object *enderActIDstr = json_object_new_string(wasEndedBys[i].enderActID.c_str());
        json_object_object_add(tmpjobj,"enderActID", enderActIDstr);

        json_object *eTimestr = json_object_new_string(wasEndedBys[i].eTime.c_str());
        json_object_object_add(tmpjobj,"eTime", eTimestr);

        json_object *labelstr = json_object_new_string(wasEndedBys[i].label.c_str());
        json_object_object_add(tmpjobj,"label", labelstr);

        json_object *locationstr = json_object_new_string(wasEndedBys[i].location.c_str());
        json_object_object_add(tmpjobj,"location", locationstr);

        json_object *rolestr = json_object_new_string(wasEndedBys[i].role.c_str());
        json_object_object_add(tmpjobj,"role", rolestr);

        json_object *typestr = json_object_new_string(wasEndedBys[i].type.c_str());
        json_object_object_add(tmpjobj,"type", typestr);

        json_object *IDstr = json_object_new_string(wasEndedBys[i].ID.c_str());
        json_object_object_add(tmpjobj,"ID", IDstr);

        json_object_array_add(jarray,tmpjobj);
    }
    json_object_object_add(jobj,"wasEndedBys", jarray);
    return json_object_to_json_string(jobj);
}
