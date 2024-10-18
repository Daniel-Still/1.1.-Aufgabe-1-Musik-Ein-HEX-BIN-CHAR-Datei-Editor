#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <bitset>
#include <QFileInfo>
#include <string>


#include <fileeditor.h>
#include <fileselector.h>


int input_length;
int file_length;
std::string fileContent_string;                 // file content as text
std::vector<std::string> fileContent_bin;
std::vector<std::string> fileContent_hex;

char character_binary;


// /////////////////////////////////////////////////////////////////// welcome message from fileeditor
void welcome_fileeditor()
{

    qInfo() << "\t\t\t- Welcome to the fileeditor -";
    fileeditor_mode_read_or_write();

}
// /////////////////////////////////////////////////////////////////// welcome message from fileeditor



// /////////////////////////////////////////////////////////////////// asks user if he wants to read or write the before chosen file
void fileeditor_mode_read_or_write()
{

    qInfo() << "Do you want to read the previously specified file, write something into it or go back to select a file?";
    qInfo() << "1: read\t\t2: write\t\t3: go back to fileselector\t\t4: exit";
    std::string user_input;
    std::getline(std::cin, user_input);                                                                         // gets user input as a string
    if(user_input.length() == 1)                                                                                // checks length of user input
    {
        switch(user_input[0])
        {
        case '1':                                                                                               // case 1 read
            qInfo() << "reading file from following path:" << pointer_directory_path << "...";
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))                                              // opens file and if it cant be opened, prints errorstring
            {
                qCritical() << "Could not open file! Check that file exists and the path is correct.";
                qInfo() << "Going back to fileselector...";
                welcome_fileselector();                                                                         // goes back to fileselector if file cant be opened
                return;
            }
            else
            {
                fileeditor_mode_read();
            }
            break;
        case '2':                                                                                               // case 2 write
            fileeditor_mode_write_userinput_binary();
            break;
        case '3':                                                                                               // case 3 back to fileselector to select another file
            welcome_fileselector();
            break;
        case '4':                                                                                               // case 4 exit
            qInfo() << "exiting... good bye...";
            break;
        default:
            qInfo() << "wrong input, please use a valid number...";
            fileeditor_mode_read_or_write();
            break;

        }
    }
    else
    {
        qInfo() << "invalid input, please use only one symbol...";
        fileeditor_mode_read_or_write();
    }

}
// /////////////////////////////////////////////////////////////////// asks user if he wants to read or write the before chosen file




// /////////////////////////////////////////////////////////////////// reads the file
void fileeditor_mode_read()
{

    QTextStream in(&file);
    QString fileContent = in.readAll();
    file_length = fileContent.length();

    if (file_length == 0)
    {
        qInfo() << "file is completely empty...";
        file.close();
    }

    else if(file_length>0)
    {
        qInfo() << "File content:" << fileContent;
        qInfo() << "file length is:" << file_length;                                            // prints file length

        fileContent_string = fileContent.toStdString();                                         // converts fileContent(QString) to a normal std::string fileContent_string
        fileContent_bin.resize(file_length);                                                    // sets fileContent_bin to size of length of file
        fileContent_hex.resize(file_length);                                                    // sets fileContent_hex to size of length of file

        for (int i = 0; i < file_length; i++)                                                   // transformation from string fileContent_string to bin/hex
        {
            fileContent_bin[i] = std::bitset<8>(fileContent_string[i]).to_string();             // std::bitset<8>(fileContent_string[i]) is a constructor which transforms a char from filecontent_string[i] into binary representation.  .to_string() transforms the binary representation into a string for output

            std::stringstream hex_stringstream;                                                 // defines hex_stringstream as a stringstream (string) --> needed for hex transformation because it does not have a .to_string() function like bitset
            hex_stringstream << std::hex <<  (static_cast<int>(fileContent_string[i]) );        // static_cast<int>(fileContent_string[i]) transforms the char fileContent_string[i] into an integer number (ASCII), which is needed for std::hex, which transforms the integer (ASCII value) to a hex value and saves it in hex_stringstream
            fileContent_hex[i] = hex_stringstream.str();                                        // saves hex_stringstream in fileContent_hex with the correct hex value/string in it
        }
        // ////////////////////////////////////////// all combined tabular
        qInfo() << "tabular depiction:";
        qInfo() << "Position\tchar\t\tbin\t\t\thex";
        for(int i = 0; i < file_length; i++)
        {
            qInfo().noquote() << "Pos" << i+1 <<"\t\t" << fileContent_string[i] << "\t\t" << fileContent_bin[i] << "\t\t" << fileContent_hex[i];    // prints the file content as text/bin/hex in a tabel
        }


        file.close();
    }
    fileeditor_mode_read_whatnext();                                                            // goes to this function to ask user what to do next

}
// /////////////////////////////////////////////////////////////////// reads the file


// /////////////////////////////////////////////////////////////////// asks user what to do, after his file was read
void fileeditor_mode_read_whatnext()
{

    qInfo() << "\nWhat do you want to do next?";
    qInfo() << "1:write into this file\t2:select another file\t3:exit";
    std::string user_input;
    std::getline(std::cin, user_input);
    if(user_input.length() == 1)
    {
        switch(user_input[0])
        {
        case '1':                                           // case 1 jumps to function and gets user input as binary code
            fileeditor_mode_write_userinput_binary();
            break;
        case '2':                                           // case 2 goes back to fileselector to select another file
            welcome_fileselector();
            break;
        case '3':                                           // case 3 exit
            qInfo() << "exiting... good bye...";
            break;
        default:
            qInfo() << "wrong input, please use a valid number...";

            fileeditor_mode_read_whatnext();
            break;
        }
    }
    else
    {
        qInfo() << "invalid input, please use only one symbol...";
        fileeditor_mode_read_whatnext();
    }

}
// /////////////////////////////////////////////////////////////////// asks user what to do, after his file was read


// /////////////////////////////////////////////////////////////////// gets binary user input, transforms it into char and writes it into file
void fileeditor_mode_write_userinput_binary()
{

    std::string binary_input;
    qInfo() << "please enter an 8 digit binary code: ";
    std::getline(std::cin, binary_input);


    if (binary_input.length() != 8)                                                         // checks if input length is 8 symbols
    {
        qCritical() << "Invalid input! Binary code must be 8 symbols";
        fileeditor_mode_write_userinput_binary();
    }


    for (char check_binary_input : binary_input)                                            // check if input only contains "0" and "1". new method of a for-slope (range based). easiest way to access to all symbols in strings
    {
        if (check_binary_input != '0' && check_binary_input != '1')
        {
            qCritical() << "Invalid input! Binary code can only use '0' and '1'.";
            fileeditor_mode_write_userinput_binary();
        }
    }


    std::bitset<8> binary_set(binary_input);                                                // transforms binary input to a bitset
    character_binary = static_cast<char>(binary_set.to_ulong());                            // converts bitset to unsigned long and then in a char (cant do it in one step from binary_set)

    qInfo() << "The binary code is equal to character: '" << character_binary << "'";       // tells user, which char as bin code he entered
    fileeditor_mode_write_method();

}
// /////////////////////////////////////////////////////////////////// gets binary user input, transforms it into char and writes it into file



// /////////////////////////////////////////////////////////////////// asks user in which type of entering-modes he wants to write into a file
void fileeditor_mode_write_method()
{

    QFileInfo check_file(pointer_directory_path);                                               // checks if file is already existing
    bool file_exists = check_file.exists();                                                     // file_exists value

    if(file_exists == false)
    {
        qInfo() << "File does not exist. Would you like to create such a file?";                // if file does not exist, asks user if a new file with the previously entered path should be created
        qInfo() << "1:yes\t2:no";
        std::string user_input;
        std::getline(std::cin, user_input);

        qInfo() << "length user input:" << user_input.length() << "with content" << user_input;
        if(user_input[0] == '2' && user_input.length() == 1)
        {
            qInfo() << "going back to fileselector...";
            welcome_fileselector();
        }
        else if(user_input[0] != '1' || user_input.length() != 1)
        {
            qInfo() << "wrong input, please use a valid number...";
            fileeditor_mode_write_method();
        }
    }

    qInfo() << "for writing into your file file...";
    qInfo() << "would you like to: add symbols at the end of existing symbols (append) or write from position 0 and overwrite all file content(writeonly)? ";
    qInfo() << "1: append\t2: writeonly\t\t3: back to fileselector\t\t\t4: exit";
    std::string user_input;
    std::getline(std::cin, user_input);
    if(user_input.length() == 1)
    {
        switch(user_input[0])
        {
        case '1':                                                           // case 1 for append (add at the end of files content)
            qInfo() << "entering mode: append...";
            fileeditor_mode_write_method_append();
            break;
        case '2':                                                           // case 2 write from start and overwrite following symbols
            qInfo() << "entering mode: writeonly...";
            fileeditor_mode_write_method_writeonly();
            break;
        case '3':                                                           // case 3 goes back to fileselector
            welcome_fileselector();
            break;
        case '4':                                                           // exit
            qInfo() << "exiting... good bye...";
            break;

        default:
            qInfo() << "wrong input, please use a valid number...";

            fileeditor_mode_read_whatnext();
            break;
        }
    }
    else
    {
        qInfo() << "invalid input, please use only one symbol...";
        fileeditor_mode_write_method();
    }

}
// /////////////////////////////////////////////////////////////////// asks user in which type of entering-modes he wants to write into a file



// /////////////////////////////////////////////////////////////////// function for appending into a file
void fileeditor_mode_write_method_append()
{

    {
        if (!file.open(QIODevice::Append | QIODevice::Text))                                                // opens file and if it cant be opened, prints errorstring. Writemode append
        {
            qCritical() << "Could not open file for writing! Check that path for creating is correct.";
            qInfo() << "Going back to fileselector...";
            welcome_fileselector();
            return;
        }
        else
        {
            QTextStream out(&file);
            out << character_binary;                                                                        // writes char into file
            file.close();
        }

        fileeditor_mode_write_textafterwrite();


    }

}
// /////////////////////////////////////////////////////////////////// function for appending into a file

// /////////////////////////////////////////////////////////////////// function for writeonly into a file
void fileeditor_mode_write_method_writeonly()
{

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))                                                 // opens file and if it cant be opened, prints errorstring. Writemode writeonly
    {
        qCritical() << "Could not open file for writing! Check that path for creating is correct.";
        qInfo() << "Going back to fileselector...";
        welcome_fileselector();
        return;
    }
    else
    {
        QTextStream out(&file);
        out << character_binary;                                                                            // writes char into file
        file.close();
    }

    fileeditor_mode_write_textafterwrite();

}
// /////////////////////////////////////////////////////////////////// function for writeonly into a file


// /////////////////////////////////////////////////////////////////// asks user what to do after write mode finished
void fileeditor_mode_write_textafterwrite()
{

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        qInfo() << "file now contents:" << fileContent;                         // prints new file content after user added something
    }
    qInfo() << "what do you want to do next?";
    qInfo() << "1: write again\t2: back to fileselector\t3:exit";
    std::string user_input;
    std::getline(std::cin, user_input);
    if(user_input.length() == 1)
    {
        switch(user_input[0])
        {
        case '1':                                                               // case 1 if user wants to write again in same file
            fileeditor_mode_write_userinput_binary();
            break;
        case '2':                                                               // case 2 if user wants to select another file
            welcome_fileselector();
            break;
        case '3':                                                               // case 3 exit
            qInfo() << "exiting... good bye...";
            break;

        default:
            qInfo() << "wrong input, please use a valid number...";

            fileeditor_mode_write_textafterwrite();
            break;
        }
    }
    else
    {
        qInfo() << "invalid input, please use only one symbol...";
        fileeditor_mode_write_textafterwrite();
    }

}
// /////////////////////////////////////////////////////////////////// asks user what to do after write mode finished
