# Introduction

InDiProv is a prototypical library to allow for provenance tracking in visual analysis workflows. 
We aim to address the tracking of interactions in multiple applications by multiple users by means of a web interface and, thus, allowing even for tracking of remote-located collaboration partners.


## Features

InDiPROV provides the following major features:

* Server-client implementation of the W3C PROV model
* Publish and subscribe to a server using ZeroMQ
* Communication encoded in JSON
* Asynchronous, reliable transport
* Using MySQL to store PROV data
* Export the SQL data into XML format


# Build

InDiPROV is a cross-platform toolkit, designed to run on any modern operating system, including all Unix variants. The server and client application are implemented in C++ but JSON enables to communication to the server with any language or platform. Before starting the server application, a corresponding database needs to be set up. To ensure the correct structure of the database, the [mydb.sql](https://github.com/HBPVIS/InDiProv/blob/master/Database-Structure/mydb.sql) file needs to be imported into MySQL database. Then the connection can be established with a username and password.

InDiPROV requires the following external, pre-installed dependencies:

* [ZeroMQ](http://zeromq.org/)
* [Boost C++ libraries](http://www.boost.org/)
* [MySQL Connector/C++ libraries](https://dev.mysql.com/downloads/connector/cpp/) 
* [Xerces-C++ XML Parser](http://xerces.apache.org/xerces-c/index.html)

The server application is provided as Qt project file and to build the client example application a *Makefile* can be used. We are currently working on *CMakelists* files for easier building on all systems.


# Introduction to the PROV Abstract Model

**InDiPROV** is based on the general **PROV model**. [PROV primer](https://www.w3.org/TR/2013/NOTE-prov-primer-20130430/) gives a good explanation of the PROV model to start with. The image below shows the main provenances that can be stored using PROV model.

![alt tag](https://github.com/HBPVIS/InDiProv/blob/master/Documents/starting-points.png)

# Server Implementation

* *InDiProv* class: The main class to process the received messages.
* *EReciever* function in the *MainWindow* class: This function is waiting for messages on specified socket ports and processes them.

# Client Implementation

* *InDiProvClient* class: This class is the main class to prepare a JSON message and communicate it to server. To see how it works please refer to the provided example in the client folder. Also please take s look at the [client documentation](https://github.com/HBPVIS/InDiProv/tree/master/client/Document/html/index.html).

The most relevant functions provided in this version are listed in the following. For list of all functions please look at InDiProv class.

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

# Help and Feedback

For any questions and feedback please send an e-mail to mdkhatami@gmail.com
