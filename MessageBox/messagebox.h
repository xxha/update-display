#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QtGui/QMainWindow>
#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QObject>
#include <QtCore/QFlag>
#include <QtCore/QFlags>
#include <QtGui/QIcon>
class MessageBox : public QDialog
{
    Q_OBJECT

public:
    MessageBox(QMessageBox::Icon icon, const QString & title, const QString & text, QMessageBox::StandardButtons buttons , QWidget * parent, Qt::WindowFlags f);
    ~MessageBox();
    QMessageBox *mbox;
    QPainter *m_paint;
    QLabel *m_title;
    QPushButton *m_close_button;
    void paintEvent(QPaintEvent *);
private slots:
    //void close_button_slot();

};

#endif // MESSAGEBOX_H
