#include "endpoint.h"
#include "serializer/rapidjson.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <thread>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "Controller/vertex-actions.hxx"
#include "Controller/edge-actions.hxx"

#include "config.hxx"

using namespace odb::core;
using namespace Net;

class LogService {
public:
  LogService(Net::Address addr)
    : httpEndpoint(std::make_shared<Net::Http::Endpoint>(addr))
    , desc("InDiProv Logging API", "0.1")
  { }

  void init(size_t thr, int argc, char* argv[]) {
		db = create_database(argc, argv);

    auto opts = Net::Http::Endpoint::options()
      .threads(thr);
    httpEndpoint->init(opts);
    createDescription();
  }

  void start() {
    router.initFromDescription(desc);

    Rest::Swagger swagger(desc);
    swagger
        .uiPath("/doc")
        .uiDirectory(Config::swaggerDir)
        .apiPath("/api.json")
        .serializer(&Rest::Serializer::rapidJson)
        .install(router);

    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
  }

  void shutdown() {
    httpEndpoint->shutdown();
  }

private:
  std::shared_ptr<database> db;
  std::shared_ptr<Net::Http::Endpoint> httpEndpoint;
  Rest::Description desc;
  Rest::Router router;

  void createDescription() {
    desc
      .info()
      .license("Apache", "http://www.apache.org/licenses/LICENSE-2.0");

    auto backendErrorResponse =
      desc.response(Http::Code::Internal_Server_Error, "An error occured with the backend");

    desc
      .route(desc.get("/ready"), "Query if server is ready.")
      .bind(&LogService::handleReady, this)
      .response(Http::Code::Ok, "'Absolutely.' if server is ready");

    desc
      .route(desc.post("/prettify"), "Prettify a JSON payload.")
      .bind(&LogService::prettifyJSON, this)
      .consumes(MIME(Application, Json))
      .produces(MIME(Application, Json))
      .response(Http::Code::Ok, "Prettified 'application/json' data.");

    desc
      .route(desc.post("/log"), "Log the PROV-O objects in payload JSON.")
      .bind(&LogService::logPayloadJSON, this)
      .consumes(MIME(Application, Json))
      .response(Http::Code::Ok, "Logging request confirmation");

    desc
      .route(desc.get("/agents"))
      .bind(&LogService::getAgents, this);

    desc
      .route(desc.get("/activities"))
      .bind(&LogService::getActivities, this);

    desc
      .route(desc.get("/entities"))
      .bind(&LogService::getEntities, this);
  }

  void handleReady(const Rest::Request&, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "Absolutely.\n");
  }

  void prettifyJSON(const Rest::Request& request, Http::ResponseWriter response) {
    auto ct = request.headers().get<Http::Header::ContentType>();
    auto mime = ct->mime();
    if(mime == MIME(Application, Json)) {
      rapidjson::Document doc;
      doc.Parse(request.body().c_str());
      rapidjson::StringBuffer buffer;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
      doc.Accept(writer);
      response.send(Http::Code::Ok, buffer.GetString());
    }
    response.send(Http::Code::Ok, "MIME type is not application/json.\n");
  }

  void logPayloadJSON(const Rest::Request& request, Http::ResponseWriter response) {
    auto ct = request.headers().get<Http::Header::ContentType>();
    auto mime = ct->mime();
    std::string testResult;
    if(mime == MIME(Application, Json)) {
      rapidjson::Document doc;
      doc.Parse(request.body().c_str());
      std::string client = doc["client"].GetString();
      std::string session = doc["session"].GetString();
      testResult = "client: " + client + "\n";
      testResult += "session: " + session + "\n";
      testResult += "vertices\n";
      const rapidjson::Value& vertices = doc["vertices"];
      for (auto& vertex : vertices.GetArray()) {
        vertexType type;
        try {
          type = stringToVertexType(vertex["type"].GetString());
        } catch (std::string e) {
          response.send(Http::Code::Not_Implemented, e);
          return;
        }
        createVertex(db, type, vertex["name"].GetString(), vertex["start"].GetUint64(), vertex["end"].GetUint64());
        testResult = testResult + vertex["type"].GetString() + " "
                                + vertex["name"].GetString() + "\n";
      }
      testResult += "edges\n";
      const rapidjson::Value& edges = doc["edges"];
      for (auto& edge : edges.GetArray()) {
        edgeType type;
        try {
          type = stringToEdgeType(edge["type"].GetString());
        } catch (std::string e) {
          response.send(Http::Code::Not_Implemented, e);
          return;
        }
        createEdge(db, type, edge["first"].GetString(), edge["second"].GetString());
        testResult = testResult + edge["first"].GetString() + " "
                                + edge["type"].GetString() + " "
                                + edge["second"].GetString() + "\n";
      }
    }
    response.send(Http::Code::Ok, testResult);
  }

  void getAgents(const Rest::Request& request, Http::ResponseWriter response) {
    auto verts = getVertex(db, vertexType::Agent);
    std::string res = "";
    for(Vertex vert : verts) {
      res = res + vert.GetName() + "\n";
    }
    response.send(Http::Code::Ok, res);
  }

  void getActivities(const Rest::Request& request, Http::ResponseWriter response) {
    auto verts = getVertex(db, vertexType::Activity);
    std::string res = "";
    for(Vertex vert : verts) {
      res = res + vert.GetName() + "\n";
    }
    response.send(Http::Code::Ok, res);
  }

  void getEntities(const Rest::Request& request, Http::ResponseWriter response) {
    auto verts = getVertex(db, vertexType::Entity);
    std::string res = "";
    for(Vertex vert : verts) {
      res = res + vert.GetName() + "\n";
    }
    response.send(Http::Code::Ok, res);
  }
};
