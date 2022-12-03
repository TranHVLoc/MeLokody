# Make file to automate compiling process and server-client execution

# Compile all .cpp files
all:
	make signup
	make login
	make feellove
	make tastecheck
	make mylist
	make getsong

# Process Signup
signup: getpost.o
	g++ -Wall -o processSignup.cgi processSignup.cpp -lmysqlpp getpost.o

# Process Login
login: getpost.o
	g++ -Wall -o processLogin.cgi processLogin.cpp -lmysqlpp getpost.o

# 1st feature
feellove: getpost.o
	g++ -Wall -o FeelLove.cgi FeelLove.cpp -lmysqlpp getpost.o

getsong: getpost.o
	g++ -Wall -o GetSong.cgi GetSong.cpp -lmysqlpp getpost.o

# 2nd feature
tastecheck: getpost.o
	g++ -Wall -o TasteCheck.cgi TasteCheck.cpp -lmysqlpp getpost.o

# 3rd feature
mylist: getpost.o
	g++ -Wall -o MyList.cgi MyList.cpp -lmysqlpp getpost.o

# Method compile
getpost.o: getpost.cpp getpost.h
	g++ -Wall -c getpost.cpp


# Copy .cgi files to public_html
copy:
	cp processSignup.cgi processLogin.cgi FeelLove.cgi GetSong.cgi MyList.cgi TasteCheck.cgi ~/public_html

# Clean all .cgi files from assignment4 repo
clean:
	rm ~/assignment4/processLogin.cgi
	rm ~/assignment4/processSignup.cgi
	rm ~/assignment4/FeelLove.cgi
	rm ~/assignment4/GetSong.cgi
	rm ~/assignment4/MyList.cgi
	rm ~/assignment4/TasteCheck.cgi

# Clean all .cgi files from public_html repo
clean-public:
	rm ~/public_html/processSignup.cgi
	rm ~/public_html/processLogin.cgi
	rm ~/public_html/FeelLove.cgi
	rm ~/public_html/GetSong.cgi
	rm ~/public_html/MyList.cgi
	rm ~/public_html/TasteCheck.cgi