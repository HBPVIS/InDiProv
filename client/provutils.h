#ifndef PROVUTILS_H
#define PROVUTILS_H
#include <string>
using namespace std;
class ProvUtils
{
public:
    struct Entity{        
        string label;
        string location;
        string type;
        string value;
        string ID;
    };
    struct Activity{
        string startTime;
        string endTime;
        string label;
        string location;
        string type;
        string ID;
    };
    struct Agent{
        string label;
        string location;
        string type;
        string ID;
    };
    struct Used{
        string actID;
        string entID;
        string usedTime;
        string label;
        string location;
        string role;
        string type;
        string ID;
    };
    struct WasGeneratedBy{
        string actID;
        string entID;
        string generateTime;
        string label;
        string location;
        string role;
        string type;
        string ID;
    };
    struct WasDerivedFrom{
        string genEntID;
        string usdEntID;
        string actID;
        string genID;
        string usgID;
        string label;
        string type;
        string ID;
    };
    struct WasAttributedTo{
        string entID;
        string agentID;
        string label;
        string type;
        string ID;
    };
    struct WasAssociatedWith{
        string actID;
        string agentID;
        string planID;
        string label;
        string role;
        string type;
        string ID;
    };
    struct WasStartedBy{
        string actID;
        string entID;
        string starterActID;
        string sTime;
        string label;
        string location;
        string role;
        string type;
        string ID;
    };
    struct WasEndedBy{
        string actID;
        string entID;
        string enderActID;
        string eTime;
        string label;
        string location;
        string role;
        string type;
        string ID;
    };
    struct WasInformedBy{
        string informed;
        string informant;
        string label;
        string type;
        string ID;
    };


    ProvUtils();
    ~ProvUtils();
};

#endif // PROVUTILS_H
