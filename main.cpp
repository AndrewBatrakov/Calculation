#include "mainwindow.h"
#include "databasedirection.h"
#include <QApplication>
#include "registrationform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    DataBaseDirection manipulation;

//    manipulation.connectDataBase();

    RegistrationForm registrationForm;
    registrationForm.exec();

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/client.png"));
    splash->show();

    Sleep(2000);
    MainWindow w;
    w.show();

    return a.exec();
}
