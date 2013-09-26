#include "update_soft.h"
#include  <QtCore/QFile>
#include  <QtCore/QIODevice>
#include <QtGui/QMessageBox>
#include <sys/types.h>
#include <signal.h>
#include <QtGui/QMovie>
#define gWidth 800
#define gHeight 600

#define g_STR_UX400_UPGRADE_TITLE    "UX400 Upgrade Software"
#define g_STR_UPGRADE_ROOTFS    "rootfs"
#define g_STR_LOADING    "loading"
#define g_CHECKED_PNG     "./image/checkbox_checked.png"
#define g_UNCHECK_PNG     "./image/checkbox_unchecked.png"
#define g_LOADING_GIF     "./image/loading.gif"

struct upgrade_module
{
    QString current_version_file;
    QString new_version_file;
};

struct upgrade_module module[8] =
{
    {"/usr/local/10g/lib/vendor-info.txt", "/tmp/version.10g"},
    {"/usr/local/40g/lib/vendor-info.txt", "/tmp/version.40g"},
    {"/usr/local/100ge/lib/vendor-info.txt", "/tmp/version.100ge"},
    {"/usr/local/16g/lib/vendor-info.txt", "/tmp/version.16g"},
    {"/usr/local/1ge/lib/vendor-info.txt", "/tmp/version.1ge"},
    {"/usr/local/2g5/lib/vendor-info.txt", "/tmp/version.2g5"},
    {"/usr/local/v300/lib/vendor-info.txt", "/tmp/version.v300"},
    {"/etc/version", "/tmp/version.rootfs"}
};

void sig_handle(int sig)
{
    if (sig == SIGCHLD)
    {
        int pid;
        int status;
        //printf("recv SIGCHLD\n");
        while((pid = waitpid(-1, &status, WNOHANG)) > 0);
    }
}

QString read_rootfs_version(const QString path)
{
    if (path.isEmpty())
        return "";
    QString ret = "";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }
    QTextStream in(&file);
    QString tmp;
    //读取一行//如果读取多行
    while ( (tmp=in.readLine()) != "")
    {
        ret = tmp;
    }
    file.close();
    return ret;
}


QString update_soft::read_file( const QString path)
{
    if (path.isEmpty())
        return "";
    QString ret = "";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //QMessageBox::warning(this,"Warnning","can't open",QMessageBox::Yes);
        //qDebug() << "can't open file: " << path;
        return "";
    }
    QTextStream in(&file);
    QString tmp;
    //读取一行//如果读取多行
    while ( (tmp=in.readLine()) != "")
    {
        if (tmp.contains("SOFTWARE-VERSION:", Qt::CaseSensitive))
        {
            ret = tmp.remove("SOFTWARE-VERSION:", Qt::CaseSensitive).remove(" ", Qt::CaseSensitive);
            break;
        }
    }
    file.close();
    return ret;
}

/*
  module type:
  0 : 10g
  1 : 40g
  2 : 100ge
  3 : 16g
  4 : 1ge
  5 : 2g5
  6 : v300
*/
bool update_soft::get_soft_version(const int type)
{
    if (type < 0 || type > 7)
        return 0;
    QString new_version = "";
    QString current_version = "";
  //  qDebug() <<"type:" <<type;
    switch (type)
    {
    case 0:
        current_version = read_file(module[type].current_version_file);
        current_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_1_2->setText(current_version);
        new_version = read_file(module[type].new_version_file);
        new_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_1_3->setText(new_version);

        //qDebug() << current_version <<"," <<new_version;
        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_1_3->setTextColor(color);
            m_10gCheckBox->setEnabled(true);
            m_10gCheckBox_isChecked = true;
            m_10gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_10gCheckBox->setEnabled(true);
            m_10gCheckBox_isChecked = false;
            m_10gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    case 1:
        current_version = read_file(module[type].current_version_file);
        current_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_2_2->setText(current_version);
        new_version = read_file(module[type].new_version_file);
        new_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_2_3->setText(new_version);

        //qDebug() << current_version <<"," <<new_version;
        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_2_3->setTextColor(color);
            m_40gCheckBox->setEnabled(true);
            m_40gCheckBox_isChecked = true;
            m_40gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_40gCheckBox->setEnabled(true);
            m_40gCheckBox_isChecked = false;
            m_40gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    case 2:
        current_version = read_file(module[type].current_version_file);
        current_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_3_2->setText(current_version);
        new_version = read_file(module[type].new_version_file);
        new_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_3_3->setText(new_version);

        //qDebug() << current_version <<"," <<new_version;
        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_3_3->setTextColor(color);
            m_100geCheckBox->setEnabled(true);
            m_100geCheckBox_isChecked = true;
            m_100geCheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_100geCheckBox->setEnabled(true);
            m_100geCheckBox_isChecked = false;
            m_100geCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    case 3:
        current_version = read_file(module[type].current_version_file);
        current_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_4_2->setText(current_version);
        new_version = read_file(module[type].new_version_file);
        new_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_4_3->setText(new_version);
        //qDebug() << current_version <<"," <<new_version;
        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_4_3->setTextColor(color);
            m_16gCheckBox->setEnabled(true);
            m_16gCheckBox_isChecked = true;
            m_16gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_16gCheckBox->setEnabled(true);
            m_16gCheckBox_isChecked = false;
            m_16gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    case 4:
        current_version = read_file(module[type].current_version_file);
        current_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_5_2->setText(current_version);
        new_version = read_file(module[type].new_version_file);
        new_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_5_3->setText(new_version);

        //qDebug() << current_version <<"," <<new_version;
        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_5_3->setTextColor(color);
            m_1geCheckBox->setEnabled(true);
            m_1geCheckBox_isChecked = true;
            m_1geCheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_1geCheckBox->setEnabled(true);
            m_1geCheckBox_isChecked = false;
            m_1geCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    case 5:
        current_version = read_file(module[type].current_version_file);
        current_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_6_2->setText(current_version);
        new_version = read_file(module[type].new_version_file);
        new_version.remove("ux400-", Qt::CaseInsensitive);
        m_item_6_3->setText(new_version);

        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_6_3->setTextColor(color);
            m_2_5gCheckBox->setEnabled(true);
            m_2_5gCheckBox_isChecked = true;
            m_2_5gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_2_5gCheckBox->setEnabled(true);
            m_2_5gCheckBox_isChecked = false;
            m_2_5gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    case 6:
        //v300 module
        current_version = read_file(module[type].current_version_file);
        current_version.remove("ux400-", Qt::CaseInsensitive);
        current_version.replace("Common", "Comm", Qt::CaseInsensitive);
        m_item_7_2->setText(current_version);
       // system("gunzip -dc ux400-local-v300.tar.gz | tar xf - v300/lib/vendor-info.txt");
        new_version = read_file(module[type].new_version_file);
        new_version.remove("ux400-", Qt::CaseInsensitive);
        new_version.replace("Common", "Comm", Qt::CaseInsensitive);
        m_item_7_3->setText(new_version);

        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_7_3->setTextColor(color);
            m_common_CheckBox->setEnabled(true);
            m_common_CheckBox_isChecked = true;
            m_common_CheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_common_CheckBox->setEnabled(true);
            m_common_CheckBox_isChecked = false;
            m_common_CheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    case 7:
        current_version = read_rootfs_version(module[type].current_version_file);
        m_item_8_2->setText(current_version);
       // system("gunzip -dc ux400-local-v300.tar.gz | tar xf - v300/lib/vendor-info.txt");
        new_version = read_rootfs_version(module[type].new_version_file);
        //new_version.remove("ux400-", Qt::CaseInsensitive);
        //new_version.replace("Common", "Comm", Qt::CaseInsensitive);
        m_item_8_3->setText(new_version);

        //qDebug() << current_version <<"," <<new_version;
        if (new_version.isEmpty() == false && !(new_version == current_version))
        {
            QColor color(Qt::red);
            m_item_8_3->setTextColor(color);
            m_rootfs_CheckBox->setEnabled(true);
            m_rootfs_CheckBox_isChecked = true;
            m_rootfs_CheckBox->setIcon(QIcon(g_CHECKED_PNG));
        }
        else if(new_version.isEmpty() == false)
        {
            m_rootfs_CheckBox->setEnabled(true);
            m_rootfs_CheckBox_isChecked = false;
            m_rootfs_CheckBox->setIcon(QIcon(g_UNCHECK_PNG));
        }
        else
        {}
        break;
    default:
        //unknow type
        break;
    }

    //sleep(2);
    return 1;
}

update_soft::update_soft(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(gWidth, gHeight);
    this->setWindowFlags(Qt::FramelessWindowHint);

    int cell_window_width = gWidth/13;
    int cell_window_height = gHeight/13;
    int reserve_width = 10;
    int use_width = gWidth-reserve_width*2;

    read_fifo = -1;
    upgrade_pid = -1;
    QFont f("unicode", 30, 0);
    f.setBold(true);

    QFont p("unicode", 18, 0);

    m_titleLable = new QLabel(this);
    m_titleLable->setObjectName(QString::fromUtf8("m_titleLable"));
    m_titleLable->setGeometry(QRect(reserve_width, cell_window_height/4, use_width, cell_window_height));
    m_titleLable->setText(g_STR_UX400_UPGRADE_TITLE);
    m_titleLable->setAlignment(Qt::AlignCenter);
    m_titleLable->setFont(f);
    //m_titleLable->setFrameShape(QFrame::Box);
    m_titleLable->setFrameShape(QFrame::NoFrame);

    m_rootfs_CheckBox = new QPushButton(this);
    m_rootfs_CheckBox->setObjectName(QString::fromUtf8("m_rootfs_CheckBox"));
    //m_rootfs_CheckBox->setGeometry(QRect(reserve_width+28, cell_window_height*21/2, use_width/12, cell_window_height));
    m_rootfs_CheckBox->setIconSize(QSize(25, 25));
    //m_rootfs_CheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    m_rootfs_CheckBox->setFlat(true);
    m_rootfs_CheckBox->setEnabled(false);
    m_rootfs_CheckBox->setFocusPolicy(Qt::NoFocus);
    m_rootfs_CheckBox->setText("");

    /*
    m_rootfsLable = new QLabel(this);
    m_rootfsLable->setObjectName(QString::fromUtf8("m_rootfsLable"));
    m_rootfsLable->setGeometry(QRect(reserve_width+28+use_width/12, cell_window_height*21/2, use_width/6, cell_window_height));
    m_rootfsLable->setText(g_STR_UPGRADE_ROOTFS);
    m_rootfsLable->setAlignment(Qt::AlignCenter);
    m_rootfsLable->setFont(p);
    //m_titleLable->setFrameShape(QFrame::Box);
    m_rootfsLable->setFrameShape(QFrame::NoFrame);
*/
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setObjectName(QString::fromUtf8("m_tableWidget"));
    m_tableWidget->setGeometry(QRect(reserve_width, cell_window_height*3/2, use_width, cell_window_height*17/2));
    //-m_tableWidget->setRowCount(8);
    m_tableWidget->setRowCount(9);
    m_tableWidget->verticalHeader()->hide();
    m_tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    m_tableWidget->setColumnCount(6);
    m_tableWidget->horizontalHeader()->hide();
    //m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏进度条
    //m_tableWidget->horizontalHeader()->setStretchLastSection(true);
   // m_tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
   // m_tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);

    m_tableWidget->setColumnWidth(0, 120);
    m_tableWidget->setColumnWidth(1, 110);
    m_tableWidget->setColumnWidth(4, 120);
    m_tableWidget->setColumnWidth(5, 90);

    m_tableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);

    //m_tableWidget->resizeColumnToContents(2);
    //m_tableWidget->resizeColumnToContents(3);
    //m_tableWidget->resizeRowsToContents();
    m_tableWidget->setFrameShape(QFrame::NoFrame);

    m_10gCheckBox = new QPushButton(this);
    m_10gCheckBox->setObjectName(QString::fromUtf8("m_10gCheckBox"));
    m_10gCheckBox->setFlat(true);
    m_10gCheckBox->setFocusPolicy(Qt::NoFocus);
    m_10gCheckBox->setIconSize(QSize(25, 25));
    m_10gCheckBox->setEnabled(false);
  //  m_10gCheckBox->setEnabled(true);
  //  m_10gCheckBox->setIcon(QIcon(g_CHECKED_PNG));

    m_40gCheckBox = new QPushButton(this);
    m_40gCheckBox->setFlat(true);
    m_40gCheckBox->setFocusPolicy(Qt::NoFocus);
    m_40gCheckBox->setIconSize(QSize(25, 25));
    m_40gCheckBox->setObjectName(QString::fromUtf8("m_40gCheckBox"));
    m_40gCheckBox->setEnabled(false);

    m_100geCheckBox = new QPushButton(this);
    m_100geCheckBox->setFlat(true);
    m_100geCheckBox->setFocusPolicy(Qt::NoFocus);
    m_100geCheckBox->setIconSize(QSize(25, 25));
    m_100geCheckBox->setObjectName(QString::fromUtf8("m_100geCheckBox"));
    m_100geCheckBox->setEnabled(false);

    m_16gCheckBox = new QPushButton(this);
    m_16gCheckBox->setFlat(true);
    m_16gCheckBox->setFocusPolicy(Qt::NoFocus);
    m_16gCheckBox->setIconSize(QSize(25, 25));
    m_16gCheckBox->setObjectName(QString::fromUtf8("m_16gCheckBox"));
    m_16gCheckBox->setEnabled(false);

    m_1geCheckBox = new QPushButton(this);
    m_1geCheckBox->setFlat(true);
    m_1geCheckBox->setFocusPolicy(Qt::NoFocus);
    m_1geCheckBox->setIconSize(QSize(25, 25));
    m_1geCheckBox->setObjectName(QString::fromUtf8("m_1geCheckBox"));
    m_1geCheckBox->setEnabled(false);
    m_2_5gCheckBox = new QPushButton(this);
    m_2_5gCheckBox->setFlat(true);
    m_2_5gCheckBox->setFocusPolicy(Qt::NoFocus);
    m_2_5gCheckBox->setIconSize(QSize(25, 25));
    m_2_5gCheckBox->setObjectName(QString::fromUtf8("m_2_5gCheckBox"));
    m_2_5gCheckBox->setEnabled(false);
    m_common_CheckBox = new QPushButton(this);
    m_common_CheckBox->setFlat(true);
    m_common_CheckBox->setFocusPolicy(Qt::NoFocus);
    m_common_CheckBox->setIconSize(QSize(25, 25));
    m_common_CheckBox->setObjectName(QString::fromUtf8("m_common_CheckBox"));
    m_common_CheckBox->setEnabled(false);


    m_tableWidget->setCellWidget(1, 0, m_10gCheckBox);
    m_tableWidget->setCellWidget(2, 0, m_40gCheckBox);
    m_tableWidget->setCellWidget(3, 0, m_100geCheckBox);
    m_tableWidget->setCellWidget(4, 0, m_16gCheckBox);
    m_tableWidget->setCellWidget(5, 0, m_1geCheckBox);
    m_tableWidget->setCellWidget(6, 0, m_2_5gCheckBox);
    m_tableWidget->setCellWidget(7, 0, m_common_CheckBox);
    m_tableWidget->setCellWidget(8, 0, m_rootfs_CheckBox);

    m_cancel_button = new QPushButton(this);
    m_cancel_button->setObjectName(QString::fromUtf8("m_cancel_button"));
    m_cancel_button->setGeometry(QRect(reserve_width+use_width*3/5, cell_window_height*21/2, use_width/6, cell_window_height*2));
    m_cancel_button->setFont(p);
    m_cancel_button->setText("Cancel");

    m_upgrade_button = new QPushButton(this);
    m_upgrade_button->setObjectName(QString::fromUtf8("m_upgrade_button"));
    m_upgrade_button->setGeometry(QRect(reserve_width+use_width*4/5, cell_window_height*21/2, use_width/6, cell_window_height*2));
    m_upgrade_button->setFont(p);
    m_upgrade_button->setText("Upgrade");

    m_item_0_0 =  new QTableWidgetItem("Upgrade");
    m_item_0_0->setTextAlignment(Qt::AlignVCenter);
    m_item_0_0->setFont(p);
    m_tableWidget->setItem(0, 0, m_item_0_0 );

    m_item_0_1 =  new QTableWidgetItem("Module");
    m_item_0_1->setTextAlignment(Qt::AlignVCenter);
    m_item_0_1->setFont(p);
    m_tableWidget->setItem(0, 1, m_item_0_1 );

    m_item_0_2 = new QTableWidgetItem("Current");
    m_item_0_2->setTextAlignment(Qt::AlignVCenter);
    m_item_0_2->setFont(p);
    m_tableWidget->setItem(0, 2, m_item_0_2);

    m_item_0_3 = new QTableWidgetItem("New");
    m_item_0_3->setTextAlignment(Qt::AlignVCenter);
    m_item_0_3->setFont(p);
    m_tableWidget->setItem(0, 3, m_item_0_3);

    m_item_0_4 = new QTableWidgetItem("Progress");
    m_item_0_4->setTextAlignment(Qt::AlignVCenter);
    m_item_0_4->setFont(p);
    m_tableWidget->setItem(0, 4, m_item_0_4);
    
    m_item_0_5 = new QTableWidgetItem("Result");
    m_item_0_5->setTextAlignment(Qt::AlignVCenter);
    m_item_0_5->setFont(p);
    m_tableWidget->setItem(0, 5, m_item_0_5);

    m_item_1_1 =  new QTableWidgetItem("10G");
    m_item_1_1->setTextAlignment(Qt::AlignVCenter);
    m_item_1_1->setFont(p);
    m_tableWidget->setItem(1, 1, m_item_1_1 );

    m_item_1_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_1_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(1, 2, m_item_1_2 );

    m_item_1_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_1_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(1, 3, m_item_1_3 );

    m_item_1_5 =  new QTableWidgetItem("");
    m_item_1_5->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(1, 5, m_item_1_5 );

    m_item_2_1 =  new QTableWidgetItem("40G");
    m_item_2_1->setTextAlignment(Qt::AlignVCenter);
    m_item_2_1->setFont(p);
    m_tableWidget->setItem(2, 1, m_item_2_1 );

    m_item_2_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_2_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(2, 2, m_item_2_2 );

    m_item_2_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_2_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(2, 3, m_item_2_3 );

    m_item_2_5 =  new QTableWidgetItem("");
    m_item_2_5->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(2, 5, m_item_2_5 );

    m_item_3_1 =  new QTableWidgetItem("100GE");
    m_item_3_1->setTextAlignment(Qt::AlignVCenter);
    m_item_3_1->setFont(p);
    m_tableWidget->setItem(3, 1, m_item_3_1 );

    m_item_3_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_3_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(3, 2, m_item_3_2 );

    m_item_3_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_3_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(3, 3, m_item_3_3 );


    m_item_3_5 =  new QTableWidgetItem("");
    m_item_3_5->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(3, 5, m_item_3_5 );


    m_item_4_1 =  new QTableWidgetItem("16G");
    m_item_4_1->setTextAlignment(Qt::AlignVCenter);
    m_item_4_1->setFont(p);
    m_tableWidget->setItem(4, 1, m_item_4_1 );

    m_item_4_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_4_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(4, 2, m_item_4_2 );

    m_item_4_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_4_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(4, 3, m_item_4_3 );


    m_item_4_5 =  new QTableWidgetItem("");
    m_item_4_5->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(4, 5, m_item_4_5 );


    m_item_5_1 =  new QTableWidgetItem("1GE");
    m_item_5_1->setTextAlignment(Qt::AlignVCenter);
    m_item_5_1->setFont(p);
    m_tableWidget->setItem(5, 1, m_item_5_1 );

    m_item_5_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_5_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(5, 2, m_item_5_2 );

    m_item_5_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_5_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(5, 3, m_item_5_3 );


    m_item_5_5 =  new QTableWidgetItem("");
    m_item_5_5->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(5, 5, m_item_5_5 );

    m_item_6_1 =  new QTableWidgetItem("2G5");
    m_item_6_1->setTextAlignment(Qt::AlignVCenter);
    m_item_6_1->setFont(p);
    m_tableWidget->setItem(6, 1, m_item_6_1 );

    m_item_6_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_6_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(6, 2, m_item_6_2 );

    m_item_6_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_6_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(6, 3, m_item_6_3 );

    m_item_6_5 =  new QTableWidgetItem("");
    m_item_6_5->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(6, 5, m_item_6_5 );

    m_item_7_1 =  new QTableWidgetItem("Comm");
    m_item_7_1->setTextAlignment(Qt::AlignVCenter);
    m_item_7_1->setFont(p);
    m_tableWidget->setItem(7, 1, m_item_7_1 );

    m_item_7_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_7_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(7, 2, m_item_7_2 );

    m_item_7_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_7_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(7, 3, m_item_7_3 );

    m_item_7_5 =  new QTableWidgetItem("");
    m_item_7_5->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(7, 5, m_item_7_5 );

    m_item_8_1 =  new QTableWidgetItem("Rootfs");
    m_item_8_1->setTextAlignment(Qt::AlignVCenter);
    m_item_8_1->setFont(p);
    m_tableWidget->setItem(8, 1, m_item_8_1 );

    m_item_8_2 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_8_2->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(8, 2, m_item_8_2 );

    m_item_8_3 =  new QTableWidgetItem(g_STR_LOADING);
    m_item_8_3->setTextAlignment(Qt::AlignVCenter);
    m_tableWidget->setItem(8, 3, m_item_8_3 );

    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    QObject::connect(m_10gCheckBox, SIGNAL(released()), this, SLOT(m_10gCheckBox_slot()));
    QObject::connect(m_40gCheckBox, SIGNAL(released()), this, SLOT(m_40gCheckBox_slot()));
    QObject::connect(m_100geCheckBox, SIGNAL(released()), this, SLOT(m_100geCheckBox_slot()));
    QObject::connect(m_16gCheckBox, SIGNAL(released()), this, SLOT(m_16gCheckBox_slot()));
    QObject::connect(m_1geCheckBox, SIGNAL(released()), this, SLOT(m_1geCheckBox_slot()));
    QObject::connect(m_2_5gCheckBox, SIGNAL(released()), this, SLOT(m_2_5gCheckBox_slot()));
    QObject::connect(m_common_CheckBox, SIGNAL(released()), this, SLOT(m_common_CheckBox_slot()));
    QObject::connect(m_cancel_button, SIGNAL(released()), this, SLOT(m_cancel_button_slot()));
    QObject::connect(m_upgrade_button, SIGNAL(released()), this, SLOT(m_upgrade_button_slot()));
    QObject::connect(m_rootfs_CheckBox, SIGNAL(released()), this, SLOT(m_rootfs_CheckBox_slot()));

    m_10gCheckBox_isChecked = false;
    m_40gCheckBox_isChecked = false;
    m_100geCheckBox_isChecked = false;
    m_16gCheckBox_isChecked = false;
    m_1geCheckBox_isChecked = false;
    m_2_5gCheckBox_isChecked = false;
    m_common_CheckBox_isChecked = false;
    m_rootfs_CheckBox_isChecked = false;

    m_item_1_4 = new QProgressBar(NULL);
    m_item_1_4->setValue(0);
    //m_tableWidget->setCellWidget(1, 4, m_item_1_4);

    m_item_2_4 = new QProgressBar(NULL);
    m_item_2_4->setValue(0);
    //m_tableWidget->setCellWidget(2, 4, m_item_2_4);


    m_item_3_4 = new QProgressBar(NULL);
    m_item_3_4->setValue(0);
   // m_tableWidget->setCellWidget(3, 4, m_item_3_4);

    m_item_4_4 = new QProgressBar(NULL);
    m_item_4_4->setValue(0);
    //m_tableWidget->setCellWidget(4, 4, m_item_4_4);


    m_item_5_4 = new QProgressBar(NULL);
    m_item_5_4->setValue(0);
    //m_tableWidget->setCellWidget(5, 4, m_item_5_4);

    m_item_6_4 = new QProgressBar(NULL);
    m_item_6_4->setValue(0);
   // m_tableWidget->setCellWidget(6, 4, m_item_6_4);

    m_item_7_4 = new QProgressBar(NULL);
    m_item_7_4->setValue(0);
    //m_tableWidget->setCellWidget(7, 4, m_item_7_4);
/*
    for (int i=0; i < sizeof(image_map)/sizeof(IMAGES_MAP); i++)
    {
        get_soft_version(i);
    }
*/
    this->m_timer = new QTimer(this);

    m_cancel_button->setEnabled(false);
    m_upgrade_button->setEnabled(false);

    this->m_loading = new QLabel (this, Qt::SubWindow); //初始化qlabel
    m_loading->setObjectName(QString::fromUtf8("m_loading"));
    m_loading->setGeometry(QRect(370, 210, 60, 60 )); //屏幕大小，初始位置
    m_loading->setFont(f);
    m_loading->setText(g_STR_LOADING);
    //m_titleLable->setFrameShape(QFrame::Box);
    //m_loading->setFrameShape(QFrame::Box);
    m_movie = new QMovie(g_LOADING_GIF);   //设定要显示的图片
    m_loading->setMovie(m_movie); //将图片加载到label上
    m_movie->setScaledSize(QSize(60, 60));
    m_movie->start();

    m_load_version_timer = new QTimer(this);
    QObject::connect(this->m_load_version_timer, SIGNAL(timeout()), this, SLOT(load_version_timer()));
    m_load_version_timer->start(10);
}

void update_soft::show_loaded_status()
{
    m_cancel_button->setEnabled(true);
    m_upgrade_button->setEnabled(true);
    should_enable_upgrade();
}

void update_soft::load_version_timer()
{

        if (extract_thread_status == -1)
            return;
        static int i=0;
        while(module_ready)
        {       
            for (; i < sizeof(image_map)/sizeof(IMAGES_MAP); i++)
            {
                get_soft_version(i);
                if (module_ready & (1<<i))
                {
                    module_ready &= ~(1<<i);
                    i++;
                    break;
                }
            }
        }

        if (!module_ready && (extract_thread_status==0))
        {
            int j = 0;
            for (; j < sizeof(image_map)/sizeof(IMAGES_MAP); j++)
            {
                this->get_soft_version(j);
            }

            QObject::disconnect(this->m_load_version_timer, SIGNAL(timeout()), this, SLOT(load_version_timer()));
            m_load_version_timer->stop();
            this->m_loading->close();
            if (IS_HAVE_INVALID())
            {
                int i;
                char msg[128];
                for (i=0; i< sizeof(image_map)/sizeof(IMAGES_MAP); i++)
                {
                    if (IS_BIT_INVALID(i))
                    {
                            sprintf(msg, "%s is a invalid package, cann't decompressed,please check it", image_map[i].image_name);
                            QString text(msg);
                            MessageBox *msg= new MessageBox(QMessageBox::Warning, "Package Invalid", text, QMessageBox::NoButton, NULL, Qt::SubWindow);
                            msg->exec();
                            CLEAR_BIT_INVALID(i);
                    }
                }
            }
            CLEAR_UPGRADE_FLAG();

            show_loaded_status();
        }
}

void update_soft::should_enable_upgrade()
{
    if (m_10gCheckBox_isChecked || m_40gCheckBox_isChecked|| m_100geCheckBox_isChecked||
        m_1geCheckBox_isChecked||m_2_5gCheckBox_isChecked||m_common_CheckBox_isChecked||
             m_rootfs_CheckBox_isChecked||m_16gCheckBox_isChecked)
    {
        m_upgrade_button->setEnabled(true);
    }
    else
        m_upgrade_button->setEnabled(false);
}

void update_soft::m_rootfs_CheckBox_slot()
{
    if (this->m_rootfs_CheckBox_isChecked)
    {
        m_rootfs_CheckBox_isChecked = false;
        m_rootfs_CheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_rootfs_CheckBox_isChecked = true;
        m_rootfs_CheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_10gCheckBox_slot()
{
    if (this->m_10gCheckBox_isChecked)
    {
        m_10gCheckBox_isChecked = false;
        m_10gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_10gCheckBox_isChecked = true;
        m_10gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_40gCheckBox_slot()
{
    if (this->m_40gCheckBox_isChecked)
    {
        m_40gCheckBox_isChecked = false;
        m_40gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_40gCheckBox_isChecked = true;
        m_40gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_100geCheckBox_slot()
{
    if (this->m_100geCheckBox_isChecked)
    {
        m_100geCheckBox_isChecked = false;
        m_100geCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_100geCheckBox_isChecked = true;
        m_100geCheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_16gCheckBox_slot()
{
    if (this->m_16gCheckBox_isChecked)
    {
        m_16gCheckBox_isChecked = false;
        m_16gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_16gCheckBox_isChecked = true;
        m_16gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_1geCheckBox_slot()
{
    if (this->m_1geCheckBox_isChecked)
    {
        m_1geCheckBox_isChecked = false;
        m_1geCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_1geCheckBox_isChecked = true;
        m_1geCheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_2_5gCheckBox_slot()
{
    if (this->m_2_5gCheckBox_isChecked)
    {
        m_2_5gCheckBox_isChecked = false;
        m_2_5gCheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_2_5gCheckBox_isChecked = true;
        m_2_5gCheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_common_CheckBox_slot()
{
    if (this->m_common_CheckBox_isChecked)
    {
        m_common_CheckBox_isChecked = false;
        m_common_CheckBox->setIcon(QIcon(g_UNCHECK_PNG));
    }
    else
    {
        m_common_CheckBox_isChecked = true;
        m_common_CheckBox->setIcon(QIcon(g_CHECKED_PNG));
    }
    should_enable_upgrade();
}

void update_soft::m_cancel_button_slot()
{
    CLEAR_UPGRADE_FLAG();

    if (upgrade_pid > 0)
    {

        kill(upgrade_pid, 9);
        upgrade_pid = -1;
    }
    this->close();
    system("poweroff");
}

void update_soft::timer_timeout()
{

    struct msgtype msg;
    int size;
    char buf[64] = {0};

    if ( (size = msgrcv(msg_id, &msg, MSG_LEN, eCLIENT, IPC_NOWAIT|MSG_NOERROR)) < 0)
    {
        //perror("serv:recv msg queue fail:");
        if (errno == ENOMSG)
        {
            int ret;
            ret = kill(upgrade_pid, 0);
            if (ret < 0)
            {
                printf("child had dead\n");
                //QMessageBox::critical(NULL, "Unknow Error", "System an unknown error occurred", QMessageBox::Ok);
                //QApplication b(0, NULL);
                MessageBox *msg = new MessageBox(QMessageBox::Critical, "Unknow Error", "System an unknown error occurred", QMessageBox::NoButton, NULL, Qt::SubWindow);
                msg->exec();
                system("poweroff");
                //this->close();
                //b.exec();
                //exit(0);
            }
        }
    }
    else
    {
        printf("serv:mgstype=%d, buf=%s\n", msg.mtype, msg.buffer);
    }

    if (size > 0)
    {
        if (msg.mtype == eSTART)
        {
            upgrade_type = atoi(msg.buffer);
            if (upgrade_type >= 0 && upgrade_type <=6)
            {
                memset(msg.buffer, 0, sizeof(msg.buffer));
                msg.from = eSERVER;
                strcpy(msg.buffer, "a");//ack
                msgsnd(msg_id, &msg, MSG_LEN, 0);
                if (upgrade_type == 0)
                    m_tableWidget->setCellWidget(1, 4, m_item_1_4);
                else if (upgrade_type ==1)
                    m_tableWidget->setCellWidget(2, 4, m_item_2_4);
                else if (upgrade_type ==2)
                    m_tableWidget->setCellWidget(3, 4, m_item_3_4);
                else if (upgrade_type ==3)
                    m_tableWidget->setCellWidget(4, 4, m_item_4_4);
                else if (upgrade_type ==4)
                    m_tableWidget->setCellWidget(5, 4, m_item_5_4);
                else if (upgrade_type ==5)
                    m_tableWidget->setCellWidget(6, 4, m_item_6_4);
                else if (upgrade_type ==6)
                    m_tableWidget->setCellWidget(7, 4, m_item_7_4);
            }
            //回复消息
        }
        else if(msg.mtype == eSUCC)
        {
            int tmp;
            tmp = atoi(msg.buffer);
            //
            if (upgrade_type>= 0 && upgrade_type<=6 && upgrade_type == tmp)
            {
                QColor color(Qt::black);
                if (upgrade_type == 0)
                {
                    m_item_1_4->setValue(100);
                    m_item_1_5->setTextColor(color);
                    m_item_1_5->setText("Succeed");
                }
                else if (upgrade_type == 1)
                {
                    m_item_2_4->setValue(100);
                    m_item_2_5->setTextColor(color);
                    m_item_2_5->setText("Succeed");
                }
                else if (upgrade_type == 2)
                {
                    m_item_3_4->setValue(100);
                    m_item_3_5->setTextColor(color);
                    m_item_3_5->setText("Succeed");
                }
                else if (upgrade_type == 3)
                {
                    m_item_4_4->setValue(100);
                    m_item_4_5->setTextColor(color);
                    m_item_4_5->setText("Succeed");
                }
                else if (upgrade_type == 4)
                {
                    m_item_5_4->setValue(100);
                    m_item_5_5->setTextColor(color);
                    m_item_5_5->setText("Succeed");
                }
                else if (upgrade_type == 5)
                {
                    m_item_6_4->setValue(100);
                    m_item_6_5->setTextColor(color);
                    m_item_6_5->setText("Succeed");
                }
                else
                {
                    m_item_7_4->setValue(100);
                    m_item_7_5->setTextColor(color);
                    m_item_7_5->setText("Succeed");
                }
                memset(msg.buffer, 0, sizeof(msg.buffer));
                msg.from = eSERVER;
                strcpy(msg.buffer, "a");//ack
                msgsnd(msg_id, &msg, MSG_LEN, 0);

            }
        }
        else if (msg.mtype == eFAIL)
        {
            int tmp;
            tmp = atoi(msg.buffer);
            //
            if (upgrade_type>= 0 && upgrade_type<=6 && upgrade_type == tmp)
            {
                QColor color(Qt::red);
                if (upgrade_type == 0)
                {
                    m_item_1_5->setTextColor(color);
                    m_item_1_5->setText("Failed");
                }
                else if (upgrade_type == 1)
                {
                    m_item_2_5->setTextColor(color);
                    m_item_2_5->setText("Failed");
                }
                else if (upgrade_type == 2)
                {
                    m_item_3_5->setTextColor(color);
                    m_item_3_5->setText("Failed");
                }
                else if (upgrade_type == 3)
                {
                    m_item_4_5->setTextColor(color);
                    m_item_4_5->setText("Failed");
                }
                else if (upgrade_type == 4)
                {
                    m_item_5_5->setTextColor(color);
                    m_item_5_5->setText("Failed");
                }
                else if (upgrade_type == 5)
                {
                    m_item_6_5->setTextColor(color);
                    m_item_6_5->setText("Failed");
                }
                else
                {
                    m_item_7_5->setTextColor(color);
                    m_item_7_5->setText("Failed");
                }
                memset(msg.buffer, 0, sizeof(msg.buffer));
                strcpy(msg.buffer, "a");//ack
                msg.from = eSERVER;
                msgsnd(msg_id, &msg, MSG_LEN, 0);
            }
        }
        else if (msg.mtype == eALL)
        {
            /*
            //enable button
            if (m_box_status[0] == true)
                m_10gCheckBox->setEnabled(true);
            else
                m_10gCheckBox->setEnabled(false);

            if (m_box_status[1] == true)
                m_40gCheckBox->setEnabled(true);
            else
                m_40gCheckBox->setEnabled(false);

            if (m_box_status[2] == true)
                m_100geCheckBox->setEnabled(true);
            else
                m_100geCheckBox->setEnabled(false);

            if (m_box_status[3] == true)
                m_16gCheckBox->setEnabled(true);
            else
                m_16gCheckBox->setEnabled(false);

            if (m_box_status[4] == true)
                m_1geCheckBox->setEnabled(true);
            else
                m_1geCheckBox->setEnabled(false);

            if (m_box_status[5] == true)
                m_2_5gCheckBox->setEnabled(true);
            else
                m_2_5gCheckBox->setEnabled(false);

            if (m_box_status[6] == true)
                m_common_CheckBox->setEnabled(true);
            else
                m_common_CheckBox->setEnabled(false);
            m_cancel_button->setEnabled(true);
            m_upgrade_button->setEnabled(true);
            */
            //取消定时器
            QObject::disconnect(this->m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
            m_timer->stop();
            upgrade_type = -1;
            ::close(read_fifo);

            upgrade_module = atoi(msg.buffer);
            md5sum_failed = upgrade_module&0xFF;

            memset(msg.buffer, 0, sizeof(msg.buffer));
            strcpy(msg.buffer, "a");//ack
            msg.from = eSERVER;
            msgsnd(msg_id, &msg, MSG_LEN, 0);

            //printf("upgraded upgrade_module = 0x%x\n", upgrade_module);
            if (IS_HAVE_UPGRADE_FAILED())
            {
                int i;
                char cmsg[128];
                upgrade_rootfs = false;//m_rootfs_CheckBox_isChecked
                for (i=0; i< sizeof(image_map)/sizeof(IMAGES_MAP); i++)
                {
                    if (IS_BIT_UPGRADE_FAILED(i))
                    {
                        //printf("md5sum_failed = %x\n", md5sum_failed);
                        if (IS_BIT_MD5FAIL(i))
                        {
                            sprintf(cmsg, "%s is a invalid package, check md5 failed, please contact customer service", image_map[i].image_name);
                            //QMessageBox::critical(this, "Invalid package", msg, QMessageBox::Ok);
                            //QApplication a(0, NULL);
                            QString text(cmsg);
                            MessageBox *msg= new MessageBox(QMessageBox::Critical, "Upgrade Failed", text, QMessageBox::NoButton, NULL, Qt::SubWindow);
                            msg->exec();
                        }
                        else
                        {
                        sprintf(cmsg, "%s module upgraded failed, Check USB,Maybe package is not exist or invalid", image_map[i].image_name);

                        MessageBox *msg = new MessageBox(QMessageBox::Critical, "Upgraded failed", cmsg, QMessageBox::NoButton, NULL, Qt::SubWindow);
                        msg->exec();
                        }
                        CLEAR_BIT_UPGRADE_FAILED(i);
                    }
                }
                system("poweroff");
                //ret = 0;
            }
            else
            {
                char cmsg[128];
                if (m_rootfs_CheckBox_isChecked ==  true)
                    upgrade_rootfs = m_rootfs_CheckBox_isChecked;
                if (IS_UPGRADE_NONE())
                {
                    if (upgrade_rootfs)
                        sprintf(cmsg, "New software loaded, system will upgrade rootfs");
                    else
                        sprintf(cmsg, "New software loaded, system will shut down now");
                    //QMessageBox::critical(NULL, "Upgraded Successed", cmsg, QMessageBox::Ok);

                    MessageBox *msg= new MessageBox(QMessageBox::Information, "Upgraded Successed", cmsg, QMessageBox::NoButton, NULL, Qt::SubWindow);
                    QTimer::singleShot(5000,msg,SLOT(close()));
                    msg->exec();
                    if (upgrade_rootfs)
                    {
                        printf("upgrade rootfs\n");
                    }
                    else
                    {
                        printf("poweroff\n");
                        system("poweroff");
                    }
                }
                //ret = 1;
            }
            this->close();
            return;
        }
        else
        {
        }

    }

    memset(buf, 0, 64);
    size = read(this->read_fifo, buf, 6);
    if (size < 0)
    {
        if(errno==EAGAIN)
        {
            printf("no data avlaible\n");
        }
        else
            perror("read fifo failed:");
    }
    else if (size == 0)
    {
        //printf("no fifo data\n");
    }
    else
    {
        int progress = -1;
        clear_str_lws(buf);
        //printf("fifo buf:size = %d, buf = [%s]\n", size, buf);
        sscanf(buf, "%d%", &progress);
        if (progress >= 0 && progress <= 100)
        {
            if (upgrade_type == 0)
            {
                m_item_1_4->setValue(progress);
            }
            else if (upgrade_type == 1)
            {
                m_item_2_4->setValue(progress);
            }
            else if (upgrade_type == 2)
            {
                m_item_3_4->setValue(progress);
            }
            else if (upgrade_type == 3)
            {
                m_item_4_4->setValue(progress);
            }
            else if (upgrade_type == 4)
            {
                m_item_5_4->setValue(progress);
            }
            else if (upgrade_type == 5)
            {
                m_item_6_4->setValue(progress);
            }
            else if (upgrade_type == 6)
            {
                m_item_7_4->setValue(progress);
            }
            else
            {}
        }
    }
}

void update_soft::clear_str_lws(char *s)
{
    if (s == NULL || *s=='\0')
        return;
    int len, i;
    len = strlen(s);
    for (i =0; i < len; i++ )
    {
        if (*s == '\n' || *s == '\t' || *s == '\r')
            *s = ' ';
    }
    return;
}

int update_soft::open_fifo()
{
    char fifo_path[64]={0};
    pid_t pid = getpid();

    sprintf(fifo_path, "/tmp/fifo-%d", pid);
    //sprintf(fifo_path, "/tmp/fifo");
    unlink(fifo_path);
    mkfifo(fifo_path, 0777);

    printf("serv open fifo name:%s\n", fifo_path);
    if ( (this->read_fifo = open(fifo_path, O_RDONLY|O_NONBLOCK|O_CREAT ,0666)) == -1)
    {
        perror("open fifo failed:");
        return -1;
    }
    return 1;
}

int update_soft::create_msgid()
{
    key_t key;
    int msgid;

    if((key=ftok(MSG_FILE,g_ckey))==-1)
    {
        fprintf(stderr,"Creat Key Error:%s\n", strerror(errno));
        return -1;
    }

    if((msgid=msgget(key, PERM|IPC_CREAT|IPC_NOWAIT))==-1)
    {
        fprintf(stderr, "Creat Message Error:%s\n", strerror(errno));
        exit(-1);
    }

    printf("server msgid:%d\n", msgid);
    return msgid;
}

void update_soft::m_upgrade_button_slot()
{
    if (m_10gCheckBox_isChecked)
        SET_10G_UPGRADE();
    if (m_40gCheckBox_isChecked)
        SET_40G_UPGRADE();
    if (m_100geCheckBox_isChecked)
        SET_100GE_UPGRADE();
    if (m_16gCheckBox_isChecked)
        SET_16G_UPGRADE();
    if (m_1geCheckBox_isChecked)
        SET_1GE_UPGRADE();
    if (m_2_5gCheckBox_isChecked)
        SET_2G5_UPGRADE();
    if (m_common_CheckBox_isChecked)
        SET_V300_UPGRADE();

    if (!IS_UPGRADE_NONE())
    {
        if (m_rootfs_CheckBox_isChecked ==  true)
        {
            upgrade_rootfs = m_rootfs_CheckBox_isChecked;
            this->close();
        }
    }
    else
    {
        pid_t pid = -1;


        if (m_10gCheckBox->isEnabled())
            m_box_status[0] = true;
        else
            m_box_status[0] = false;
        if (m_40gCheckBox->isEnabled())
            m_box_status[1] = true;
        else
            m_box_status[1] = false;
        if (m_100geCheckBox->isEnabled())
            m_box_status[2] = true;
        else
            m_box_status[2] = false;
        if (m_16gCheckBox->isEnabled())
            m_box_status[3] = true;
        else
            m_box_status[3] = false;
        if (m_1geCheckBox->isEnabled())
            m_box_status[3] = true;
        else
            m_box_status[3] = false;
        if (m_2_5gCheckBox->isEnabled())
            m_box_status[4] = true;
        else
            m_box_status[4] = false;
        if (m_common_CheckBox->isEnabled())
            m_box_status[5] = true;
        else
            m_box_status[5] = false;
        m_cancel_button->setEnabled(false);
        m_upgrade_button->setEnabled(false);
        m_10gCheckBox->setEnabled(false);
        m_40gCheckBox->setEnabled(false);
        m_100geCheckBox->setEnabled(false);
        m_1geCheckBox->setEnabled(false);
        m_16gCheckBox->setEnabled(false);
        m_2_5gCheckBox->setEnabled(false);
        m_common_CheckBox->setEnabled(false);
        m_rootfs_CheckBox->setEnabled(false);

        m_tableWidget->removeCellWidget(1, 4);
        m_tableWidget->removeCellWidget(2, 4);
        m_tableWidget->removeCellWidget(3, 4);
        m_tableWidget->removeCellWidget(4, 4);
        m_tableWidget->removeCellWidget(5, 4);
        m_tableWidget->removeCellWidget(6, 4);
        m_tableWidget->removeCellWidget(7, 4);

        pid = fork();
        if (pid < 0)
        {
            //exit_no_condition();
            exit(0);
        }
        if (pid == 0)
        {
            if (read_fifo > 0)
                ::close(read_fifo);
            upgrade();
            exit(0);

        }
        signal(SIGCHLD, sig_handle);
        upgrade_pid = pid;
        if (open_fifo() == 0)
            printf("create fifo error\n");
        msg_id = create_msgid();
        QObject::connect(this->m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
        m_timer->start(10);

        //this->close();
    }
}
update_soft::~update_soft()
{

    delete m_titleLable;

    delete m_loading;
    delete m_movie;

    delete m_10gCheckBox;
    delete m_40gCheckBox;
    delete m_100geCheckBox;
    delete m_16gCheckBox;
    delete m_1geCheckBox;
    delete m_2_5gCheckBox;
    delete m_common_CheckBox;

    delete m_cancel_button;
    delete m_upgrade_button;

    delete m_item_0_0;
    delete m_item_0_1;
    delete m_item_0_2;
    delete m_item_0_3;

    delete m_item_1_1;
    delete m_item_1_2;
    delete m_item_1_3;


    delete m_item_2_1;
    delete m_item_2_2;
    delete m_item_2_3;


    delete m_item_3_1;
    delete m_item_3_2;
    delete m_item_3_3;

    delete m_item_4_1;
    delete m_item_4_2;
    delete m_item_4_3;

    delete m_item_5_1;
    delete m_item_5_2;
    delete m_item_5_3;

    delete m_item_6_1;
    delete m_item_6_2;
    delete m_item_6_3;

    delete m_item_7_1;
    delete m_item_7_2;
    delete m_item_7_3;

    delete m_item_8_1;
    delete m_item_8_2;
    delete m_item_8_3;

    delete m_tableWidget;
}
