#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QSqlRelationalTableModel>
#include <QtNetwork>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readSettings();
    void writeSettings();
    void createActions();
    void createMenus();
    void aboutProgProcedure();
    void editRecordOfTable();
    void addRecordOfTable();
    void deleteRecordOfTable();

    void viewTemplateTable(QString);
    void sortTable(int index);
    void updateRightPanel(QModelIndex);

    void createPanel();
    void viewClient();
    void viewOrganization();
    void viewLogistika();
    
private:
    QWidget *panelWidget;
    QMenu *fileMenu;
    QAction *exitAction;

    QMenu *referenceMenu;
    QAction *clientAction;
    QAction *organizationAction;

    QMenu *calculationMenu;
    QAction *logistikaAction;

    QMenu *helpMenu;
    QAction *aboutQtAction;
    QAction *aboutProgAction;

    QTableView *tableView;
    QLabel *tableLabel;
    QSqlRelationalTableModel *templateModel;
    //QSqlQueryModel *queryModel;

    QPushButton *addRecordTable;
    QPushButton *editRecordTable;
    QPushButton *deleteRecordTable;

    bool setFilter;
    QString filterTable;

    QLabel *searchString;
    QFile fileExchange;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
