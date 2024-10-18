#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <string>

#include "fileselector.h"
#include "fileeditor.h"


char fileselector_mode;                         // mode of fileselector   1: test.txt    ; 2: enter own directory path ; ...
QFile file;                                     // QFile named "file"
QString directory_path;                         // directory_path works as QString
std::string directory_path_string;              // directory_path_string for input --> later in code converted to QString and saved in directory_path
char* pointer_directory_path = nullptr;


// /////////////////////////////////////////////////////////////////// Welcome message from fileselector
void welcome_fileselector()
{
    qInfo() << "\n\t\t\t- Welcome to the fileselector -";
    fileselector();
}
// /////////////////////////////////////////////////////////////////// Welcome message from fileselector



// /////////////////////////////////////////////////////////////////// fileselector - chooses the file for the next steps
void fileselector()
{

    qInfo() << "Do you want to use the existing test.txt file or want to enter a file directory manually?";
    qInfo() << "1: test.txt\t2: enter directory\t3: exit";
    std::string user_input;
    std::getline(std::cin, user_input);                                                 // gets user-input as a string
    if(user_input.length() == 1)                                                        // check for correct input length
    {
        //fileselector_mode = user_input[0];
        switch(user_input[0])
        {
        case '1':                                                                       // case 1 for test-file
        {
            qInfo() << "test.txt file selected...";
            directory_path_string = "C:/Users/danny/Desktop/test.txt";
            directory_path = QString::fromStdString(directory_path_string);
            file.setFileName(directory_path);                                           // path example to file test.txt
            pointer_directory_path = &directory_path_string[0];                         // pointer_directory_path points to directory_path_string


            welcome_fileeditor();                                                       // program goes to fileeditor


            break;
        }

        case '2':                                                                       // case 2 for manually entered path
        {
            qInfo() << "enter directory path...";
            std::getline(std::cin, directory_path_string);
            //std::cin >> directory_path_string;                                        // reads in manual directory_path_string as a string - instead of cin and code in next line
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');       // (this one), because cin also takes return button from keyboard (like \n) and takes it to the next cin input --> causes wrong input for next cin --> use only one symbol --> use getline instead
            directory_path = QString::fromStdString(directory_path_string);             // transforms string to QString for Q library
            file.setFileName(directory_path);                                           // sets file to the directory path
            pointer_directory_path = &directory_path_string[0];                         // pointer_directory_path points to directory_path_string



            welcome_fileeditor();                                                       // program goes to fileeditor

            break;
        }


        case '3':                                                                       // case 3 for exiting the programm
        {
            qInfo() << "Exiting... Good bye...";
            break;
        }


        default:                                                                        // if input symbol is invalid --> goes back to start of fileselector
        {
            qInfo() << "wrong input, please use a valid number...";
            fileselector();
            break;
        }
        }
    }
    else                                                                                // if input length is too long --> goes back to start of fileselector
    {
        qInfo() << "invalid input, please use only one symbol...";
        fileselector();
    }

}
// /////////////////////////////////////////////////////////////////// fileselector - chooses the file for the next steps
