#ifndef FILESELECTOR_H
#define FILESELECTOR_H

void welcome_fileselector();
void fileselector();

extern char* pointer_directory_path;
#include <QFile>        // needs to be included here, that fileeditor with included fileselector header knows QFile
extern QFile file;      // and knows the "file"
#endif // FILESELECTOR_H
