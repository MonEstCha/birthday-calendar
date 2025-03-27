#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using namespace std;

bool ping_db(mongocxx::database db);

namespace ma{
    bool establish_database_connection();
    std::vector<std::string> get_birthday(const string date);
    bool insert_birthday(const std::string date, const std::string name);
}

