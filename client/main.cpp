#include <stdio.h>
#include<iostream>
#include "indiprovclient.h"
#include <unistd.h>
#include <fstream>
using namespace std;
int main()
{
    InDiProvClient* provclient = new InDiProvClient();
    string wfname="WFName1";
    string wfpass = "password";
    //int wfid = provclient->createWF(wfname ,wfpass);
    //if (wfid != -1){
    int wfid = 2;
    //cout << "Workflow created sucessfully with ID: "<< wfid << endl;
    if (provclient->loadWF(wfid , wfpass)==wfid)
        cout << "Workflow loaded sucessfully with ID: "<< wfid << endl;


        int entityID= provclient->setEntity("myLabel","myLocation","myType","myVal");
        if (entityID != -1)
            cout << "Entity created sucessfully with ID: "<< entityID << endl;

        int activityID= provclient->setActivity(provclient->getCurrentTime(),provclient->getCurrentTime(),"myActivityLabel","myActivityLocation","myActivityType");
        if (activityID != -1)
            cout << "Activity created sucessfully with ID: "<< activityID << endl;

        int agentID= provclient->setAgent("myAgentLabel","myAgentLocation","myAgentType");
        if (agentID != -1)
            cout << "Agent created sucessfully with ID: "<< agentID << endl;

        int usedID= provclient->setused(activityID,entityID,provclient->getCurrentTime(),"myUsedLabel","myUsedLocation", "myUsedRole","myUsedType");
        if (usedID != -1)
            cout << "Used created sucessfully with ID: "<< usedID << endl;

        int wasGeneratedByID= provclient->setwasGeneratedBy(entityID, activityID,provclient->getCurrentTime(),"mywasGeneratedByLabel","mywasGeneratedByLocation", "mywasGeneratedByRole","mywasGeneratedByType");
        if (wasGeneratedByID != -1)
            cout << "wasGeneratedBy created sucessfully with ID: "<< wasGeneratedByID << endl;

        int wasDerivedFromID = provclient->setwasDerivedFrom(entityID,entityID,activityID,wasGeneratedByID,usedID,"mywasDerivedFromLabel","mywasDerivedFromType");
        if (wasDerivedFromID != -1)
            cout << "wasDerivedFrom created sucessfully with ID: "<< wasDerivedFromID << endl;

        int wasAttributedToID= provclient->setwasAttributedTo(entityID,agentID,"mysetwasAttributedToLabel","mysetwasAttributedToType");
        if (wasAttributedToID != -1)
            cout << "wasAttributedTo created sucessfully with ID: "<< wasAttributedToID << endl;

        int wasAssociatedWithID= provclient->setwasAssociatedWith(activityID,agentID,-1,"mywasAssociatedWithLabel", "mywasAssociatedWithRole","mywasAssociatedWithType");
        if (wasAssociatedWithID != -1)
            cout << "wasAssociatedWith created sucessfully with ID: "<< wasAssociatedWithID << endl;

        int wasStartedByID= provclient->setwasStartedBy(activityID,entityID,entityID,provclient->getCurrentTime(), "mywasStartedByLabel", "mywasStartedByLocation", "mywasStartedByRole","mywasStartedByType");
        if (wasStartedByID != -1)
            cout << "wasStartedBy created sucessfully with ID: "<< wasStartedByID << endl;

        int wasEndedByID= provclient->setwasEndedBy(activityID,entityID,entityID,provclient->getCurrentTime(), "mywasEndedByLabel", "mywasEndedByLocation", "mywasEndedByRole","mywasEndedByType");
        if (wasEndedByID != -1)
            cout << "wasEndedBycreated sucessfully with ID: "<< wasEndedByID << endl;

        int wasInformedByID= provclient->setwasInformedBy(activityID,activityID, "wasInformedByLabel","wasInformedByType");
        if (wasInformedByID != -1)
            cout << "wasInformedBy created sucessfully with ID: "<< wasInformedByID << endl;
        
        // get entities
        string EntitiesJSON= provclient->getEntities();
        cout << "Entities loaded sucessfully: "<< endl;
        char *cstr = new char[EntitiesJSON.length() + 1];
        strcpy(cstr, EntitiesJSON.c_str());
        vector <ProvUtils::Entity> entities = provclient->deSerializeEntities(cstr);
        delete [] cstr;

        for (int i = 0; i < entities.size(); ++i) {
            cout << entities[i].label << " ," <<  entities[i].location << " ," <<  entities[i].type<< " ," <<  entities[i].value << " ," <<  entities[i].ID << endl ;
        }
        //get activities
        string ActivitiesJSON= provclient->getActivities();
        cout << "Activities loaded sucessfully, total: "<< endl;

        char *cstr2 = new char[ActivitiesJSON.length() + 1];
        strcpy(cstr2, ActivitiesJSON.c_str());
        vector <ProvUtils::Activity> activities = provclient->deSerializeActivities(cstr2);
        for (int i = 0; i < activities.size(); ++i) {
            cout << activities[i].startTime << " ," << activities[i].endTime << " ," <<activities[i].label << " ," <<  activities[i].location << " ," <<  activities[i].type<< " ," <<  activities[i].ID << endl ;
        }
        delete [] cstr2;
        //get agents
        string AgentsJSON= provclient->getAgents();
        cout << "Agents loaded sucessfully: "<< endl;
        char *cstr3 = new char[AgentsJSON.length() + 1];
        strcpy(cstr3, AgentsJSON.c_str());
        vector <ProvUtils::Agent> agents = provclient->deSerializeAgents(cstr3);
        delete [] cstr3;

        for (int i = 0; i < agents.size(); ++i) {
            cout << agents[i].label << " ," <<  agents[i].location << " ," <<  agents[i].type<< " ," <<  agents[i].ID << endl ;
        }


        //get useds
        string UsedsJSON= provclient->getUseds();
        cout << "Useds loaded sucessfully: "<< endl;
        char *cstr4 = new char[UsedsJSON.length() + 1];
        strcpy(cstr4, UsedsJSON.c_str());
        vector <ProvUtils::Used> useds= provclient->deSerializeUseds(cstr4);
        delete [] cstr4;

        for (int i = 0; i < useds.size(); ++i) {
            cout << useds[i].actID << " ," <<  useds[i].entID << " ," <<  useds[i].usedTime << " ," <<  useds[i].label << " ," <<  useds[i].location << " ," <<  useds[i].role << " ," <<  useds[i].type << " ," <<  useds[i].ID << endl;
        }

        //get WasGeneratedBys
        string WasGeneratedBysJSON= provclient->getWasGeneratedBys();
        cout << "GeneratedBys loaded sucessfully: "<< endl;
        char *cstr5 = new char[WasGeneratedBysJSON.length() + 1];
        strcpy(cstr5, WasGeneratedBysJSON.c_str());
        vector <ProvUtils::WasGeneratedBy> wasGeneratedBys= provclient->deSerializeWasGeneratedBys(cstr5);
        delete [] cstr5;

        for (int i = 0; i < wasGeneratedBys.size(); ++i) {
            cout << wasGeneratedBys[i].actID << " ," << wasGeneratedBys[i].entID << " ," << wasGeneratedBys[i].actID << " ," << wasGeneratedBys[i].generateTime << " ," << wasGeneratedBys[i].label << " ," << wasGeneratedBys[i].location << " ," << wasGeneratedBys[i].role << " ," << wasGeneratedBys[i].type << " ," << wasGeneratedBys[i].ID << endl;
        }

        //get WasDerivedFrom
        string WasDerivedFromsJSON= provclient->getWasDerivedFroms();
        cout << "WasDerivedFrom loaded sucessfully: "<< endl;
        char *cstr6 = new char[WasDerivedFromsJSON.length() + 1];
        strcpy(cstr6, WasDerivedFromsJSON.c_str());
        vector <ProvUtils::WasDerivedFrom> WasDerivedFroms= provclient->deSerializeWasDerivedFroms(cstr6);
        delete [] cstr6;

        for (int i = 0; i < WasDerivedFroms.size(); ++i) {
            cout << WasDerivedFroms[i].genEntID << " ," << WasDerivedFroms[i].usdEntID << " ," <<WasDerivedFroms[i].actID << " ," <<WasDerivedFroms[i].genID << " ," <<WasDerivedFroms[i].usgID << " ," <<WasDerivedFroms[i].label << " ," <<WasDerivedFroms[i].type << " ," <<WasDerivedFroms[i].ID << endl;
        }

        //get WasAttributedTo
        string WasAttributedTosJSON= provclient->getWasAttributedTos();
        cout << "WasAttributedTos loaded sucessfully: "<<  endl;
        char *cstr7 = new char[WasAttributedTosJSON.length() + 1];
        strcpy(cstr7, WasAttributedTosJSON.c_str());
        vector <ProvUtils::WasAttributedTo> WasAttributedTos= provclient->deSerializeWasAttributedTos(cstr7);
        delete [] cstr7;

        for (int i = 0; i < WasAttributedTos.size(); ++i) {
            cout << WasAttributedTos[i].entID << " ," << WasAttributedTos[i].agentID << " ," <<WasAttributedTos[i].label << " ," <<WasAttributedTos[i].type << " ," <<WasAttributedTos[i].ID << endl ;
        }

        //get WasAssociatedWiths
        string WasAssociatedWithsJSON= provclient->getWasAssociatedWiths();
        cout << "WasAssociatedWiths loaded sucessfully: "<<  WasAssociatedWithsJSON << endl;
        char *cstr8 = new char[WasAssociatedWithsJSON.length() + 1];
        strcpy(cstr8, WasAssociatedWithsJSON.c_str());
        vector <ProvUtils::WasAssociatedWith> WasAssociatedWiths= provclient->deSerializeWasAssociatedWiths(cstr8);
        delete [] cstr8;

        for (int i = 0; i < WasAssociatedWiths.size(); ++i) {
            cout << WasAssociatedWiths[i].actID << " ," <<  WasAssociatedWiths[i].agentID << " ," <<WasAssociatedWiths[i].planID << " ," <<WasAssociatedWiths[i].label << " ," <<WasAssociatedWiths[i].role << " ," <<WasAssociatedWiths[i].type << " ," <<WasAssociatedWiths[i].ID << endl ;
        }

        //get WasStartedBys
        string WasStartedBysJSON= provclient->getwasStartedBys();
        cout << "WasStartedBys loaded sucessfully: "<<  WasStartedBysJSON << endl;
        char *cstr9 = new char[WasStartedBysJSON.length() + 1];
        strcpy(cstr9, WasStartedBysJSON.c_str());
        vector <ProvUtils::WasStartedBy> wasStartedBys= provclient->deSerializeWasStartedBys(cstr9);
        delete [] cstr9;

        for (int i = 0; i < wasStartedBys.size(); ++i) {
            cout << wasStartedBys[i].actID << " ," <<  wasStartedBys[i].entID << " ," <<  wasStartedBys[i].starterActID << " ," <<  wasStartedBys[i].sTime << " ," <<  wasStartedBys[i].label << " ," <<  wasStartedBys[i].location << " ," <<  wasStartedBys[i].role << " ," <<  wasStartedBys[i].type << " ," <<  wasStartedBys[i].ID << endl ;
        }
        
        //get WasInformedBys
        string WasInformedBysJSON= provclient->getwasInformedBys();
        cout << "WasInformedBys loaded sucessfully: "<<  WasInformedBysJSON << endl;
        char *cstr10 = new char[WasInformedBysJSON.length() + 1];
        strcpy(cstr10, WasInformedBysJSON.c_str());
        vector <ProvUtils::WasInformedBy> wasInformedBys= provclient->deSerializeWasInformedBys(cstr10);
        delete [] cstr10;

        for (int i = 0; i < wasInformedBys.size(); ++i) {
            cout << wasInformedBys[i].informed << " ," << wasInformedBys[i].informant << " ," << wasInformedBys[i].label << " ," << wasInformedBys[i].type << endl;
        }
        
        //cout << "To XML serialization . . . "<<provclient->exportPROV() << endl;

return 1;
}
