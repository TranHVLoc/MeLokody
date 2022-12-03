# I. Introduction

This is the CSE 278 Assignment 4 description.  

The project is a fully functional web application that supports music playlist creation for user and music streaming via embeded Spotify and Youtube.
Features in the project are:

A. Authentification
  1. *Sign-up* 
  2. *Login*

B. Music services
  1. *Feel & Love*
  2. *Taste Check*
  3. *My List*

## A. Authentification

### 1. Sign-up

At Sign-up, users are required to provide their *first name*, *last name*, *email address*, *username* (must be unique), and password respectively. After providing the information, the program will save it into **MySQL** table called `tranlh4_Users`. In the mean time, a new MySQL table is also created in the format `tranlh4_user_` + `username`. This newly created table is the database for their own music playlist so that any new song with reference can be stored in it.

For example, if the useer's username is *Miami*, then the table will be `tranlh4_user_Miami`. This table has 4 columns, namely *title*, *artist*, *genre*, and *link*. At **link** column, the only allowed format is Youtube link as the program will have to process the provided link and store it in a new form that is compatible with HTML Embed feature.

### 2. Login

After finishing signing up, the user can go back to `Login.html` and access the account. The program can then identify which user bases on **unique** username, extract the information, and act exactly like using a regular account. All of these will be display on `processLogin.cgi`, compiled from `processLogin.cpp` with `mysqlpp` library if user enters the credentials correctly. Otherwise, it will announce a Login Error and prompt user get back to `Login.html`.

## B. Music Services

### 1. Feel & Love

The service is shown in `FeelLove.cgi`. This is the service where user can listen to music throughout an embedded Spotify playlist. Below is a button which user can add a song to their album by providing the title, artist, genre, and youtube link related to the song. After adding the song, they will be directed to a process page (`GetSong.cgi`) to check for query input. If the song has been added to the playlist, they will be notified and can go back to Feel & Love page.

***NOTE:*** When add the music link, remember to add ONLY youtube link with format `https://www.youtube.com/watch?v=<videoID>`. That way, user can see the embedded youtube video on *My List* service.

### 2. My List

The service is shown in `MyList.cgi`. This is the service where user can view the music album that they have added so far. The album is displayed as a table like MySQL table yet more beautiful design as well as the link will work like a video thumbnail, which is big enough for user to watch multiple videos simultaneously.

### 3. Taste Check

The service is shown in `TasteCheck.cgi`. This might be the most interesting part of the website as the program will preview the whole list of current user and determine which music genre is their favourite. Then, the program will scan through other users' album who also have the same music taste as the current one (having the same favourite genre). For each matched user with the current customer, the program will project their table including the songs of that genre. That way, the current user can discover more of their favourite music genre from listening to the community's shared music.


# II. Quick Start Guide

Everything to execute the program has been set up. Please review the `makefile` file to see the comment.

To compile all `.cpp` files to `.cgi` programs, use `make all` on the terminal window. Then copy all `.cgi` files to *website folder* (e.g `public_html`) using `make copy`.

To asccess the landing page, click [here](http://os1.csi.miamioh.edu/~tranlh4/)

To access the login page, click [here](http://os1.csi.miamioh.edu/~tranlh4/Login.html)


# III. Technology

The web application use *Three-tier architecture* with Front-end, Back-end, and Database.

* For front-end, the project has HTML, CSS, and JavaScrip to for design and interaction. The web structure with HTML tags are be included in `.cpp` files, styles are included in `.css` files, and function interraction are inluded in `popup.js`, which is used for Feel & Love feature to open song add form.
* For back-end, the CGI programs is used with MySQL++ library plugin. All the code for connecting to MySQL and web browser must be written in **C++**. For any `.cpp ` file that receives data submitted from previous file, it must also include `getpost.o`, which is compiled from `getpost.cpp` and `getpost.h` to process the submitted data.
* For database, MySQL is responsible for storing user information and is also responsible for creating playlist under table form for each account created

See the structure of **MeLokody** here:

![architecture](https://github.com/cse278abcd-assignments/assignment4-tranlh4/blob/main/architecture.png)
