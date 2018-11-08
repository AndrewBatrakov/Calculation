#include "databasedirection.h"

DataBaseDirection::DataBaseDirection()
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
}

DataBaseDirection::~DataBaseDirection()
{
    db.close();
    db.removeDatabase("QSQLITE");
    //QSqlDatabase::removeDatabase("QSQLITE");
    //if(QFile::remove("CE_SQLite.dat"));
}


bool DataBaseDirection::connectDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Calc_SQLite.dat");
    if (!db.open()) {
        QMessageBox::warning(0, QObject::trUtf8("SQLite Database Error"),
                             db.lastError().text());
        return false;
    }else{
        return true;
    }
}
