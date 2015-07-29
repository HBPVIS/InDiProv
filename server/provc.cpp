#include "provc.h"
#include <iostream>
#include <memory>
#include "prov-core.hxx"
#include <string>
#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "provutils.h"

PROVC::PROVC():
    entityseqvar(provdoc.entity()),
    activityseqvar(provdoc.activity()),
    agentseqvar(provdoc.agent()),
    usedseqvar(provdoc.used()),
    generatedbyseqvar(provdoc.wasGeneratedBy())
{


}

PROVC::~PROVC()
{


}
void PROVC::setEntities(){
    this->setEntity("Label","Location","type","values","000");
}

void PROVC::setEntities(vector<ProvUtils::Entity>* exEntities){
    for (int i = 0; i < exEntities->size(); i++) {
        this->setEntity((*exEntities)[i].label,(*exEntities)[i].location, (*exEntities)[i].type, (*exEntities)[i].value, (*exEntities)[i].ID);
    }

}
void PROVC::setEntity(string label, string location, string type, string val, string ID){
    //xsd cxx-tree --generate-serialization --root-element-all --namespace-map http://www.w3.org/ns/prov#=prov ../prov-core.xsd
    //prov::Document provdoc;
    //prov::Document::entity_sequence& entityseqvar (provdoc.entity());
    //create entity
    prov::Entity tmpent;
    //set label
    prov::Entity::label_sequence& ls (tmpent.label());
    ls.push_back(label);
    //location
    prov::Entity::location_sequence& loqseq (tmpent.location());
    //loqseq.push_back("/home/myfile.txt");
    loqseq.push_back(location);
    // type
    prov::Entity::type_sequence& typseq (tmpent.type());
    typseq.push_back(type);
    //value
    prov::Entity::value_type valtype(val);
    tmpent.value (valtype);
    //set ID
    prov::Entity::id_type myidytpe(ID);
    tmpent.id (myidytpe);
    //
    entityseqvar.push_back(tmpent);
}

void PROVC::setActivities(){

    prov::Activity::startTime_type tmpstarttime(2015,4,27,17,30,3.1234);
    prov::Activity::startTime_type tmpendtime(2015,4,27,17,30,1.1234);
    setActivity(tmpstarttime, tmpendtime, "Label", "Location", "Type", "100");
}
void PROVC::setActivities(vector<ProvUtils::Activity>* exactivities){
    for (int i = 0; i < exactivities->size(); i++) {
        boost::posix_time::ptime t1(boost::posix_time::time_from_string((*exactivities)[i].startTime));
        prov::Activity::startTime_type tmpstarttime(t1.date().year(),t1.date().month(),t1.date().day(),t1.time_of_day().hours(),t1.time_of_day().minutes(),t1.time_of_day().seconds());
        boost::posix_time::ptime t2(boost::posix_time::time_from_string((*exactivities)[i].endTime));
        prov::Activity::startTime_type tmpendtime(t2.date().year(),t2.date().month(),t2.date().day(),t2.time_of_day().hours(),t2.time_of_day().minutes(),t2.time_of_day().seconds());
        this->setActivity(tmpstarttime,tmpendtime,(*exactivities)[i].label,(*exactivities)[i].location,(*exactivities)[i].type,(*exactivities)[i].ID);

    }

}
void PROVC::setActivity(prov::Activity::startTime_type startTime, prov::Activity::startTime_type endTime, string label, string location, string type, string ID){

    //prov::Document::activity_sequence& actseq (provdoc.activity());
    //create entity
    prov::Activity tmpact;
    //set time
    tmpact.startTime (startTime);
    tmpact.endTime (endTime);
    //set label
    prov::Activity::label_sequence& ls (tmpact.label());
    ls.push_back(label);
    //location
    prov::Activity::location_sequence& loqseq (tmpact.location());
    loqseq.push_back(location);
    // type
    prov::Activity::type_sequence& typseq (tmpact.type());
    typseq.push_back(type);

    //set ID
    prov::Activity::id_type myidytpe(ID);
    tmpact.id (myidytpe);
    //
    activityseqvar.push_back(tmpact);

}

void PROVC::setwasInformedBys(){
    this->setwasInformedBy("informedID","informantID","label", "type", "ID");
}
void PROVC::setwasInformedBy(string informedID, string informantID, string label, string type, string ID){
    prov::Document::wasInformedBy_sequence& infseq (provdoc.wasInformedBy());

    //set Informant and informed ID
    prov::IDRef::ref_type informedtype(informedID);
    prov::Communication::informed_type tmpinformed(informedtype);
    prov::IDRef::ref_type informanttype(informantID);
    prov::Communication::informed_type tmpinformant(informanttype);
    prov::Communication tmpcom(tmpinformed,tmpinformant);

    //set label
    prov::Communication::label_sequence& ls (tmpcom.label());
    ls.push_back(label);
    // type
    prov::Communication::type_sequence& typseq (tmpcom.type());
    typseq.push_back(type);
    //set ID
    prov::Communication::id_type myidytpe(ID);
    tmpcom.id (myidytpe);
    //
    infseq.push_back(tmpcom);

}

void PROVC::setwasStartedBys(){
    prov::Start::time_type tmptime(2015,4,27,17,30,3.1234);
    this->setwasStartedBy("activityID","triggerID","starterID",tmptime,"Label","Location","role: in","type","301");

}
void PROVC::setwasStartedBy(string actID, string trigID, string starterID, prov::Start::time_type startTime, string label, string location, string role, string type, string ID){
    prov::Document::wasStartedBy_sequence& strtseq (provdoc.wasStartedBy());

    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Start::activity_type tmpact(actreftype);
    prov::Start tmpstart(tmpact);
    //set Trigger ID
    prov::IDRef::ref_type trigreftype(trigID);
    prov::Start::trigger_type tmptrig(trigreftype);
    tmpstart.trigger(tmptrig);
    //set Starter ID
    prov::IDRef::ref_type starterreftype(starterID);
    prov::Start::starter_type tmpstarter(starterreftype);
    tmpstart.starter(tmpstarter);

    //time
    tmpstart.time(startTime);
    //set label
    prov::Start::label_sequence& ls (tmpstart.label());
    ls.push_back(label);
    //location
    prov::Start::location_sequence& loqseq (tmpstart.location());
    loqseq.push_back(location);
    // type
    prov::Start::type_sequence& typseq (tmpstart.type());
    typseq.push_back(type);
    // role
    prov::Start::role_sequence& rolseq(tmpstart.role());
    rolseq.push_back(role);

    //set ID
    prov::Start::id_type myidytpe(ID);
    tmpstart.id (myidytpe);
    //
    strtseq.push_back(tmpstart);
}

void PROVC::setwasEndedBys(){
    prov::End::time_type tmptime(2015,4,27,17,30,3.1234);
    this->setwasEndedBy("activityID","triggerID","enderID",tmptime,"Label","Location","role: in","type","301");

}
void PROVC::setwasEndedBy(string actID, string trigID, string enderID, prov::End::time_type endTime, string label, string location, string role, string type, string ID){
    prov::Document::wasEndedBy_sequence& endseq (provdoc.wasEndedBy());

    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::End::activity_type tmpact(actreftype);
    prov::End tmpEnd(tmpact);
    //set Trigger ID
    prov::IDRef::ref_type trigreftype(trigID);
    prov::End::trigger_type tmptrig(trigreftype);
    tmpEnd.trigger(tmptrig);
    //set Ender ID
    prov::IDRef::ref_type Enderreftype(enderID);
    prov::End::ender_type tmpEnder(Enderreftype);
    tmpEnd.ender(tmpEnder);

    //time
    tmpEnd.time(endTime);
    //set label
    prov::End::label_sequence& ls (tmpEnd.label());
    ls.push_back(label);
    //location
    prov::End::location_sequence& loqseq (tmpEnd.location());
    loqseq.push_back(location);
    // type
    prov::End::type_sequence& typseq (tmpEnd.type());
    typseq.push_back(type);
    // role
    prov::End::role_sequence& rolseq(tmpEnd.role());
    rolseq.push_back(role);

    //set ID
    prov::End::id_type myidytpe(ID);
    tmpEnd.id (myidytpe);
    //
    endseq.push_back(tmpEnd);
}

void PROVC::setwasInvalidatedBys(){

    prov::Invalidation::time_type tmptime(2015,4,27,17,30,4.1234);
    this->setwasInvalidatedBy("activityID","EntityID",tmptime,"Label","Location","role: in","type","301");
}
void PROVC::setwasInvalidatedBy(string actID,string entID,prov::Invalidation::time_type invalidTime,string label, string location, string role, string type, string ID){
    prov::Document::wasInvalidatedBy_sequence& invalseq (provdoc.wasInvalidatedBy());

    //set Entity ID
    prov::IDRef::ref_type entreftype(entID);
    prov::Invalidation::entity_type tmpent(entreftype);
    prov::Invalidation tmpinv(tmpent);
    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Invalidation::activity_type tmpact(actreftype);
    tmpinv.activity(tmpact);


    //time
    tmpinv.time(invalidTime);
    //set label
    prov::Invalidation::label_sequence& ls (tmpinv.label());
    ls.push_back(label);
    //location
    prov::Invalidation::location_sequence& loqseq (tmpinv.location());
    loqseq.push_back(location);
    // type
    prov::Invalidation::type_sequence& typseq (tmpinv.type());
    typseq.push_back(type);
    // role
    prov::Invalidation::role_sequence& rolseq(tmpinv.role());
    rolseq.push_back(role);

    //set ID
    prov::Invalidation::id_type myidytpe(ID);
    tmpinv.id (myidytpe);
    //
    invalseq.push_back(tmpinv);
}

void PROVC::setUseds(){
    prov::Usage::time_type tmptime(2015,4,27,17,30,3.1234);
    this->setused("activityID","EntityID",tmptime,"role1 1","Location","role: in","type","301");
}
void PROVC::setUseds(vector<ProvUtils::Used>* exUseds){
    for (int i = 0; i < exUseds->size(); i++) {
        boost::posix_time::ptime t1(boost::posix_time::time_from_string((*exUseds)[i].usedTime));
        prov::Usage::time_type tmptimetype(t1.date().year(),t1.date().month(),t1.date().day(),t1.time_of_day().hours(),t1.time_of_day().minutes(),t1.time_of_day().seconds());
        this->setused((*exUseds)[i].actID,(*exUseds)[i].entID,tmptimetype,(*exUseds)[i].label,(*exUseds)[i].location,(*exUseds)[i].role,(*exUseds)[i].type,(*exUseds)[i].ID);

    }

}
void PROVC::setused(string actID,string entID,prov::Usage::time_type usedTime,string label, string location, string role, string type, string ID){
    //prov::Document::used_sequence& usdseq (provdoc.used());

    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Usage::activity_type tmpact(actreftype);
    prov::Usage tmpusd(tmpact);
    //set Entity ID
    prov::IDRef::ref_type entreftype(entID);
    prov::Usage::entity_type tmpent(entreftype);
    tmpusd.entity(tmpent);
    //time
    tmpusd.time(usedTime);
    //set label
    prov::Usage::label_sequence& ls (tmpusd.label());
    ls.push_back(label);
    //location
    prov::Usage::location_sequence& loqseq (tmpusd.location());
    loqseq.push_back(location);
    // type
    prov::Usage::type_sequence& typseq (tmpusd.type());
    typseq.push_back(type);
    // role
    prov::Usage::role_sequence& rolseq(tmpusd.role());
    rolseq.push_back(role);

    //set ID
    prov::Usage::id_type myidytpe(ID);
    tmpusd.id (myidytpe);
    //
    usedseqvar.push_back(tmpusd);
}

void PROVC::setwasGeneratedBys(){

    prov::Generation::time_type tmptime(2015,4,27,17,30,4.1234);
    this->setwasGeneratedBy("activityID","EntityID",tmptime,"role1 1","Location","role: in","type","301");
}
void PROVC::setwasGeneratedBys(vector<ProvUtils::WasGeneratedBy> *exWasGeneratedBys){
    for (int i = 0; i < exWasGeneratedBys->size(); i++) {
        boost::posix_time::ptime t1(boost::posix_time::time_from_string((*exWasGeneratedBys)[i].generateTime));
        prov::Generation::time_type tmptimetype(t1.date().year(),t1.date().month(),t1.date().day(),t1.time_of_day().hours(),t1.time_of_day().minutes(),t1.time_of_day().seconds());
        this->setwasGeneratedBy((*exWasGeneratedBys)[i].actID,(*exWasGeneratedBys)[i].entID,tmptimetype,(*exWasGeneratedBys)[i].label,(*exWasGeneratedBys)[i].location,(*exWasGeneratedBys)[i].role,(*exWasGeneratedBys)[i].type,(*exWasGeneratedBys)[i].ID);

    }

}
void PROVC::setwasGeneratedBy(string actID,string entID,prov::Generation::time_type usedTime,string label, string location, string role, string type, string ID){
    //prov::Document::wasGeneratedBy_sequence& genseq (provdoc.wasGeneratedBy());

    //set Entity ID
    prov::IDRef::ref_type entreftype(entID);
    prov::Generation::entity_type tmpent(entreftype);
    prov::Generation tmpgen(tmpent);
    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Generation::activity_type tmpact(actreftype);
    tmpgen.activity(tmpact);


    //time
    tmpgen.time(usedTime);
    //set label
    prov::Generation::label_sequence& ls (tmpgen.label());
    ls.push_back(label);
    //location
    prov::Generation::location_sequence& loqseq (tmpgen.location());
    loqseq.push_back(location);
    // type
    prov::Generation::type_sequence& typseq (tmpgen.type());
    typseq.push_back(type);
    // role
    prov::Generation::role_sequence& rolseq(tmpgen.role());
    rolseq.push_back(role);

    //set ID
    prov::Generation::id_type myidytpe(ID);
    tmpgen.id (myidytpe);
    //
    generatedbyseqvar.push_back(tmpgen);
}

void PROVC::setwasDerivedFroms(){

    this->setwasDerivedFrom("GeneratedEntityID","UsedEntityID","ActivityID","GenerationID", "UsageID", "Label", "Type", "ID");
}
void PROVC::setwasDerivedFrom(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID){
    prov::Document::wasDerivedFrom_sequence& drvseq (provdoc.wasDerivedFrom());

    //set Generated and Used Entity ID
    prov::IDRef::ref_type genEntIDtype(genEntID);
    prov::Derivation::generatedEntity_type tmpgenEntID(genEntIDtype);


    prov::IDRef::ref_type usdEntIDtype(usdEntID);
    prov::Derivation::usedEntity_type tmpusdEntID(usdEntIDtype);

    prov::Derivation tmpdrv(tmpgenEntID,tmpusdEntID);
    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Derivation::activity_type tmpact(actreftype);
    tmpdrv.activity(tmpact);

    //set Generation ID
    prov::IDRef::ref_type genreftype(genID);
    prov::Derivation::generation_type tmpgen(genreftype);
    tmpdrv.generation(tmpgen);

    //set Usage ID
    prov::IDRef::ref_type usgreftype(usgID);
    prov::Derivation::usage_type tmpusg(usgreftype);
    tmpdrv.usage(tmpusg);



    //set label
    prov::Derivation::label_sequence& ls (tmpdrv.label());
    ls.push_back(label);
    // type
    prov::Derivation::type_sequence& typseq (tmpdrv.type());
    typseq.push_back(type);
    //set ID
    prov::Derivation::id_type myidytpe(ID);
    tmpdrv.id (myidytpe);
    //
    drvseq.push_back(tmpdrv);
}

void PROVC::setwasRevisionOfs(){

    this->setwasRevisionOf("GeneratedEntityID","UsedEntityID","ActivityID","GenerationID", "UsageID", "Label", "Type", "ID");
}
void PROVC::setwasRevisionOf(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID){
    prov::Document::wasRevisionOf_sequence& drvseq (provdoc.wasRevisionOf());

    //set Generated and Used Entity ID
    prov::IDRef::ref_type genEntIDtype(genEntID);
    prov::Revision::generatedEntity_type tmpgenEntID(genEntIDtype);


    prov::IDRef::ref_type usdEntIDtype(usdEntID);
    prov::Revision::usedEntity_type tmpusdEntID(usdEntIDtype);

    prov::Revision tmpdrv(tmpgenEntID,tmpusdEntID);
    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Revision::activity_type tmpact(actreftype);
    tmpdrv.activity(tmpact);

    //set Generation ID
    prov::IDRef::ref_type genreftype(genID);
    prov::Revision::generation_type tmpgen(genreftype);
    tmpdrv.generation(tmpgen);

    //set Usage ID
    prov::IDRef::ref_type usgreftype(usgID);
    prov::Revision::usage_type tmpusg(usgreftype);
    tmpdrv.usage(tmpusg);



    //set label
    prov::Revision::label_sequence& ls (tmpdrv.label());
    ls.push_back(label);
    // type
    prov::Revision::type_sequence& typseq (tmpdrv.type());
    typseq.push_back(type);
    //set ID
    prov::Revision::id_type myidytpe(ID);
    tmpdrv.id (myidytpe);
    //
    drvseq.push_back(tmpdrv);
}

void PROVC::setwasQuotedFroms(){

    this->setwasQuotedFrom("GeneratedEntityID","UsedEntityID","ActivityID","GenerationID", "UsageID", "Label", "Type", "ID");
}
void PROVC::setwasQuotedFrom(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID){
    prov::Document::wasQuotedFrom_sequence& drvseq (provdoc.wasQuotedFrom());

    //set Generated and Used Entity ID
    prov::IDRef::ref_type genEntIDtype(genEntID);
    prov::Quotation::generatedEntity_type tmpgenEntID(genEntIDtype);


    prov::IDRef::ref_type usdEntIDtype(usdEntID);
    prov::Quotation::usedEntity_type tmpusdEntID(usdEntIDtype);

    prov::Quotation tmpdrv(tmpgenEntID,tmpusdEntID);
    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Quotation::activity_type tmpact(actreftype);
    tmpdrv.activity(tmpact);

    //set Generation ID
    prov::IDRef::ref_type genreftype(genID);
    prov::Quotation::generation_type tmpgen(genreftype);
    tmpdrv.generation(tmpgen);

    //set Usage ID
    prov::IDRef::ref_type usgreftype(usgID);
    prov::Quotation::usage_type tmpusg(usgreftype);
    tmpdrv.usage(tmpusg);



    //set label
    prov::Quotation::label_sequence& ls (tmpdrv.label());
    ls.push_back(label);
    // type
    prov::Quotation::type_sequence& typseq (tmpdrv.type());
    typseq.push_back(type);
    //set ID
    prov::Quotation::id_type myidytpe(ID);
    tmpdrv.id (myidytpe);
    //
    drvseq.push_back(tmpdrv);
}

void PROVC::sethadPrimarySources(){

    this->sethadPrimarySource("GeneratedEntityID","UsedEntityID","ActivityID","GenerationID", "UsageID", "Label", "Type", "ID");
}
void PROVC::sethadPrimarySource(string genEntID,string usdEntID,string actID, string genID, string usgID, string label, string type, string ID){
    prov::Document::hadPrimarySource_sequence& drvseq (provdoc.hadPrimarySource());

    //set Generated and Used Entity ID
    prov::IDRef::ref_type genEntIDtype(genEntID);
    prov::PrimarySource::generatedEntity_type tmpgenEntID(genEntIDtype);


    prov::IDRef::ref_type usdEntIDtype(usdEntID);
    prov::PrimarySource::usedEntity_type tmpusdEntID(usdEntIDtype);

    prov::PrimarySource tmpdrv(tmpgenEntID,tmpusdEntID);
    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::PrimarySource::activity_type tmpact(actreftype);
    tmpdrv.activity(tmpact);

    //set Generation ID
    prov::IDRef::ref_type genreftype(genID);
    prov::PrimarySource::generation_type tmpgen(genreftype);
    tmpdrv.generation(tmpgen);

    //set Usage ID
    prov::IDRef::ref_type usgreftype(usgID);
    prov::PrimarySource::usage_type tmpusg(usgreftype);
    tmpdrv.usage(tmpusg);



    //set label
    prov::PrimarySource::label_sequence& ls (tmpdrv.label());
    ls.push_back(label);
    // type
    prov::PrimarySource::type_sequence& typseq (tmpdrv.type());
    typseq.push_back(type);
    //set ID
    prov::PrimarySource::id_type myidytpe(ID);
    tmpdrv.id (myidytpe);
    //
    drvseq.push_back(tmpdrv);
}

void PROVC::setAgents(){
    this->setAgent("Agent 1","Location","Prov:agent","201");
}
void PROVC::setAgents(vector<ProvUtils::Agent>* exAgents){
    for (int i = 0; i < exAgents->size(); i++) {
        this->setAgent((*exAgents)[i].label,(*exAgents)[i].location, (*exAgents)[i].type, (*exAgents)[i].ID);
    }

}
void PROVC::setAgent(string label, string location, string type, string ID){
    //prov::Document::agent_sequence& agtseq (provdoc.agent());
    //create entity
    prov::Agent tmpAgent;
    //set label
    prov::Agent::label_sequence& ls (tmpAgent.label());
    ls.push_back(label);
    //location
    prov::Agent::location_sequence& loqseq (tmpAgent.location());
    //loqseq.push_back("/home/myfile.txt");
    loqseq.push_back(location);
    // type
    prov::Agent::type_sequence& typseq (tmpAgent.type());
    typseq.push_back(type);
    //set ID
    prov::Agent::id_type myidytpe(ID);
    tmpAgent.id (myidytpe);
    //
    agentseqvar.push_back(tmpAgent);
}

void PROVC::setpersons(){
    this->setperson("person 1","Location","Prov:person","201");
}
void PROVC::setperson(string label, string location, string type, string ID){
    prov::Document::person_sequence& agtseq (provdoc.person());
    //create entity
    prov::Person tmpAgent;
    //set label
    prov::Person::label_sequence& ls (tmpAgent.label());
    ls.push_back(label);
    //location
    prov::Person::location_sequence& loqseq (tmpAgent.location());
    //loqseq.push_back("/home/myfile.txt");
    loqseq.push_back(location);
    // type
    prov::Person::type_sequence& typseq (tmpAgent.type());
    typseq.push_back(type);
    //set ID
    prov::Person::id_type myidytpe(ID);
    tmpAgent.id (myidytpe);
    //
    agtseq.push_back(tmpAgent);
}

void PROVC::setorganizations(){
    this->setorganization("Organization 1","Location","Prov:Organization","201");
}
void PROVC::setorganization(string label, string location, string type, string ID){
    prov::Document::organization_sequence& agtseq (provdoc.organization());
    //create entity
    prov::Organization tmpAgent;
    //set label
    prov::Organization::label_sequence& ls (tmpAgent.label());
    ls.push_back(label);
    //location
    prov::Organization::location_sequence& loqseq (tmpAgent.location());
    //loqseq.push_back("/home/myfile.txt");
    loqseq.push_back(location);
    // type
    prov::Organization::type_sequence& typseq (tmpAgent.type());
    typseq.push_back(type);
    //set ID
    prov::Organization::id_type myidytpe(ID);
    tmpAgent.id (myidytpe);
    //
    agtseq.push_back(tmpAgent);
}

void PROVC::setsoftwareAgents(){
    this->setsoftwareAgent("SoftwareAgent 1","Location","Prov:SoftwareAgent","201");
}
void PROVC::setsoftwareAgent(string label, string location, string type, string ID){
    prov::Document::softwareAgent_sequence& agtseq (provdoc.softwareAgent());
    //create entity
    prov::SoftwareAgent tmpAgent;
    //set label
    prov::SoftwareAgent::label_sequence& ls (tmpAgent.label());
    ls.push_back(label);
    //location
    prov::SoftwareAgent::location_sequence& loqseq (tmpAgent.location());
    //loqseq.push_back("/home/myfile.txt");
    loqseq.push_back(location);
    // type
    prov::SoftwareAgent::type_sequence& typseq (tmpAgent.type());
    typseq.push_back(type);
    //set ID
    prov::SoftwareAgent::id_type myidytpe(ID);
    tmpAgent.id (myidytpe);
    //
    agtseq.push_back(tmpAgent);
}
void PROVC::setwasAttributedTos(){
    this->setwasAttributedTo("EntityID","AgentID","Label","type","401");

}
void PROVC::setwasAttributedTo(string entID,string agentID,string label, string type, string ID){
    prov::Document::wasAttributedTo_sequence& assseq (provdoc.wasAttributedTo());
    //set activity and Agent ID
    prov::IDRef::ref_type entreftype(entID);
    prov::Attribution::entity_type tmpent(entreftype);


    prov::IDRef::ref_type agtreftype(agentID);
    prov::Attribution::agent_type tmpagt(agtreftype);

    prov::Attribution tmpass(tmpent,tmpagt);

    //set label
    prov::Attribution::label_sequence& ls (tmpass.label());
    ls.push_back(label);
    // type
    prov::Attribution::type_sequence& typseq (tmpass.type());
    typseq.push_back(type);

    //set ID
    prov::Attribution::id_type myidytpe(ID);
    tmpass.id (myidytpe);
    //
    assseq.push_back(tmpass);
}
void PROVC::setwasAssociatedWiths(){
    this->setwasAssociatedWith("activityID","AgentID","PlanID","Label","role: in","type","401");

}
void PROVC::setwasAssociatedWith(string actID,string agentID,string planID,string label, string role, string type, string ID){
    prov::Document::wasAssociatedWith_sequence& assseq (provdoc.wasAssociatedWith());
    //set activity ID
    prov::IDRef::ref_type actreftype(actID);
    prov::Association::activity_type tmpact(actreftype);
    prov::Association tmpass(tmpact);
    //set Agent ID
    prov::IDRef::ref_type agtreftype(agentID);
    prov::Association::agent_type tmpagt(agtreftype);
    tmpass.agent(tmpagt);
    //set Plan ID
    prov::IDRef::ref_type plnreftype(planID);
    prov::Association::plan_type tmppln(plnreftype);
    tmpass.plan(tmppln);
    //set label
    prov::Association::label_sequence& ls (tmpass.label());
    ls.push_back(label);
    // type
    prov::Association::type_sequence& typseq (tmpass.type());
    typseq.push_back(type);
    // role
    prov::Association::role_sequence& rolseq(tmpass.role());
    rolseq.push_back(role);

    //set ID
    prov::Association::id_type myidytpe(ID);
    tmpass.id (myidytpe);
    //
    assseq.push_back(tmpass);
}
void PROVC::setactedOnBehalfOfs(){
    this->setactedOnBehalfOf("delegateID","responsibleID","actID","label", "type", "ID");
}
void PROVC::setactedOnBehalfOf(string delegateID,string responsibleID,string actID, string label, string type, string ID){
    prov::Document::actedOnBehalfOf_sequence& actseq (provdoc.actedOnBehalfOf());

    //set Delegate and Responsible ID
    prov::IDRef::ref_type delegatetype(delegateID);
    prov::Delegation::delegate_type tmpdelegate(delegatetype);
    prov::IDRef::ref_type responsibletype(responsibleID);
    prov::Delegation::responsible_type tmpresponsible(responsibletype);
    prov::Delegation tmpdlg(tmpdelegate,tmpresponsible);

    //set activity
    prov::IDRef::ref_type actreftype(actID);
    prov::Delegation::activity_type tmpact(actreftype);
    tmpdlg.activity(tmpact);
    //set label
    prov::Delegation::label_sequence& ls (tmpdlg.label());
    ls.push_back(label);
    // type
    prov::Delegation::type_sequence& typseq (tmpdlg.type());
    typseq.push_back(type);
    //set ID
    prov::Delegation::id_type myidytpe(ID);
    tmpdlg.id (myidytpe);
    //
    actseq.push_back(tmpdlg);

}
void PROVC::setwasInfluencedBys(){
    this->setwasInfluencedBy("influenceeID","influencerID","label", "type", "ID");
}
void PROVC::setwasInfluencedBy(string influenceeID, string influencerID, string label, string type, string ID){
    prov::Document::wasInfluencedBy_sequence& infseq (provdoc.wasInfluencedBy());

    //set Informant and informed ID
    prov::IDRef::ref_type influenceetype(influenceeID);
    prov::Influence::influencee_type tmpinfluencee(influenceetype);
    prov::IDRef::ref_type influencertype(influencerID);
    prov::Influence::influencer_type tmpinfluencer(influencertype);
    prov::Influence tmpinf(tmpinfluencee,tmpinfluencer);

    //set label
    prov::Influence::label_sequence& ls (tmpinf.label());
    ls.push_back(label);
    // type
    prov::Influence::type_sequence& typseq (tmpinf.type());
    typseq.push_back(type);
    //set ID
    prov::Influence::id_type myidytpe(ID);
    tmpinf.id (myidytpe);
    //
    infseq.push_back(tmpinf);

}
void PROVC::setspecializationOfs(){
    this->setspecializationOf("SpeceficEntityID" , "GeneralEntityID");
}
void PROVC::setspecializationOf(string spcEnt, string genEnt){
    prov::Document::specializationOf_sequence& spcseq (provdoc.specializationOf());

    //set Informant and informed ID
    prov::IDRef::ref_type spcEntreftype(spcEnt);
    prov::Specialization::specificEntity_type tmpspcEnt(spcEntreftype);
    prov::IDRef::ref_type genEntreftype(genEnt);
    prov::Specialization::generalEntity_type tmpgenEnt(genEntreftype);
    prov::Specialization tmpspc(tmpspcEnt,tmpgenEnt);
    spcseq.push_back(tmpspc);
}
void PROVC::writeProv(){
    /*this->setEntities();
    this->setActivities();
    this->setAgents();
    this->setUseds();
    this->setwasGeneratedBys();
    this->setwasAssociatedWiths();
    this->setwasDerivedFroms();
    this->setwasRevisionOfs();
    this->setwasInformedBys();
    this->setwasStartedBys();
    this->setwasEndedBys();
    this->setwasInvalidatedBys();
    this->setwasQuotedFroms();
    this->sethadPrimarySources();
    this->setpersons();
    this->setorganizations();
    this->setsoftwareAgents();
    this->setwasAttributedTos();
    this->setactedOnBehalfOfs();
    this->setwasInfluencedBys();*/
    xml_schema::namespace_infomap map;
    map[""].name = "";
    map[""].schema = "../prov-core.xsd";
    std::ofstream ofs("prov-server-client.xml");
    prov::document(ofs, provdoc,map);
    //prov::document(std::cout,provdoc,map);
    std::ostringstream mystream;
    prov::document(mystream,provdoc,map);
    string myxml (mystream.str());
    cout << myxml << endl;


}
