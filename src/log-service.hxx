#include "http.h"
#include "description.h"
#include "endpoint.h"
#include "serializer/rapidjson.h"

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
      .response(Http::Code::Ok, "Response to the /ready call");

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

    entityPath
      .route(desc.post("/:name"), "Create an Entity")
      .bind(&LogService::createEntity, this)
      .parameter<Rest::Type::String>("name", "The name of the entity to create");
  }

  void handleReady(const Rest::Request&, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "Absolutely\n");
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
};
