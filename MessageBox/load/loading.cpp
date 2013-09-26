#include  <QApplication>
#include <QLabel>
//#include <qpixmap.h>    //qt中的类
#include <QTimer>
#include <QMovie>
int main (int argc, char *argv[])
{
    QApplication app(argc,argv);
    QLabel *label=new QLabel ("", NULL); //初始化qlabel
    QMovie pm("loading.gif");   //设定要显示的图片
    label->setMovie(&pm); //将图片加载到label上
    //label->setAttribute(Qt::WA_TranslucentBackground, true);
    label->setGeometry( 370, 210, 60, 60 ); //屏幕大小，初始位置
    pm.setScaledSize(QSize(60, 60));
    pm.start();
    //app.setMainWidget(label); //将图片设为放置在中间
    // QTimer::singleShot( 3*1000, label, SLOT(close()));//显示时间
    label->show();
    return app.exec();
}
