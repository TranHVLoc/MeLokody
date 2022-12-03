/*
 * This program brings users to experience trendy music playlist from Spotify
 *  and pop up a form to ask them if they want to add any of song to their own album
 * User and open and close the form at will. Any added song with data will be stored
 *  in a MySQL table as a music album specified for only that user to access it.
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
#include <iostream>

#include "getpost.h"

using namespace std;

/* Function declaration */
void divPop(const string firstName, const string username, const string password);


/* MAIN function */
int main(int argc, char *argv[]) {
    // Get a dictionary to store submitted information from previous repo (processLogin.cgi)
    map<string,string> Get;
    initializePost(Get);

    // Variables declarations
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
            << "<link rel = \"stylesheet\" href = \"./css/style_feel.css\" type=\"text/css\">"
            << "<meta charset=\"utf-8\">"
        << "</head>" << endl << endl;
    // BODY
    cout << "<body>" << endl;
    
    cout << "<h1>Feel The <i>MeLokody</i></h1>"
        << "<h3>Please listen to this trendy playlist on Spotify</h3>"

        << "<iframe src=\"https://open.spotify.com/embed/playlist/37i9dQZF1DXbYM3nMM0oPk?utm_source=generator\""
        << "width=\"70%\" height=\"580\" frameBorder=\"0\" allowfullscreen=\"\" "
        << "allow=\"autoplay; clipboard-write; encrypted-media; fullscreen; picture-in-picture\"></iframe>" << endl;
    
    // Call a form popped up if "Add to List" button is clicked
    divPop(firstName, username, password);

    cout << "<h3>Click here if you want to save the beat!!</h3>";
    // The above div only shows up if user click this button...
    cout << "<center><a href=\"#\" id=\"contact\" class=\"add-song\">Add Song</a></center><br>";

    // Back to Main Page button
    cout << "<form id=\"backToMain\" method=\"POST\" action=\"processLogin.cgi\">"
        << "<input type=\"hidden\" name=\"username\" value=\"" << username << "\">"
        << "<input type=\"hidden\" name=\"password\" value=\"" << password << "\">"
        << "</form>"
        << "<center><a href=\"javascript:{}\" onclick=\"document.getElementById(\'backToMain\').submit(); return false;\">"
        << "Back to Main Page</a></center>";

    // Link to "popup.js" to format song form popping up
    cout << "<script src=\'http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js\'></script>";
    cout << "<script src=\"popup.js\"></script>";

    // End html
    cout << "</body></html>" << endl;
}

/* Function to display the song information form if user clicks on "Add to List" button */
void divPop(const string firstName, const string username, const string password) {
    // Add song to list
    // This div will pop up if user clicks on button "Add Song"
    cout << "<div class=\"addToList contact\">"

            << "<h2>Song information</h2>"
            // Close div button
            << "<a href=\"\" class=\"cd-popup-close cd-close-button\">X</a>"

            // Add song form
            << "<form action=\"GetSong.cgi\" method=\"POST\" id=\"my_form\">"
                // Name of the song
                << "<div class=\"song-box\">"
                    << "<input type=\"text\" name=\"title\" id=\"song-box\" required>"
                    << "<label for=\"title\">Title</label>"
                << "</div>"

                // Artist
                << "<div class=\"song-box\">"
                    << "<input type=\"text\" name=\"artist\" id=\"song-box\" required>"
                    << "<label for=\"artist\">Artist</label>"
                << "</div>"

                // Genre
                << "<div class=\"song-box\">"
                    << "<input type=\"text\" name=\"genre\" id=\"song-box\" required>"
                    << "<label for=\"genre\">Genre</label>"
                << "</div>"

                // Youtube URL
                // This section only works for Youtube link
                // If user pastes other link, then will then cannot see the embeded video in My List
                << "<div class=\"song-box\">"
                    << "<input type=\"url\" name=\"link\" id=\"song-box\" required>"
                    << "<label for=\"link\">Youtube URL (Link only works for Youtube)</label>"
                << "</div>"

                // Hidden input to combine user's info and direct to GetSong.cgi
                // if they add a song and hit "Add to List"
                // This is for keeping user still in the account purpose
                << "<input type=\"hidden\" name=\"firstName\" value=\"" << firstName << "\">\n"
                << "<input type=\"hidden\" name=\"username\" value=\"" << username << "\">\n"
                << "<input type=\"hidden\" name=\"password\" value=\"" << password << "\">\n"
                // Add to List button
                << "<a href=\"#\" onclick=\"document.getElementById(\'my_form\').submit(); return false;\">"
                << "Add to list"
                << "</a>"
            << "</form>"
        << "</div>";
}
