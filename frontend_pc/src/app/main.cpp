#include "mainwindow.h"
#include "login/loginform.h"

#include <QApplication>
#include <QFile>
#include <vtkoutputwindow.h>


int main(int argc, char *argv[])
{

    vtkOutputWindow::SetGlobalWarningDisplay(0);

    QApplication a(argc, argv);
    MainWindow w;
    LoginForm login_form;

    int ret = login_form.exec();
    if (ret == QDialog::Accepted) {
        w.showMaximized();
        return a.exec();
    } else {
        return 0;
    }
}
