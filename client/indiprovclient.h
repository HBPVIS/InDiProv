#ifndef INDIPROVCLIENT_H
#define INDIPROVCLIENT_H
#include "zmq.hpp"
#include <json/json.h>
#include "provutils.h"
using namespace std;
//!  InDiProvClient class
/*!
  This class provides easy to use function to prepare, send/recieve messages to/from InDiProv server.
  After calling a function from this library the JSON based command of the function
  (including workflow name, password and required arguments) will be sent to the server
  and the corresponding result is again JSON based, which will be proccessed and returned as type of function output.

*/
class InDiProvClient
{
public:
    //! Constructor
    /*!
      stablishing connection to the server tcp://localhost:5556-5560
      zmq::context_t* context = zmq::socket_t(zmq::socket_t* socket, ZMQ_REQ)
      */
    InDiProvClient();
    //! Send the JSON based command to the defined server
    string ESender(string msgstr);
    //! Serialize command/function into JSON based
    string serializeToJson(vector<string> commandVec);
    void json_parse(json_object * jobj);
    //! Returns the current time of the client in string format.
    //! Important: Just use this function as time string argument.
    string getCurrentTime();
    //WF functions
    //! Creating new workflow with coresponding password and returns the unique id of workflow if the function is successful, else returns -1.
    int createWF(string WFName , string password);
    //! Loading workflow with corresponding id and password. Returns the the same id if the function is successful, else returns -1.
    //! Also the WFID and WFPass variables of the object will be updated if if the function is successful.
    int loadWF(int WFid , string password);
    //! Returns the current WFID, -1 means no WF is selected.
    int getWFID();
    //! Returns the current WFPass (workflow passwords)
    string getWFPass();
    //! Export the current provenance information into xml format.
    int exportPROV();

    //prov functions
    //! Creating new Entity returns the unique id of new Entity if the function is successful, else returns -1.
    /*!
        An entity is a physical, digital, conceptual, or other kind of thing with some fixed aspects; entities may be real or imaginary.<br>
        Example: An entity may be the document at IRI http://www.bbc.co.uk/news/science-environment-17526723, a file in a file system, a car, or an idea.
    */
    int setEntity(string label, string location, string type, string val);

    //! Creating new Activity returns the unique id of new Activity if the function is successful, else returns -1.
    /*!
        An activity is something that occurs over a period of time and acts upon or with entities; it may include consuming, processing, transforming, modifying, relocating, using, or generating entities.
        Just as entities cover a broad range of notions, activities can cover a broad range of notions: information processing activities may for example move, copy, or duplicate digital entities; physical activities can include driving a car between two locations or printing a book.<br>
        Example: An activity may be the publishing of a document on the Web, sending a twitter message, extracting metadata embedded in a file, driving a car from Boston to Cambridge, assembling a data set based on a set of measurements, performing a statistical analysis over a data set, sorting news items according to some criteria, running a SPARQL query over a triple store, or editing a file.
    */
    int setActivity(string startTime, string endTime, string label, string location, string type);

    //! Creating new Agent returns the unique id of new Agent if the function is successful, else returns -1.
    /*!
        For many purposes, a key consideration for deciding whether something is reliable and/or trustworthy is knowing who or what was reponsible for its production.
        Data published by a respected independent organization may be considered more trustworthy than that from a lobby organization; a claim by a well-known scientist with an established track record may be more believed than a claim by a new student; a calculation performed by an established software library may be more reliable than by a one-off program.
        An agent is something that bears some form of responsibility for an activity taking place, for the existence of an entity, or for another agent's activity.
        An agent may be a particular type of entity or activity. This means that the model can be used to express provenance of the agents themselves.<br>
        Example: Software for checking the use of grammar in a document may be defined as an agent of a document preparation activity; one can also describe its provenance, including for instance the vendor and the version history. A site selling books on the Web, the services involved in the processing of orders, and the companies hosting them are also agents.
    */
    int setAgent(string label, string location, string type);

    //! Creating new Usage returns the unique id of new usage if the function is successful, else returns -1.
    /*!
        Activities and entities are associated with each other in two different ways: activities utilize entities and activities produce entities. The act of utilizing or producing an entity may have a duration. The term 'generation' refers to the completion of the act of producing; likewise, the term 'usage' refers to the beginning of the act of utilizing entities. Thus, we define the following concepts of generation and usage.
        Usage is the beginning of utilizing an entity by an activity. Before usage, the activity had not begun to utilize this entity and could not have been affected by the entity.<br>
        Example 1: Usage examples include a procedure beginning to consume an argument, a service starting to read a value on a port, a program beginning to read a configuration file, or the point at which an ingredient, such as eggs, is being added in a baking activity. Usage may entirely consume an entity (e.g. eggs are no longer available after being added to the mix); in contrast, the same entity may be used multiple times, possibly by different activities (e.g. a file on a file system can be read indefinitely).<br>
        Example 2: Let us consider the activity of driving a car from Boston to Cambridge. One might reasonably ask what entities are used and generated by this activity. This is answered by considering that a single artifact may correspond to several entities; in this case, a car in Boston may be a different entity from the same car in Cambridge. Thus, among other things, an entity "car in Boston" would be used, and a new entity "car in Cambridge" would be generated by this activity of driving. The provenance trace of the car might include: designed in Japan, manufactured in Korea, shipped to Boston USA, purchased by customer, driven to Cambridge, serviced by engineer in Cambridge, etc., all of which might be important information when deciding whether or not it represents a sensible second-hand purchase. Or some of it might alternatively be relevant when trying to determine the truth of a web page reporting a traffic violation involving that car. This breadth of provenance allows descriptions of interactions between physical and digital artifacts.
    */
    int setused(int actID,int entID,string usedTime,string label, string location, string role, string type);

    //! Creating new Generation returns the unique id of new Generation if the function is successful, else returns -1.
    /*!
        Activities and entities are associated with each other in two different ways: activities utilize entities and activities produce entities. The act of utilizing or producing an entity may have a duration. The term 'generation' refers to the completion of the act of producing; likewise, the term 'usage' refers to the beginning of the act of utilizing entities. Thus, we define the following concepts of generation and usage.
        Generation is the completion of production of a new entity by an activity. This entity did not exist before generation and becomes available for usage after this generation.<br>
        Example: Examples of generation are the completed creation of a file by a program, the completed creation of a linked data set, and the completed publication of a new version of a document.
    */
    int setwasGeneratedBy(int entID,int actID, string generateTime, string label, string location, string role, string type);

    //! Creating new Derivation returns the unique id of new Derivation if the function is successful, else returns -1.
    /*!
        Activities utilize entities and produce entities. In some cases, utilizing an entity influences the creation of another in some way. This notion of 'influence' is captured by derivations, defined as follows.
        A derivation is a transformation of an entity into another, an update of an entity resulting in a new one, or the construction of a new entity based on a pre-existing entity.<br>
        Example: Examples of derivation include the transformation of a relational table into a linked data set, the transformation of a canvas into a painting, the transportation of a work of art from London to New York, and a physical transformation such as the melting of ice into water.<br>
        The focus of derivation is on connecting a generated entity to a used entity. While the basic idea is simple, the concept of derivation can be quite subtle: implicit is the notion that the generated entity was affected in some way by the used entity. If an artifact was used by an activity that also generated a new artifact, it does not always follow that the second artifact was derived from the first. In the activity of creating a painting, an artist may have mixed some paint that was never actually applied to the canvas: the painting would typically not be considered a derivation from the unused paint. PROV does not attempt to specify the conditions under which derivations exist; rather, derivation is considered to have been determined by unspecified means. Thus, while a chain of usage and generation is necessary for a derivation to hold between entities, it is not sufficient; some form of influence occurring during the activities involved is also needed.
    */
    int setwasDerivedFrom(int genEntID, int usdEntID, int actID, int genID, int usgID, string label, string type);

    //! Creating new Attribution returns the unique id of new Attribution if the function is successful, else returns -1.
    /*! Agents can be related to entities, activities, and other agents.
        Attribution is the ascribing of an entity to an agent.<br>
        Example: EA blog post can be attributed to an author, a mobile phone to its manufacturer.<br>
    */
    int setwasAttributedTo(int entID,int agentID,string label, string type);

    //! Creating new Association returns the unique id of new Association if the function is successful, else returns -1.
    /*! Agents are defined as having some kind of responsibility for activities.
        An activity association is an assignment of responsibility to an agent for an activity, indicating that the agent had a role in the activity.<br>
        Example: Examples of association between an activity and an agent are:<br>
            creation of a web page under the guidance of a designer;<br>
            various forms of participation in a panel discussion, including audience member, panelist, or panel chair;<br>
            a public event, sponsored by a company, and hosted by a museum;<br>

    */
    int setwasAssociatedWith(int actID, int agentID, int planID, string label, string role, string type);

    //! Creating new Communication returns the unique id of new Communication if the function is successful, else returns -1.
    /*! The generation of an entity by an activity and its subsequent usage by another activity is termed communication.
        Communication is the exchange of some unspecified entity by two activities, one activity using some entity generated by the other.<br>
        Example: The activity of writing a celebrity article was informed by (a communication instance) the activity of intercepting voicemails.
    */
    int setwasInformedBy(int informed, int informant, string label, string type);

    //! Creating new Start returns the unique id of new Start if the function is successful, else returns -1.
    /*! Start is when an activity is deemed to have been started by an entity, known as trigger. The activity did not exist before its start. Any usage, generation, or invalidation involving an activity follows the activity's start. A start may refer to a trigger entity that set off the activity, or to an activity, known as starter, that generated the trigger.
    */
    int setwasStartedBy(int actID, int entID, int starterActID, string sTime, string label, string location, string role, string type);

    //! Creating new returns the unique id of new if the function is successful, else returns -1.
    /*! End is when an activity is deemed to have been ended by an entity, known as trigger. The activity no longer exists after its end. Any usage, generation, or invalidation involving an activity precedes the activity's end. An end may refer to a trigger entity that terminated the activity, or to an activity, known as ender that generated the trigger.
    */
    int setwasEndedBy(int actID, int entID, int enderActID, string eTime, string label, string location, string role, string type);
    //prov query function

    //! Retrieving all Entities from the loaded workflow in JSON based
    string getEntities();

    //! Retrieving all Activities from the loaded workflow in JSON based
    string getActivities();

    //! Retrieving all Agents from the loaded workflow in JSON based
    string getAgents();

    //! Retrieving all Usages from the loaded workflow in JSON based
    string getUseds();

    //! Retrieving all Generations from the loaded workflow in JSON based
    string getWasGeneratedBys();

    //! Retrieving all Derivations from the loaded workflow in JSON based
    string getWasDerivedFroms();

    //! Retrieving all Attributions from the loaded workflow in JSON based
    string getWasAttributedTos();

    //! Retrieving all Associations from the loaded workflow in JSON based
    string getWasAssociatedWiths();

    //! Retrieving all Communications from the loaded workflow in JSON based
    string getwasInformedBys();

    //! Retrieving all Starts from the loaded workflow in JSON based
    string getwasStartedBys();

    //! Retrieving all Ends from the loaded workflow in JSON based
    string getwasEndedBys();

    //deserialize

    //! Exchange JSON based Entities to vector<ProvUtils::Entity>
    vector<ProvUtils::Entity> deSerializeEntities(char *entitiesStr);

    //! Exchange JSON based Activities to vector<ProvUtils::Activity>
    vector<ProvUtils::Activity> deSerializeActivities(char *activitiesStr);

    //! Exchange JSON based Agents to vector<ProvUtils::Agent>
    vector<ProvUtils::Agent> deSerializeAgents(char *agentsStr);

    //! Exchange JSON based Usages to vector<ProvUtils::Used>
    vector<ProvUtils::Used> deSerializeUseds(char *usedsStr);

    //! Exchange JSON based Generations to vector<ProvUtils::WasGeneratedBy>
    vector<ProvUtils::WasGeneratedBy> deSerializeWasGeneratedBys(char *wasGeneratedBysStr);

    //! Exchange JSON based Drivations to vector<ProvUtils::WasDerivedFrom>
    vector<ProvUtils::WasDerivedFrom> deSerializeWasDerivedFroms(char *wasDerivedFromsStr);

    //! Exchange JSON based Attributions to vector<ProvUtils::WasAttributedTo>
    vector<ProvUtils::WasAttributedTo> deSerializeWasAttributedTos(char *wasAttributedToStr);

    //! Exchange JSON based Associations to vector<ProvUtils::WasAssociatedWith>
    vector<ProvUtils::WasAssociatedWith> deSerializeWasAssociatedWiths(char *wasAssociatedWithsStr);

    //! Exchange JSON based Communications to vector<ProvUtils::WasInformedBy>
    vector<ProvUtils::WasInformedBy>  deSerializeWasInformedBys(char *wasInformedByStr);

    //! Exchange JSON based Starts to vector<ProvUtils::WasStartedBy>
    vector<ProvUtils::WasStartedBy> deSerializeWasStartedBys(char *wasStartedBysStr);

    //! Exchange JSON based Ends to vector<ProvUtils::WasEndedBy>
    vector<ProvUtils::WasEndedBy> deSerializeWasEndedBys(char *WasEndedBysStr);


private:
    int WFID;
    string WFPass;
    zmq::context_t* context;
    zmq::socket_t* socket;
    timeval curTime;
};

#endif // INDIPROVCLIENT_H
