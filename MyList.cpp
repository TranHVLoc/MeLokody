/*
 * This program display the album of songs that user has added at Feel & Love
 * The album is a table extracted from MySQL that includes title, artist, genre,
 *  and most important, the embeded Youtube video so they can watch the MV directly
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
void showTable(mysqlpp::StoreQueryResult &result, const string firstName);

/* MAIN function */
int main(int argc, char *argv[]) {
    // Get a dictionary to store submitted information from previous repo (processLogin.cgi)
    map<string,string> Get;
    initializePost(Get);

    // Variables declaration
    std::string username;
    std::string firstName;
    std::string password;

    // Get user information sent from processLogin.cgi
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
            << "<link rel = \"stylesheet\" href = \"./css/style_list.css\" type=\"text/css\">"
            << "<meta charset=\"utf-8\">"
        << "</head>" << endl << endl;
    // BODY
    cout << "<body>" << endl;

    // Connect to database with: database, server, userID, password
    mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
    if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {

        // Create a query
        mysqlpp::Query query = conn.query();
        query << "SELECT * FROM tranlh4_user_" + username + ";";

        // Check the query for correctness
        query.parse();

        // Run the query and get the result
        mysqlpp::StoreQueryResult result = query.store();

        // Always check for errors
        if (result) {
            // Show the playlist
            showTable(result, firstName);
            cout << "<h3>You have a good taste, " << firstName << "!</h3>";
        } else {
            cerr << "Query failed: " << query.error() << endl;
        }

        // Back to Main Page button
        cout << "<form id=\"backToMain\" method=\"POST\" action=\"processLogin.cgi\">"
            << "<input type=\"hidden\" name=\"username\" value=\"" << username << "\">"
            << "<input type=\"hidden\" name=\"password\" value=\"" << password << "\">"
        << "</form>"
        << "<center><a href=\"javascript:{}\" onclick=\"document.getElementById(\'backToMain\').submit(); return false;\">"
            << "Back to Main Page"
        << "</a></center>";

        // End html
        cout << "</body></html>";
        
        return(0);
    } else {
        cerr << "Connection failed: " << conn.error() <<endl;
        return(1);
    }
}

/* Function to display table for user to enjoy their album */
void showTable(mysqlpp::StoreQueryResult &result, const string firstName) {
    cout << "<h1>My <i>MeLokody</i> List</h1>"
        // Table starts from here
        << "<div class=\"list\">"
            << "<h2>" << firstName << "\'s album" << "</h2>"
            << "<table>"
                << "<tbody>"
                    // Table headers
                    << "<tr>"
                        << "<th>Title</th>"
                        << "<th>Artist</th>"
                        << "<th>Genre</th>"
                        << "<th>URL</th>"
                    << "</tr>";
    
    // Display each song for each row
    for (size_t i = 0; i < result.num_rows(); ++i) {
        cout << "<tr>"
                << "<td>" << result[i]["title"] << "</td>"
                << "<td>" << result[i]["artist"] << "</td>"
                << "<td>" << result[i]["genre"] << "</td>"
                << "<td>"
                    << "<iframe width=\"490\" height=\"298\" src=\"" << result[i]["link"] << "\"></iframe>"
                << "</td>"
            << "</tr>" << endl;
    }
    
    // End table
    cout        << "</tbody>"
            << "</table>"
        << "</div>" << endl;
}
