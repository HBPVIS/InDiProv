# Introduction

Provenance Tracking for visual analysis workflows is still a challenge as especially interaction and collaboration aspects are poorly covered in existing realizations.
Therefore, we propose a first prototype addressing these issues based on the PROV model.
Interactions in multiple applications by multiple users can be tracked by means of a web interface and, thus, allowing even for tracking of remote-located collaboration partners.
In the end, we demonstrate the applicability based on two use cases and discuss some open issues not addressed by our implementation so far but that can be easily integrated into our architecture. 

## Features

InDiPROV provides the following major features:

* Server-client implementation of W3C PROV model
* Publish and subscribe to server using ZeroMQ
* Communication encoded in JSON
* Asynchronous, reliable transport
* Using MYSQL to store PROV data
* Convert from sql format to XML format


# Building

InDiPROV is a cross-platform toolkit, designed to run on any modern operating system, including all Unix variants. The server and client application implemented in C++ but the JSON command can be sent to server via any platform.


InDiPROV requires the following external, pre-installed dependencies:

* ZeroMQ
* Boost
* mysqlcppconn
* xerces-c

# Introduction to PROV abstract model

Because of the fact that InDivPROV tools in the implementation of PROV model, it makes scence to first become familar with PROV model. Here [PROV primer](https://www.w3.org/TR/2013/NOTE-prov-primer-20130430/) is good explnation of PROV model to start from. The image below shows the main provenances that can be stored using PROV model.

![alt tag](https://github.com/HBPVIS/InDiProv/blob/master/Documents/starting-points.png)

# Server implemenation

* EReciever function: This is the main function is server side and listening for requests on specified socket ports and proccess them.
* InDiProv class: The main class to process the recieved messages.

# Client implemenation

* InDiProvClient class: This class is the main class to prepare JSON packet and communicate with server. To see how it works please refer to provided example in client folder. Also please look at [Client document](https://github.com/HBPVIS/InDiProv/tree/master/client/Document/html/index.html).

The main functions provided in this version (For list of all functions please look at InDiProv class):

    createWF
    loadWF
    setActivity
    setAgent
    setEntity
    setused
    setwasAssociatedWith
    setwasAttributedTo
    setwasDerivedFrom
    setwasEndedBy
    setwasGeneratedBy
    setwasInformedBy
    setwasStartedBy
