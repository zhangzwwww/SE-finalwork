#include "mainwindow.h"
//#include "loginform.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //LoginForm login_form;

//    TODO: consider style after all functions have done
//    QFile qss(":/login.qss");
//    qss.open(QFile::ReadOnly);
//    login_form.setStyleSheet(qss.readAll());
//    qss.close();

    //int ret = login_form.exec();
    //if (ret == QDialog::Accepted) {
    //    w.showMaximized();
    //    return a.exec();
    //} else {
    //    return 0;
    //}

    w.showMaximized();
    return a.exec();

}
