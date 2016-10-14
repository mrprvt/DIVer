/*
  ______________________________________________
 |                                              |
 |                  :: DIVER ::                 |
 |            the HTML/CSS generator            |
 |                                              |
 |              version 0.4.2 Alpha             |
 |                                              |
 |______________________________________________|

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ncurses.h>
#include <time.h>

    using namespace std;

struct data{                                                                                                    // DONE

    string date = "DATE";
    string topic = "TOPIC";
    string content = "CONTENT";
    string diverSymbol = "<!-- DIVER_INIT -->";
};

void testNewCurses(int color){

    initscr();

    start_color();

    init_pair( 1, COLOR_RED, COLOR_RED );
    init_pair( 2, COLOR_BLUE, COLOR_BLUE);

    attron(COLOR_PAIR(color));
    //for (int i = 0; i < 500; i++)printw((" "));

    attroff(COLOR_PAIR(color));

    echo();

    getch();
    getch();
    getch();

    endwin();
}

void functionInfo(bool showInfo, string name, char startOrEnd = 's'){                                           // DONE

    // Show name and status of launched function

     if(showInfo){

        char stCh[2] = {'='};

        cout << "\n";

        for(int i = 0; i < 60; i++) cout << stCh[0];

        cout << "\n";

        if(startOrEnd == 's') cout << "function.info: " << name << "() is running. ++\n";
        if(startOrEnd == 'e') cout << "function.info: " << name << "() has ended. --\n";

        for(int i = 0; i < 60; i++) cout << stCh[0];

        cout << "\n";
    }
}

void numberVievAdjusting(int number){                                                                           // DONE

    // Co-function of showContent(), and generate adjusted line number in universal fixed table who is printed before line's content, for example

    // (output) : 10000 |

    unsigned int howManySpaces;

    if(number < 10){

        howManySpaces = 3;

    }else if(number < 100){

        howManySpaces = 2;

    }else if(number < 1000){

        howManySpaces = 1;

    }else if(number < 10000){

        howManySpaces = 0;
    }

    cout << ": " << number;

    for(int i = 0; i < howManySpaces; i++) cout << " ";

    cout << " | ";
}

bool checkIsExist(string filePath, bool showInfo = false){                                                      // DONE

    // Search for file and return does exist

    functionInfo(showInfo, "checkIsExitst");

    fstream textFile;
    textFile.open(filePath.c_str());

    bool bufor;

    cout << endl;
    cout << "Searching for: " << filePath << endl;

    if(textFile.is_open() == true){

        if(showInfo) cout << "file.status: good\n";
        bufor = true;

    }else{

        if(showInfo) cout << "file.status: failed \n" << "The file is missing or has broken \n";
        bufor = false;
    }

    textFile.close();

    functionInfo(showInfo, "checkIsExitst", 'e');

    return bufor;
}

void showContent(string filePath, bool showInfo = false){                                                       // DONE

    // Copy all content from file and put into output

    functionInfo(showInfo, "showContent");

    if(checkIsExist(filePath, showInfo) == true){

        fstream textFile;
        string content = "";
        bool endLoop = false;
        int line = 0;

        textFile.open(filePath.c_str());

        cout << endl;

        for(int i = 0; i < 60; i++) cout << "#";

        cout << endl;

            do{

                if(textFile.eof() == false){

                    line++;
                    getline(textFile, content);
                    numberVievAdjusting(line);
                    cout <<  content << endl;

                }else endLoop = true;

            }while(endLoop == false);

            textFile.close();

        for(int i = 0; i < 60; i++) cout << "#";

        cout << "\n";

    }else cout << "Missing file in showContent() \n";

    functionInfo(showInfo, "showContent",'e');
}

int findInFile(string filePath, string keyWord, bool mode, bool showInfo = false){                              // DONE

    // Simple parser, it return position of keyword when exist in file (before or after keyword, it can be simply changed by 'mode' argument), or '-1' value when isn't

    functionInfo(showInfo, "findInFile");

    if(checkIsExist(filePath.c_str(), showInfo) == true){

    fstream textFile;
    textFile.open(filePath.c_str());

    string downloadedWord;

    int positionInFileAK;
    int positionInFileBK;
    int lineInFile = 0;

    bool endLoop = false;
    bool isFound = false;

    cout << "Parser is looking for: " << keyWord << endl;

    do{

        if(textFile.eof() == false){

            lineInFile++;
            positionInFileBK = textFile.tellg(); // BK means "Before Keyword"

            getline(textFile, downloadedWord);

            if(showInfo) cout << "Parser is comparing: " << downloadedWord << "\n";

            if(downloadedWord == keyWord){

                endLoop = true;
                isFound = true;

                positionInFileAK = textFile.tellg(); // AK means "After Keyword

            }else if(showInfo) cout << "In line: " << lineInFile << " hasn't found keyword.\n\n";

        }else endLoop = true;

    }while(endLoop == false);

    textFile.close();

    if(isFound == true){

        cout << "The keyword has found\n";

        if(showInfo){

            cout << endl;
            cout << "############################################################\n";
            cout << "DBG.info: position before keyword: " << positionInFileBK << endl;
            cout << "DBG.info: position after keyword: " << positionInFileAK << endl;                                          /// VERY IMPORTANT PLACE
            cout << "In line: " << lineInFile << " has found: " << downloadedWord << endl;
            cout << "############################################################\n";

        }

        functionInfo(showInfo, "findInFile", 'e');

            switch(mode){

                case 0:
                return positionInFileBK;
                break;

                case 1:
                return positionInFileAK;
                break;

                default:
                break;

            }

        }else{

            cout << "The keyword hasn't found.\n";
            return -1;

        }

    } /// end of 'if(checkIsExist())'
}

void backupContent(string filePath, string backupFilePath, int positionInFile, bool showInfo = false){          // DONE

    // Function who make backup of content from HTML file, get since 'positionInFile' argument
    // Important hint: Content of backup file is deleted (by ios::trunc) with every use of this function

    functionInfo(showInfo, "backupContent");

    if(checkIsExist(filePath, showInfo) == true){

        string bufor;
        bool endLoop = false;

        fstream textFile;
        textFile.open(filePath.c_str());

        fstream backupFile;
        backupFile.open(backupFilePath.c_str(), ios::trunc | ios::out | ios::in);


        textFile.seekg(positionInFile, ios_base::beg);
        backupFile.seekp(0, ios_base::beg);

            do{

                if(textFile.eof() == false){

                    getline(textFile, bufor);
                    backupFile << bufor << endl;

                }else endLoop = true;

            }while(endLoop == false);

            textFile.close();
            backupFile.close();

    }else cout << "Missing file in backupContent() \n";

    functionInfo(showInfo, "backupContent",'e');
}

void generateBlock(string filePath, data filler, int positionInFile = 0, bool showInfo = false){                // DONE

    // Generate block of <div> markup with data from user

    if(checkIsExist(filePath.c_str(), showInfo) == true){

        functionInfo(showInfo, "generator");

        cout << "Path: " << filePath << " Position: " << positionInFile << endl;

        fstream textFile;
        textFile.open(filePath.c_str());

        /// positionInFile is value from findInFile() function
        textFile.seekp(positionInFile, ios_base::beg);

        string date = "<div id=\"data\"> " + filler.date + " </div>";
        string topic = "<div id=\"topic\"> " + filler.topic + " </div>";
        string content = "<div id=\"content\"> " + filler.content + " </div>";

        textFile << date << endl;
        textFile << topic << endl;
        textFile << content << endl;

        textFile << endl << filler.diverSymbol << endl << endl;

        textFile.close();

        functionInfo(showInfo, "generator", 'e');

    }
}

void putBackup(string filePath, string backupFilePath, int positionInFile, bool showInfo = false){              // DONE

    // Put content of backup file straigt to HTML file after keyword position

    functionInfo(showInfo, "backupContent");

    if(checkIsExist(filePath, showInfo) == true){

        string content = "";
        string bufor;
        bool endLoop = false;

        fstream textFile;
        textFile.open(filePath.c_str());

        fstream backupFile;
        backupFile.open(backupFilePath.c_str());

        /// (position is defined by findInFile() function)
        textFile.seekg(positionInFile, ios_base::beg);

        /// Goto start
        backupFile.seekp(0, ios_base::beg);

            do{

                if(backupFile.eof() == false){

                    getline(backupFile, bufor);
                    textFile << bufor << endl;

                }else endLoop = true;

            }while(endLoop == false);

            textFile.close();
            backupFile.close();

    }else cout << "Missing file in backupContent() \n";

    functionInfo(showInfo, "backupContent",'e');
}

string write(string name, string endKeyword = "-e", bool showInfo = false){                                     // TODO: more default information to user about used data formats

    // Write service, this set many values used in generating <div> markup data block f.e. date of post, topic, content of post and other
    // Return data entered by user

    functionInfo(showInfo, "write");

    string bufor = "";
    string content = "";
    bool endLoop = false;

    for(int i = 0; i < 60; i++) cout << "#";

    cout << "\n\n";
    cout << "# You're writting in: " << name << "\n\n";

    /// Default data formats
    if(name == "Date") cout << "Date Format: \n\n\tdd.mm.yy " << endKeyword << "\n" << "or" << "\n" << "\tdd mm yy " << endKeyword << "\n" << "or" << "\n" << "\tdd/mm/yy " << endKeyword << "\n\n";

    cout << "# If you want go to next step, you should write \" " << endKeyword << " \"; at end of your text and separate it by pressing spacebar.\n\n";
    cout << ": ";

    // TODO: more default information to user about used data formats (there is 1/3 default formats, let you see higher)

    /// If the buffer's value is other than endKeyword, put buffer's value into 'string content' and add space, else end loop
    do{

        cin >> bufor;

        if(bufor != endKeyword) {

            bufor = bufor + " ";
            content = content + bufor;

        }else endLoop = true;

    }while(endLoop == false);

    for(int i = 0; i < 60; i++) cout << "#";

    cout << "\n";

    functionInfo(showInfo, "write", 'e');

    return content;
}

string getSetting(string filePath, string settingName, bool showInfo = false){                                  // TODO: get setting from config file by using putBackup() function

    return 0;
}

void runEditor(bool showInfo){                                                                                  // TODO: get setting from config file by getSetting() function

    // Run editor, use write() function

    string endKeyword = "-e";

    string filePath       = "test_sets/index.html";
    string backupFilePath = "test_sets/backup.txt";

    data userDB; // DB means "Data Base"

    userDB.date = write("Date", endKeyword, showInfo);
    userDB.topic = write("Topic", endKeyword, showInfo);
    userDB.content = write("Content", endKeyword, showInfo);
    userDB.diverSymbol = "<!-- DIVER_INIT -->";

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    functionInfo(showInfo, "main");

    showContent(filePath, showInfo);

    // findInFile() return also position of keyword in file
    // : 0 = return position before keyword, 1 = return position after keyword

    /// Copy content from HTML file after keyword position to backup file (keyword position is defined by findInFile() function)
    backupContent(filePath, backupFilePath, findInFile(filePath, userDB.diverSymbol, 1, false), showInfo);

    /// Generate block of <div> markup with data from user in HTML file before keyword (keyword position is defined by findInFile() function)
    generateBlock(filePath, userDB, findInFile(filePath, userDB.diverSymbol, 0, false), showInfo);

    /// Put all content from backup file to HTML file after keyword position (keyword position is defined by findInFile() function)
    putBackup(filePath, backupFilePath, findInFile(filePath, userDB.diverSymbol, 1, false), showInfo);

    functionInfo(showInfo, "main", 'e');

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

int main(){                                                                                                     // TODO: main menu

    // main menu

    //runEditor(true);

    initscr();

    echo();

    getch();

    cout << "heheh";

    printw("hoho");

    getch();

    /*do{

    testNewCurses(1);
    testNewCurses(2);

    }while(true);
    */

    endwin();

    return 0;
}

























