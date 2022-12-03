/*
 * This program gets song information and add to database of a particular user before returning
 *  to the previous service page (FeelLove.cgi).
 * This program uses the MySQL++ wrapper for the MySQL C APIs.
 * It connects a CGI program to a MySQL database.
 *
 * Copyright 2022
 * Author: Loc Tran
 */

#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <iostream>

#include "getpost.h"

using namespace std;

int main(int argc, char *argv[]) {

    map<string,string> Get;
    initializePost(Get);

    string username;
    string password;
    string firstName;
    string title;
    string artist;
    string genre;
    string link;

    // Display the webpage
    cout << "Content-type: text/html\r\n\r\n";
    cout << "<html lang = \"en-US\">";
    
    // HEADER
    cout << "<head>"
            << "<meta charset=\"utf-8\">"
            << "<link rel = \"stylesheet\" href = \"./css/style_announcement.css\" type=\"text/css\">"
            << "<title>MeLokody</title>"
        << "</head>" << endl << endl;

    // BODY    
    cout << "<body>" << endl;

    // Get song information as well as user information sent from FeelLove.cgi
    if (Get.find("username") != Get.end() && Get.find("firstName") != Get.end() && Get.find("password") != Get.end()
        && Get.find("title") != Get.end() && Get.find("artist") != Get.end() && Get.find("genre") != Get.end()
        && Get.find("link") != Get.end()) {
        username = Get["username"];
        password = Get["password"];
        firstName = Get["firstName"];
        title = Get["title"];
        artist = Get["artist"];
        genre = Get["genre"];

        // Get the link but it must be re-formatted for Youtube embed compability first
        // E.g: https://www.youtube.com/watch?v=123345 --> https://www.youtube.com/embed/123345
        link = "https://www.youtube.com/embed/" + Get["link"].substr(Get["link"].find("watch?v=", 0) + 8);

        // Convert all alphabet letter to uppercase for genre
        transform(genre.begin(), genre.end(), genre.begin(), ::toupper);

        // Connect to database with: database, server, userID, password
        mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {

            // Create a query
            mysqlpp::Query query = conn.query();
            query << "INSERT into tranlh4_user_" + username + " (title, artist, genre, link) VALUES(%0q, %1q, %2q, %3q);";

            // Check the query for correctness
            query.parse();

            // Run the query -- note INSERT does not return results so we are using SimpleResult
            mysqlpp::SimpleResult result = query.execute(title, artist, genre, link);

            // Always check for errors
            if (result) {
                cout <<"<h3>Song information storing success...</h3>\n";
            } else {
                cout << "<h2 style = \"color: red;\">Query error: " << query.error() << "</h2>";
                cout << "<h3>Please try again!</h3>";
            }
        } else {
            cerr << "Connection failed: " << conn.error() << endl;
            return (1);
        }
    } else {
        cout << "\t\t<h1 style=\"color:red;\">Error!!!</h1>\n";
        cout << "\t\t<p>INVALID song information</p>\n"
            << "\t\t<p>Please try again and make sure you provide valid inputs for song's information</p>\n";
    }
    
    // Back to Song Page button
    cout << "\t\t<form id=\"backToFeelLove\" method=\"POST\" action=\"FeelLove.cgi\">\n"
        << "\t\t\t<input type=\"hidden\" name=\"firstName\" value=\"" << firstName << "\">\n"
        << "\t\t\t<input type=\"hidden\" name=\"username\" value=\"" << username << "\">\n"
        << "\t\t\t<input type=\"hidden\" name=\"password\" value=\"" << password << "\">\n"
        << "\t\t</form>\n"
        << "\t\t\t<center><a href=\"javascript:{}\" onclick=\"document.getElementById(\'backToFeelLove\').submit(); return false;\">"
        << "\t\t\tBack to Song Listen</a></center>\n";

    // End html
    cout << "</body></html>" << endl;

    return (0);
}
