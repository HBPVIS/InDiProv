#include "http.h"
#include "description.h"
#include "endpoint.h"
#include "rapidjson/rapidjson.h"

using namespace Net;

class LogService {
public:
  LogService(Net::Address addr)
    : httpEndpoint(std::make_shared<Net::Http::Endpoint>(addr))
    , desc("InDiProv Logging API", "0.1")
  { }

  void init(size_t thr = 2) {
    auto opts = Net::Http::Endpoint::options()
      .threads(thr)
      .flags(Net::Tcp::Options::InstallSignalHandler);
    httpEndpoint->init(opts);
    createDescription();
  }

  void start() {
    router.initFromDescription(desc);

    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
  }

  void shutdown() {
    httpEndpoint->shutdown();
  }

private:
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
  }

  void handleReady(const Rest::Request&, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "Absolutely\n");
  }
};
