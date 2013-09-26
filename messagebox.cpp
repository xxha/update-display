#include "messagebox.h"
#include <QtCore/QDebug>

MessageBox::MessageBox( QMessageBox::Icon icon, const QString & title, const QString & text, QMessageBox::StandardButtons buttons, QWidget * parent = 0, Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry((800-400)/2,(480-150)/2, 400, 150 );
    QString str = text;
    if (str.length() > 100)
        str.resize(100);
    if (str.length() > 50)
    {
        int i = 0;
        for (i = 40;i<50;i++)
        {
            if (str.at(i) == ' ')
            {
                str.insert(i+1, "\n");
                break;
            }    
        }
        if (str.length() >50 && i == 50)
            str.insert(i, "\n");
    }
    //mbox = new QMessageBox(QMessageBox::Critical, "test", str, QMessageBox::NoButton, this, Qt::SubWindow);
    mbox = new QMessageBox(icon, title, str, buttons, parent?parent:this, f);
    mbox->setGeometry(0, 30, 30, 100);
    mbox->show();

    QObject::connect(mbox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(close()));
    m_close_button = new QPushButton(this);
    m_close_button->setGeometry(this->width()-30, 0, 30, 30);
    m_close_button->setIconSize(QSize(40, 40));
    m_close_button->setBackgroundRole(QPalette::Text);
    m_close_button->setIcon(QIcon("image/close.png"));
    m_close_button->setFlat(true);
    m_close_button->setFocusPolicy(Qt::NoFocus);
    QObject::connect(m_close_button, SIGNAL(clicked()), this, SLOT(close()));
    m_title = new QLabel(title, this);
    m_title->setGeometry(5, 5, 200, 20);
    m_title->setFrameStyle(QFrame::NoFrame);
    this->show();
}

void MessageBox::paintEvent(QPaintEvent *)
{

    m_paint=new QPainter;
    //QBrush b(Qt::darkGray, Qt::SolidPattern);
    QBrush b(Qt::lightGray);
    m_paint->begin(this);
    m_paint->setBrush(b);
    m_paint->setPen(Qt::NoPen);

    //QPen pen( Qt::black, 0 );
    //pen.setJoinStyle( Qt::MiterJoin );

    //m_paint->setPen( pen );
    m_paint->drawRect(0, 0, this->width(), 30);
    m_paint->drawRect(0, 30, 4, this->height());
    m_paint->drawRect(this->width()-4, 30, this->width(), this->height());
    m_paint->drawRect(4, this->height()-4, this->width()-4, this->height()-4);
    m_paint->end();


}

MessageBox::~MessageBox()
{

}
