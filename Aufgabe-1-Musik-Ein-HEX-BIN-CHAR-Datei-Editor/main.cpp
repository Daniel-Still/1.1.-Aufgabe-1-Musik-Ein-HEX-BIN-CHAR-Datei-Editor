#include <QCoreApplication>
#include <fileselector.h>
#include <fileeditor.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    welcome_fileselector();

    return a.exec();
}
