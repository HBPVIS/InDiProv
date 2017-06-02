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
      .route(desc.get("/ready"))
      .bind(&LogService::handleReady, this)
      .response(Http::Code::Ok, "'Absolutely.' if server is ready");

    desc
      .route(desc.post("/prettify"))
      .bind(&LogService::prettifyJSON, this)
      .response(Http::Code::Ok, "Prettified 'application/json' data.");

    auto agentPath = desc.path("/agent");
    auto activityPath = desc.path("/activity");
    auto entityPath = desc.path("/entity");

    agentPath
      .route(desc.post("/:name"), "Create an Agent")
      .bind(&LogService::createAgent, this)
      .parameter<Rest::Type::String>("name", "The name of the agent to create");

    desc
      .route(desc.get("/agents"))
      .bind(&LogService::getAgents, this);

    activityPath
      .route(desc.post("/:name/:from/:to"), "Create an Activity")
      .bind(&LogService::createActivity, this)
      .parameter<Rest::Type::String>("name", "The name of the activity to create");

    desc
      .route(desc.get("/activities"))
      .bind(&LogService::getActivities, this);

    entityPath
      .route(desc.post("/:name"), "Create an Entity")
      .bind(&LogService::createEntity, this)
      .parameter<Rest::Type::String>("name", "The name of the entity to create");

    desc
      .route(desc.get("/entities"))
      .bind(&LogService::getEntities, this);
  }

  void handleReady(const Rest::Request&, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "Absolutely.\n");
  }

  void prettifyJSON(const Rest::Request& request, Http::ResponseWriter response) {
    auto headers = request.headers();
    auto ct = request.headers().get<Http::Header::ContentType>();
    auto mime = ct->mime();
    if(mime == MIME(Application, Json)) {
      rapidjson::Document d;
      const char* json = request.body().c_str();
      d.Parse(json);
      rapidjson::StringBuffer buffer;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
      d.Accept(writer);
      response.send(Http::Code::Ok, buffer.GetString());
    }
    response.send(Http::Code::Ok, "MIME type is not application/json.\n");
  }

  void createAgent(const Rest::Request& request, Http::ResponseWriter response) {
    std::string name = request.param(":name").as<std::string>();
    auto t = time(nullptr);
    createVertex(db, Agent, name, t, t);
    response.send(Http::Code::Ok, name + " created.\n");
  }

  void createActivity(const Rest::Request& request, Http::ResponseWriter response) {
    std::string name = request.param(":name").as<std::string>();
    auto from = request.param(":from").as<uint>();
    auto to = request.param(":to").as<uint>();
    createVertex(db, Activity, name, from, to);
    response.send(Http::Code::Ok, name + " created.\n");
  }

  void createEntity(const Rest::Request& request, Http::ResponseWriter response) {
    std::string name = request.param(":name").as<std::string>();
    auto t = time(nullptr);
    createVertex(db, Entity, name, t, t);
    response.send(Http::Code::Ok, name + " created.\n");
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
