/*
 * This program display the information that users have entered and store it
 *  in MySQL database for further authentification purpose
 * This program uses the MySQL++ wrapper for the MySQL C APIs.
 * It connects a CGI program to a MySQL database.
 *
 * Copyright 2022
 * Author: Loc Tran
 */

#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <string>
#include <iostream>

#include "getpost.h"

using namespace std;


/* Function declaration */
void makeTable(string username);


/* MAIN function */
int main(int argc, char* argv[]) {
    // Get a dictionary to store submitted information from previous repo
    map<string,string> Get;
    initializePost(Get);

    // Start displaying the webpage
    cout << "Content-type: text/html\r\n\r\n";
    cout << "<html lang = \"en-US\">" << endl;

    // HEADER
    cout << "<head>"
            << "<meta charset=\"utf-8\">"
            << "<link rel = \"stylesheet\" href = \"./css/style_announcement.css\" type=\"text/css\">"
            << "<title>MeLokody</title>"
        << "</head>" << endl << endl;

    // BODY
    cout << "<body>"
            << "<h1>Sign-up information</h1>"
            << "<h3>--- Please take note again these pieces of information that you have used signing-up ---</h3>" << endl;

    // Declare variables
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string username;
    std::string password;

    // Display information submitted from Signup.html
    if (Get.find("username") != Get.end() && Get.find("password") != Get.end()
        && Get.find("firstName") != Get.end() && Get.find("lastName") != Get.end()) {
        
        // Dipslay information for user to review before turning back to Login.html
        cout << "<div class=\"information\">"
                << "<h3 id=\"fullname\">FULLNAME</h3>"  // Fullname (firstName + lastName)
                << "<p>" << Get["firstName"] << " " << Get["lastName"] << "</p>"
                << "<h3 id=\"email\">EMAIL</h3>"    // Email
                << "<p>" << Get["email"] << "</p>"
                << "<h3 id=\"username\">USERNAME</h3>"  // Username
                << "<p>" << Get["username"] << "</p>"
                << "<h3 id=\"password\">PASSWORD</h3>"  // Password
                << "<p>" << Get["password"] << "</p>"
            << "</div>" << endl << endl;
        
        // Initialize variables
        firstName = Get["firstName"];
        lastName = Get["lastName"];
        email = Get["email"];
        username = Get["username"];
        password = Get["password"];

        // Connect to database with: database, server, userID, password
        mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            // Create a query
            mysqlpp::Query query = conn.query();

            // Design the query to MySQL
            query << "INSERT INTO tranlh4_Users (firstName, lastName, Email, username, password) VALUES (%0q, %1q, %2q, %3q, md5(%4q));";

            // Check to ensure it is correct
            query.parse();

            // Run the query -- note INSERT does not return results so we are using SimpleResult
            mysqlpp::SimpleResult result = query.execute(firstName, lastName, email, username, password);

            // Always check for errors
            if (result) {
                cout <<"<h3>Information storing success...</h3>";
                // Create MeLokody database for that user
                makeTable(username);
            } else {
                cout << "<h3 style=\"color: red;\">Query error: " << query.error() << "</h3>";
            }

        } else {
            cerr << "Connection failed: " << conn.error() << endl;
            return (1);
        }

    } else {
        cout << "<p>Fill out the from and press submit</p>" << endl;
    }

    // Back to Login.html
    cout << "<div class=\"return\">"
            << "<a href=\"Login.html\">"
                << "Back to Login"
            << "</a>"
        << "</div>" << endl;

    cout << "</body></html>" << endl;

    return (0);
}


/* Function to make a table for each registerd account */
void makeTable (string username) {
    // Connect to database with: database, server, userID, password
    mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
    if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
        // Create a query
        mysqlpp::Query query = conn.query();

        // Design the query to MySQL
        query << "CREATE TABLE tranlh4_user_" + username
                + " (title VARCHAR(100) NOT NULL, artist VARCHAR(100), genre VARCHAR(30), link VARCHAR(2083) NOT NULL);";

        // Check to ensure it is correct
        query.parse();

        // Run the query -- note INSERT does not return results so we are using SimpleResult
        mysqlpp::SimpleResult result = query.execute();

        // Always check for errors
        if (result) {
            cout <<"<h3>Account database created...</h3>\n";
        } else {
            cout << "<h3 style=\"color: red;\">Query error: " << query.error() << "</h3>\n";
        }
    } else {
        cerr << "Connection failed: " << conn.error() << endl;
        return;
    }
}
