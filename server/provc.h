#ifndef PROVC_H
#define PROVC_H
#include <memory>
#include "prov-core.hxx"
#include "provutils.h"
#include <string>
#include <fstream>

using namespace std;
class PROVC
{
public:
    PROVC();
    ~PROVC();
    void writeProv();
    // Component 1 elements -- complete

    void setEntities();
    void setEntities(vector<ProvUtils::Entity>* exEntities);
    void setEntity(string label, string location, string type, string val, string ID);

    void setActivities();
    void setActivities(vector<ProvUtils::Activity>* exactivities);
    void setActivity(prov::Activity::startTime_type startTime, prov::Activity::startTime_type endTime, string label, string location, string type, string ID);

    void setwasGeneratedBys(); // correct time    
    void setwasGeneratedBys(vector<ProvUtils::WasGeneratedBy>* exWasGeneratedBys);
    void setwasGeneratedBy(string actID,string entID,prov::Usage::time_type usedTime,string label, string location, string role, string type, string ID);

    void setUseds();
    void setUseds(vector<ProvUtils::Used>* exUseds);
    void setused(string actID,string entID,prov::Usage::time_type usedTime,string label, string location, string role, string type, string ID);

    void setwasInformedBys();
    void setwasInformedBy(string informedID,string informantID,string label, string type, string ID);
    void setwasStartedBys();
    void setwasStartedBy(string actID,string trigID,string starterID,prov::Start::time_type startTime ,string label, string location, string role, string type, string ID);
    void setwasEndedBys();
    void setwasEndedBy(string actID,string trigID,string enderID,prov::End::time_type endTime ,string label, string location, string role, string type, string ID);
    void setwasInvalidatedBys();
    void setwasInvalidatedBy(string actID,string entID,prov::Invalidation::time_type invalidTime,string label, string location, string role, string type, string ID);

    // Component 2 elements -- complete
    void setwasDerivedFroms();
    void setwasDerivedFroms(vector<ProvUtils>* exwasDerivedFroms);
    void setwasDerivedFrom(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID);

    void setwasRevisionOfs();
    void setwasRevisionOf(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID);
    void setwasQuotedFroms();
    void setwasQuotedFrom(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID);
    void sethadPrimarySources();
    void sethadPrimarySource(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID);
    // Component 3 elements
    void setAgents();
    void setAgents(vector<ProvUtils::Agent>* exAgents);
    void setAgent(string label, string location, string type, string ID);

    void setpersons();
    void setperson(string label, string location, string type, string ID);
    void setorganizations();
    void setorganization(string label, string location, string type, string ID);
    void setsoftwareAgents();
    void setsoftwareAgent(string label, string location, string type, string ID);

    void setwasAttributedTos();
    void setwasAttributedTos(vector<ProvUtils::WasAttributedTo>* exwasAttributedTos);
    void setwasAttributedTo(string entID,string agentID,string label, string type, string ID);

    void setwasAssociatedWiths();
    void setwasAssociatedWiths(vector<ProvUtils::WasAssociatedWith>* wasAssociatedWiths);
    void setwasAssociatedWith(string actID,string agentID,string planID,string label, string role, string type, string ID);

    void setactedOnBehalfOfs();
    void setactedOnBehalfOf(string delegateID,string responsibleID,string actID, string label, string type, string ID);
    void setwasInfluencedBys();
    void setwasInfluencedBy(string influenceeID,string influencerID,string label, string type, string ID);

    // Component 4 elements

    // Component 5 elements
    void setspecializationOfs();
    void setspecializationOf(string spcEnt , string genEnt);
    void setalternateOfs();
    void setalternateOf(string alt1 , string alt2);

    // component 6
    void setcollections();
    void setemptyCollections();
    void sethadMembers();

    // component 7
    void setplans();

private:
    prov::Document provdoc;
    prov::Document::entity_sequence& entityseqvar;
    prov::Document::activity_sequence& activityseqvar ;
    prov::Document::agent_sequence& agentseqvar;
    prov::Document::used_sequence& usedseqvar;
    prov::Document::wasGeneratedBy_sequence& generatedbyseqvar;
};

#endif // PROVC_H
