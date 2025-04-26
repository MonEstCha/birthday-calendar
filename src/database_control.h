#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>


bool ping_db(mongocxx::database db);

namespace ma{
    bool establish_database_connection();
    std::vector<std::string> get_birthday(const std::string date);
    bool insert_birthday(const std::string date, const std::string name);
    bool remove_birthday(const std::string name);
}

