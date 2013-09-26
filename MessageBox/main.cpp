#include <QtGui/QApplication>
#include "messagebox.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QString str = "connext aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaBBBBBBBBBBBBBBBBBBBBBBB";
    QString str =  "error";
    MessageBox w(QMessageBox::Critical, "upgrade successed", str, QMessageBox::NoButton, NULL, Qt::SubWindow);
    w.show();
    return a.exec();
}
