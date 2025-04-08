#include <cstdlib>

#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "database_control.h"


namespace ma {
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

// The mongocxx::instance constructor and destructor initialize and shut down the driver,
// respectively. Therefore, a mongocxx::instance must be created before using the driver and
// must remain alive for as long as the driver is in use.
//
// This example demonstrates how one might keep a heap allocated mongocxx::instance and
// mongocxx::pool object associated in a way that allows dynamic configuration. Most of the examples
// simply create a stack allocated mongocxx::instance object, which is fine for small examples, but
// real programs will typically require shared access to a commonly configured instance and
// connection pool across different translation units and components. By providing a singleton which
// owns the instance and pool objects, we can defer configuration until we are ready to use MongoDB,
// and share the instance and pool objects between multiple functions.

class mongo_access {
   public:
    static mongo_access& instance() {
        static mongo_access instance;
        return instance;
    }

    void configure(std::unique_ptr<mongocxx::instance> instance, std::unique_ptr<mongocxx::pool> pool) {
        _instance = std::move(instance);
        _pool = std::move(pool);
    }

    using connection = mongocxx::pool::entry;

    connection get_connection() {
        return _pool->acquire();
    }

   private:
    mongo_access() = default;

    std::unique_ptr<mongocxx::instance> _instance = nullptr;
    std::unique_ptr<mongocxx::pool> _pool = nullptr;
};


// The 'configure' function uses the same mongocxx::instance and mongocxx::pool
// objects by way of the mongo_access singleton.

void configure(mongocxx::uri myUri) {
    class noop_logger : public mongocxx::logger {
       public:
        void operator()(mongocxx::log_level, bsoncxx::stdx::string_view, bsoncxx::stdx::string_view) noexcept override {
        }
    };

    // Use `std::make_unique` with C++14 and newer.
    auto instance =
        std::unique_ptr<mongocxx::instance>(new mongocxx::instance(std::unique_ptr<noop_logger>(new noop_logger())));

    // Use `std::make_unique` with C++14 and newer.
    auto pool = std::unique_ptr<mongocxx::pool>(new mongocxx::pool(std::move(myUri)));

    mongo_access::instance().configure(std::move(instance), std::move(pool));
}

bool establish_database_connection()
{
    if (const char *mongodb_uri = std::getenv("MONGO_DB_URI"))
    {
        auto uri = mongocxx::uri{mongodb_uri};
        ma::configure(std::move(uri));
        return true;
    }
    return false;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

std::vector<std::string> get_birthday(const std::string date)
{
    auto dateComp = split(date, "-");
    std::string day = "day", month = "month";

    std::vector<std::string> result{};

    if(dateComp.size() == 3){ 
        auto connection = ma::mongo_access::instance().get_connection();
        auto collection = connection["knowledge_app"]["birthdays"];
        mongocxx::options::find opts{};
        opts.projection(make_document(kvp("_id", 0), kvp("name", 1)));
        auto cursor = collection.find(make_document(kvp(month, dateComp[1]), kvp(day, dateComp[2])), opts);
        for(auto&& doc : cursor) {
            bsoncxx::document::element name = doc["name"];
            auto val = doc["name"].get_string().value;
            result.push_back(std::string(val));
        }
    }
    return result;

}

bsoncxx::v_noabi::document::view_or_value make_birthday_doc(const std::string date, const std::string name){
    auto dateComp = split(date, "-");
    std::string day = "day", month = "month", year = "year";
    bsoncxx::v_noabi::document::view_or_value doc;
    if(dateComp.size() == 3)
        doc = make_document(kvp("name", name), kvp(month, dateComp[1]), kvp(day, dateComp[2]), kvp(year, dateComp[0]));
    return doc;

}

template<typename document, typename collection>
bool doc_exist(document doc, collection coll){
    auto result = coll.find_one(doc);
    return result.has_value();
}

bool insert_birthday(const std::string date, const std::string name)
{
    auto connection = ma::mongo_access::instance().get_connection();
    auto collection = connection["knowledge_app"]["birthdays"];

    std::vector<std::string> result{};
    bsoncxx::v_noabi::document::view_or_value doc = make_birthday_doc(date, name);

    if(!doc_exist(doc, collection)){ 
        collection.insert_one(doc);
        return true;
    }
    return false;

}


} // namespace ma


