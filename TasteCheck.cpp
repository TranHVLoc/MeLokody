/*
 * This program quickly reviews on the most favourite genre from current user
 *  and do another quick search on other users to check if there is anyone has
 *  the same taste with current user.
 * Then it will display all songs of that genre
 *  from those users as a playlist for each so that the current user can discover more
 *  of the favourite genre from others' sharing.
 * This program uses the MySQL++ wrapper for the MySQL C APIs.
 * It connects a CGI program to a MySQL database.
 *
 * Copyright 2022
 * Author: Loc Tran
 */

/* Referenced from https://codepen.io/kingzcancode0509/pen/Poaegyz */
/* Modify by Loc Tran for project building purpose */

#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "getpost.h"

using namespace std;

/* Function declaration */
void getTables(const string most_fre, const string firstName, const string username);
void findEachTable(mysqlpp::StoreQueryResult &result, const string most_fre, const string username);
void display(const string table_name, const string most_fre);
string getName(const string table_name);


/* MAIN function */
int main(int argc, char *argv[]) {
    // Get a dictionary to store submitted information from processLogin.cgi
    map<string,string> Get;
    initializePost(Get);

    // Variables declaration
    std::string username;
    std::string firstName;
    std::string password;

    // Get song information as well as user information sent from processLogin.cgi
    if (Get.find("username") != Get.end() && Get.find("firstName") != Get.end() && Get.find("password") != Get.end()) {
        username = Get["username"];
        firstName = Get["firstName"];
        password = Get["password"];
    }

    // Display the webpage
    cout << "Content-type: text/html\r\n\r\n";
    cout << "<html lang = \"en-US\">";

    // HEADER
    cout << "<head>"
            << "<title>MeLokody</title>"
            << "<link rel = \"stylesheet\" href = \"./css/style_taste.css\" type=\"text/css\">"
            << "<meta charset=\"utf-8\">"
        << "</head>" << endl << endl;
    // BODY
    cout << "<body>" << endl;

    // Connect to database with: database, server, userID, password
    mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
    if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {

        // Create a query
        mysqlpp::Query query = conn.query();
        // Get command for query
        query << "SELECT genre, COUNT(genre) as occurrences from tranlh4_user_" << username
                << " GROUP BY genre ORDER BY occurrences DESC LIMIT 1;";

        // Check the query for correctness
        query.parse();

        // Run the query and get the result
        mysqlpp::StoreQueryResult result = query.store();

        // Always check for errors
        if (result) {
            // Most favourite song
            string most_fre = result[0]["genre"].c_str();
            // Call other albums
            getTables(most_fre, firstName, username);
        } else {
            cout << "<h3>Query failed: " << query.error() << "</h3>" << endl;
        }

        // Back to Main Page button
        cout << "<form id=\"backToMain\" method=\"POST\" action=\"processLogin.cgi\">"
                << "<input type=\"hidden\" name=\"username\" value=\"" << username << "\">"
                << "<input type=\"hidden\" name=\"password\" value=\"" << password << "\">"
            << "</form>\n"
            << "<center><a href=\"#\" onclick=\"document.getElementById(\'backToMain\').submit(); return false;\">"
                << "Back to Main Page"
            << "</a></center>";

        // End html
        cout << "</body></html>";
        
        return(0);
    } else {
        cout << "<h3>Connection failed: " << conn.error() << "</h3>" << endl;
        return(1);
    }
}

/* Function to check from other MeLokody albums and check favourite song for each album */
void getTables(const string most_fre, const string firstName, const string username) {
    // Start the display and let user know their favourite music genre
    cout << "<h1><i>MeLokody</i> Taste Check</h1>"
        << "<h3>" << firstName << ", seems like you are a big fan of " << most_fre << "!</h3>";

    // Connect to database with: database, server, userID, password
    mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
    if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
        // Create a query
        mysqlpp::Query query = conn.query();
        // Get command for query
        query << "SHOW tables LIKE 'tranlh4_user_%';";

        // Check the query for correctness
        query.parse();

        // Run the query and get the result
        mysqlpp::StoreQueryResult result = query.store();

        // Always check for result
        if (result) {
            cout << "<h4>We have found some users that you can share the beat with.</h4>"
                << "<h4>Let's join the club!!!</h4>";
            // Check each table for matching
            findEachTable(result, most_fre, username);
        } else {
            cout << "<h3>However, there is likely no other user"
                << " recorded to have the same taste like you :( </h3>" << endl;
        }
    } else {
        cout << "<h3>Connection failed: " << conn.error() << "</h3>" << endl;
    }

}

/* Function to find most favourite song of each table */
void findEachTable(mysqlpp::StoreQueryResult &result, const string most_fre, const string username) {
    // Start displaying tables
    cout << "<div class=\"list\">";
    // If match, then turn this to true
    bool found = false;

    // For each album...
    for (size_t i = 0; i < result.num_rows(); ++i) {
        // Connect to database with: database, server, userID, password
        mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            // Create a query
            mysqlpp::Query query = conn.query();
            // Get command for query
            string table_name = result[i]["Tables_in_cse278 (tranlh4_user_%)"].c_str();
            
            // If the current table belongs to current user, skip it
            if (table_name == ("tranlh4_user_" + username)) {
                continue;
            }

            // Get the query design
            query << "SELECT genre, COUNT(genre) as occurrences FROM " << table_name
                << " GROUP BY genre ORDER BY occurrences DESC LIMIT 1;";

            // Check the query for correctness
            query.parse();

            // Run the query and get the result
            mysqlpp::StoreQueryResult output = query.store();

            // Always check for errors
            if (output) {
                // Get the most favorite genre of current table
                string frequent_value = output[0]["genre"].c_str();
                // Check the match with current user's
                if (frequent_value.compare(most_fre) == 0) {
                    found = true;
                    // Show the table
                    display(table_name, most_fre);
                }
            } else {
                cout << "<h3>NO database found!</h3>";
            }
        } else {
            cout << "<h3>Connection failed: " << conn.error() << "</h3>" << endl;
        }
    }

    // If not match is found, notify user :(
    if (!found) {
        cout << "<h3>Oopps, no match found :(</h3>";
    }

    cout << "</div>" << endl;
}

/* Function to display the album for each music sharing user */
void display(const string table_name, const string most_fre) {
    // Connect to database with: database, server, userID, password
    mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
    if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
        // Create a query
        mysqlpp::Query query = conn.query();

        // Design the query
        query << "SELECT * FROM " << table_name << " WHERE genre = '" + most_fre + "';";

        // Check the query for correctness
        query.parse();

        // Run the query and get the result
        mysqlpp::StoreQueryResult result = query.store();

        // Always check for error
        if (result) {
            // And display the album
            cout << "<h2>" << getName(table_name) << "</h2>";
            cout << "<table>"
                << "<tbody>"
                    // Table header
                    << "<tr>"
                        << "<th>Title</th>"
                        << "<th>Artist</th>"
                        << "<th>Genre</th>"
                        << "<th>URL</th>"
                    << "</tr>";

            // Table rows
            for (size_t i = 0; i < result.num_rows(); ++i) {
                cout << "<tr>"
                        << "<td>" << result[i]["title"] << "</td>"
                        << "<td>" << result[i]["artist"] << "</td>"
                        << "<td>" << result[i]["genre"] << "</td>"
                        << "<td>"
                            << "<a id = \"url\" target=\"_blank\" href=\"" << result[i]["link"].c_str() << "\">Watch Here</a>"
                        << "</td>"
                    << "</tr>";
            }
            // End table
            cout << "</tbody>"
                << "</table>" << endl;
        } else {
            cout << "<h3>Query failed: " << query.error() << "</h3>" << endl;
        }
    } else {
        cerr << "Connection failed: " << conn.error() << endl;
    }
}

/* Function to get the full name of each music sharing user */
string getName(const string table_name) {
    string username = table_name.substr(13);
    // Connect to database with: database, server, userID, password
    mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
    if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {

        // Create a query
        mysqlpp::Query query = conn.query();
        query << "SELECT firstName, lastName FROM tranlh4_Users WHERE username = '" + username + "';";

        // Check the query for correctness
        query.parse();

        // Run the query and get the result
        mysqlpp::StoreQueryResult result = query.store();

        // Always check for error
        if (result) {
            // Get first name and last name
            string firstName = result[0]["firstName"].c_str();
            string lastName = result[0]["lastName"].c_str();
            return firstName + " " + lastName;
        } else {
            return "N/A";
        }
    } else {
        cerr << "Connection failed: " << conn.error() << endl;
    }
}
