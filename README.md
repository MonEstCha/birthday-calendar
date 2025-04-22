# Birthday Calendar
An interactive calendar app displaying the birthday celebrants at the selected date.<br>
The birthday calendar integrates the wxwidgets framework with the MongoDB database, which stores the names and birth dates of the celebrants.
Therefore, following prerequisites need to be satisfied to use the app:

- The mongo-cxx-driver has been installed on your system.
- The mongo-c-driver has been installed on your system.
- A connection to the database has been established and the connection string stored in an environment variable named MONGO_DB_URI.
  
See [MongoDB - Getting Started](https://www.mongodb.com/docs/languages/cpp/cpp-driver/current/get-started/) for instructions on how to install the drivers and obtain a connection string.

## Credits
The birthday calendar is an adaptation of the wxwidgets sample calendar app. The database integration follows the MongoDB example on how to provide shared access to a commonly configured instance.
