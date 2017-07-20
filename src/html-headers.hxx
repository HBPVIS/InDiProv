#pragma once
#include "http.h"

namespace Net {

namespace Http {

namespace Header {

class AccessControlAllowOrigin : public Header {
public:
    NAME("Access-Control-Allow-Origin")

    AccessControlAllowOrigin() { }

    explicit AccessControlAllowOrigin(const std::string& uri) {
        uri_ = uri;
    }

    void parse(const std::string& data) { }

    void write(std::ostream& os) const {
        os << uri_;
    }

    std::string uri() const { return uri_; }

private:
    std::string uri_;
};

}
}
}
