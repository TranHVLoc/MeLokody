/*
 * This program process the authentification services that
 *  accept users to enter their account if username and password are valid
 *  and show the main service page the direct user to 3 fucntions as well as
 *  connect to their MySQL database for further experience purpose
 * This program uses the MySQL++ wrapper for the MySQL C APIs.
 * It connects a CGI program to a MySQL database.
 *
 * Copyright 2022
 * Author: Loc Tran
 */

/* Referenced from https://codepen.io/petegarvin1/pen/YzWBbRx */
/* Modify by Loc Tran for project building purpose */

#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "getpost.h"

using namespace std;

/* Fucntion declaration */
void loginPage(string firstName, string username, string password);

/* MAIN function */
int main(int argc, char *argv[]) {
    // Get a dictionary to store submitted information from previous repo
    map<string,string> Get;
    initializePost(Get);
    // Variables declaration
    std::string username;
    std::string password;

    // Start displaying webpage
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

    // Display information submitted from Login.html
    if (Get.find("username") != Get.end() && Get.find("password") != Get.end()) {
        username = Get["username"];
        password = Get["password"];

        // Connect to database with: database, server, userID, password
        mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {

            // Create a query
            mysqlpp::Query query = conn.query();
            query << "SELECT * FROM tranlh4_Users WHERE username = '" + username + "' AND password = md5('" + password + "');";

            // Check the query for correctness
            query.parse();

            // Run the query and get the result
            mysqlpp::StoreQueryResult result = query.store();

            // Always check for errors
            if (result) {
                // If user submit username and password successfully, then log them in
                if (result.num_rows() >= 1) {
                    std::string firstName = result[0]["firstName"].c_str();
                    // Proceed to Main MeLokody service page
                    loginPage(firstName, username, password);
                } else {    // Else, alert user and direct back to Login.html
                    cout << "<h2 style = \"color: red;\">!!! Login error !!!</h2>\n";
                    cout << "<h4>The login information might be invalid! Please try again.</h4>\n";
                    cout << "<br><a href = \"Login.html\">Back to Login</a>\n";
                }
            } else {
                cerr << "Query failed: " << query.error() << endl;
            }
            return(0);
        } else {
            cerr << "Connection failed: " << conn.error() << endl;
            return (1);
        }
    } else {
        cout << "<h1 style=\"color:red;\">Error!!!</h1>";
        cout << "<p>INVALID username or password</p>"
            << "<p>Please try login again and make sure to provide 2 valid inputs for username and password!</p>";
        cout << "<br><a href = \"Login.html\">Back to Login</a>";
    }

    cout << "</body></html>" << endl;
}

/* Display the layout of 3 services if user is authenticated successfully */
void loginPage(string firstName, string username, string password) {
    // Style the new layout if user log in successfully
    cout << "<link rel = \"stylesheet\" href = \"./css/style_login.css\" type=\"text/css\">";

    cout << "<h1>MeLokody</h1>"

        << "<h3>Welcome, <i>" << firstName << "</i>!</h3>"
        << "<p>Please choose any of these services and you won't regret it</p>"
        << "<p>Let's listen, feel, add, and match with other users for greatest experiences!</p>"

        // Each of card includes a hidden form to help pass user's information among CGI pages
        //  which can prevent error when going back to this file
        << "<div class=\"container\">"

            // Feel & Love
            << "<form id=\"1st_form\" method=\"POST\" action=\"FeelLove.cgi\">"
                << "<input type=\"hidden\" name=\"firstName\" value=\"" << firstName << "\">"
                << "<input type=\"hidden\" name=\"username\" value=\"" << username << "\">"
                << "<input type=\"hidden\" name=\"password\" value=\"" << password << "\">"
            << "</form>"
            << "<a href=\"javascript:{}\" onclick=\"document.getElementById(\'1st_form\').submit(); return false;\">"
                << "<div class=\"card card0\">"
                    << "<div class=\"border\">"
                        << "<h2>Feel & Love</h2>"
                    << "</div>"
                << "</div>"
            << "</a>"

            // Taste Check
            << "<form id=\"2nd_form\" method=\"POST\" action=\"TasteCheck.cgi\">"
                << "<input type=\"hidden\" name=\"firstName\" value=\"" << firstName << "\">"
                << "<input type=\"hidden\" name=\"username\" value=\"" << username << "\">"
                << "<input type=\"hidden\" name=\"password\" value=\"" << password << "\">"
            << "</form>"
            << "<a href=\"javascript:{}\" onclick=\"document.getElementById(\'2nd_form\').submit(); return false;\">"
                << "<div class=\"card card1\">"
                    << "<div class=\"border\">"
                        << "<h2>Taste Check</h2>"
                    << "</div>"
                << "</div>"
            << "</a>"

            // My List
            << "<form id=\"3rd_form\" method=\"POST\" action=\"MyList.cgi\">"
                << "<input type=\"hidden\" name=\"firstName\" value=\"" << firstName << "\">"
                << "<input type=\"hidden\" name=\"username\" value=\"" << username << "\">"
                << "<input type=\"hidden\" name=\"password\" value=\"" << password << "\">"
            << "</form>"
            << "<a href=\"javascript:{}\" onclick=\"document.getElementById(\'3rd_form\').submit(); return false;\">"
                << "<div class=\"card card2\">"
                    << "<div class=\"border\">"
                        << "<h2>My List</h2>"
                    << "</div>"
                << "</div>"
            << "</a>"

        << "</div>" << endl;
    
    // Log off button --> Login.html
    cout << "<a id=\"logout\" href = \"Login.html\">Log off</a>";
    
}
