#ifndef UPDATE_SOFT_H
#define UPDATE_SOFT_H

#include <QtGui/QMainWindow>
#include <QtGui/QCheckBox>
#include <QtGui/QTableWidget>
#include <QtGui/QLabel>
#include <QtGui/QFont>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QPushButton>
#include <QtGui/QIcon>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGui/QProgressBar>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "messagebox.h"

int extract_thread_status = -1;//-1 is not start, 0 is exit, 1 is run
int module_ready = 0;//bit 1 is 10g, ... , bit 8 is rootfs
unsigned int upgrade_module = 0x0;
unsigned int md5sum_failed = 0x0;
bool upgrade_rootfs = false;

char g_ckey = 'a'; //for msg key
#define   MSG_FILE "/home"
#define   BUFFER 64
//#define   PERM S_IRUSR|S_IWUSR
#define   PERM 0666

enum msg_type
{
    eSTART = 1,
    eSUCC,
    eFAIL,
    eALL
};

enum msgfrom
{
    eCLIENT = 1,
    eSERVER
};

struct msgtype {
    long from;
    long mtype;
    char buffer[BUFFER];
};

#define MSG_LEN  (sizeof(msgtype)-sizeof(long))

enum module_type
{
    e10G = 0,
    e40G,
    e100GE,
    e16G,
    e1GE,
    e2G5,
    eV300
};

int upgrade();

//md5sum
#define SET_10G_MD5FAIL() (md5sum_failed |= 0x1<<0)
#define SET_40G_MD5FAIL() (md5sum_failed |= 0x1<<1)
#define SET_100GE_MD5FAIL() (md5sum_failed |= 0x1<<2)
#define SET_16G_MD5FAIL() (md5sum_failed |= 0x1<<3)
#define SET_1GE_MD5FAIL() (md5sum_failed |= 0x1<<4)
#define SET_2G5_MD5FAIL() (md5sum_failed |= 0x1<<5)
#define SET_V300_MD5FAIL() (md5sum_failed |= 0x1<<6)

#define IS_MD5FAIL_10G() (md5sum_failed & 0x1<<0)
#define IS_MD5FAIL_40G() (md5sum_failed & 0x1<<1)
#define IS_MD5FAIL_100GE() (md5sum_failed & 0x1<<2)
#define IS_MD5FAIL_16G() (md5sum_failed & 0x1<<3)
#define IS_MD5FAIL_1GE() (md5sum_failed & 0x1<<4)
#define IS_MD5FAIL_2G5() (md5sum_failed & 0x1<<5)
#define IS_MD5FAIL_v300() (md5sum_failed & 0x1<<6)
#define IS_BIT_MD5FAIL(x) (md5sum_failed & 0x1<<(x))

#define CLEAR_BIT_MD5FAIL(x) (md5sum_failed &= ~(0x1<<(x)))
//upgrade
#define CLEAR_UPGRADE_FLAG()   (upgrade_module &= 0x0)

#define CLEAR_BIT_INVALID(x) (upgrade_module &= ~(0x1<<(x)))
#define CLEAR_ALL_INVALID() (upgrade_module &= 0xFFFFFF00)

#define SET_BIT_INVALID(x) (upgrade_module |= 0x1<<(x))
#define SET_10G_INVALID() (upgrade_module |= 0x1<<0)
#define SET_40G_INVALID() (upgrade_module |= 0x1<<1)
#define SET_100GE_INVALID() (upgrade_module |= 0x1<<2)
#define SET_16G_INVALID() (upgrade_module |= 0x1<<3)
#define SET_1GE_INVALID() (upgrade_module |= 0x1<<4)
#define SET_2G5_INVALID() (upgrade_module |= 0x1<<5)
#define SET_V300_INVALID() (upgrade_module |= 0x1<<6)

#define IS_VALID_10G() (upgrade_module & 0x1<<0)
#define IS_VALID_40G() (upgrade_module & 0x1<<1)
#define IS_VALID_100GE() (upgrade_module & 0x1<<2)
#define IS_VALID_16G() (upgrade_module & 0x1<<3)
#define IS_VALID_1GE() (upgrade_module & 0x1<<4)
#define IS_VALID_2G5() (upgrade_module & 0x1<<5)
#define IS_VALID_v300() (upgrade_module & 0x1<<6)
#define IS_BIT_INVALID(x) (upgrade_module & 0x1<<(x))
#define IS_HAVE_INVALID() (upgrade_module & 0x7F)

#define SET_10G_UPGRADE() (upgrade_module |= 0x1<<10)
#define SET_40G_UPGRADE() (upgrade_module |= 0x1<<11)
#define SET_100GE_UPGRADE() (upgrade_module |= 0x1<<12)
#define SET_16G_UPGRADE() (upgrade_module |= 0x1<<13)
#define SET_1GE_UPGRADE() (upgrade_module |= 0x1<<14)
#define SET_2G5_UPGRADE() (upgrade_module |= 0x1<<15)
#define SET_V300_UPGRADE() (upgrade_module |= 0x1<<16)

#define IS_UPGRADE_10G() (upgrade_module & 0x1<<10)
#define IS_UPGRADE_40G() (upgrade_module & 0x1<<11)
#define IS_UPGRADE_100GE() (upgrade_module & 0x1<<12)
#define IS_UPGRADE_16G() (upgrade_module & 0x1<<13)
#define IS_UPGRADE_1GE() (upgrade_module & 0x1<<14)
#define IS_UPGRADE_2G5() (upgrade_module & 0x1<<15)
#define IS_UPGRADE_v300() (upgrade_module & 0x1<<16)
#define IS_UPGRADE_NONE() (upgrade_module & (0x7F<<10))

#define SET_10G_UPGRADE_FAILED() (upgrade_module |= 0x1<<20)
#define SET_40G_UPGRADE_FAILED() (upgrade_module |= 0x1<<21)
#define SET_100GE_UPGRADE_FAILED() (upgrade_module |= 0x1<<22)
#define SET_16G_UPGRADE_FAILED() (upgrade_module |= 0x1<<23)
#define SET_1GE_UPGRADE_FAILED() (upgrade_module |= 0x1<<24)
#define SET_2G5_UPGRADE_FAILED() (upgrade_module |= 0x1<<25)
#define SET_V300_UPGRADE_FAILED() (upgrade_module |= 0x1<<26)

#define IS_BIT_UPGRADE_FAILED(x) (upgrade_module & 0x1<<(x+20))
#define IS_UPGRADE_FAILED_10G() (upgrade_module & 0x1<<20)
#define IS_UPGRADE_FAILED_40G() (upgrade_module & 0x1<<21)
#define IS_UPGRADE_FAILED_100GE() (upgrade_module & 0x1<<22)
#define IS_UPGRADE_FAILED_16G() (upgrade_module & 0x1<<23)
#define IS_UPGRADE_FAILED_1GE() (upgrade_module & 0x1<<24)
#define IS_UPGRADE_FAILED_2G5() (upgrade_module & 0x1<<25)
#define IS_UPGRADE_FAILED_v300() (upgrade_module & 0x1<<26)
#define IS_HAVE_UPGRADE_FAILED() (upgrade_module & (0x7F<<20))

#define CLEAR_BIT_UPGRADE_FAILED(x) (upgrade_module &= ~(0x1<<(x+20)))

typedef struct _IMAGES_MAP{
    char image_name[128];
    char ver_file_name[128];
    char type[64];
}IMAGES_MAP;

IMAGES_MAP image_map[]={
    {
        "ux400-local-10g.tar.gz",
        "10g/lib/vendor-info.txt",
        "version.10g",
    },
    {
        "ux400-local-40g.tar.gz",
        "40g/lib/vendor-info.txt",
        "version.40g",
    },
    {
        "ux400-local-100ge.tar.gz",
        "100ge/lib/vendor-info.txt",
        "version.100ge",
    },
    {
        "ux400-local-16g.tar.gz",
        "16g/lib/vendor-info.txt",
        "version.16g",
    },
    {
        "ux400-local-1ge.tar.gz",
        "1ge/lib/vendor-info.txt",
        "version.1ge",
    },
    {
        "ux400-local-2g5.tar.gz",
        "2g5/lib/vendor-info.txt",
        "version.2g5",
    },
    {
        "ux400-local-v300.tar.gz",
        "v300/lib/vendor-info.txt",
        "version.v300",
    },
    {
        "ux400-veex-rootfs-x86.tar.gz",
        "version",
        "version.rootfs",
    }
};

class update_soft : public QMainWindow
{
    Q_OBJECT

public:
    update_soft(QWidget *parent = 0);
    ~update_soft();

    int msg_id;
    int read_fifo;
    int upgrade_type;
    int upgrade_pid;

    QLabel *m_titleLable;
    QLabel *m_loading;
    QMovie *m_movie;
    //QLabel *m_rootfsLable;

    QPushButton *m_10gCheckBox;
    QPushButton *m_40gCheckBox;
    QPushButton *m_100geCheckBox;
    QPushButton *m_16gCheckBox;
    QPushButton *m_1geCheckBox;
    QPushButton *m_2_5gCheckBox;
    QPushButton *m_common_CheckBox;
    QPushButton *m_rootfs_CheckBox;

    bool m_box_status[7];

    bool m_10gCheckBox_isChecked;
    bool m_40gCheckBox_isChecked;
    bool m_100geCheckBox_isChecked;
    bool m_16gCheckBox_isChecked;
    bool m_1geCheckBox_isChecked;
    bool m_2_5gCheckBox_isChecked;
    bool m_common_CheckBox_isChecked;
    bool m_rootfs_CheckBox_isChecked;

    QTableWidget *m_tableWidget;

    QPushButton *m_cancel_button;
    QPushButton *m_upgrade_button;

    QTableWidgetItem *m_item_0_0;
    QTableWidgetItem *m_item_0_1;
    QTableWidgetItem *m_item_0_2;
    QTableWidgetItem *m_item_0_3;
    QTableWidgetItem *m_item_0_4;
    QTableWidgetItem *m_item_0_5;

    QTableWidgetItem *m_item_1_1;
    QTableWidgetItem *m_item_1_2;
    QTableWidgetItem *m_item_1_3;
    QProgressBar *m_item_1_4;
    QTableWidgetItem *m_item_1_5;

    QTableWidgetItem *m_item_2_1;
    QTableWidgetItem *m_item_2_2;
    QTableWidgetItem *m_item_2_3;
    QProgressBar *m_item_2_4;
    QTableWidgetItem *m_item_2_5;

    QTableWidgetItem *m_item_3_1;
    QTableWidgetItem *m_item_3_2;
    QTableWidgetItem *m_item_3_3;
    QProgressBar *m_item_3_4;
    QTableWidgetItem *m_item_3_5;


    QTableWidgetItem *m_item_4_1;
    QTableWidgetItem *m_item_4_2;
    QTableWidgetItem *m_item_4_3;
    QProgressBar *m_item_4_4;
    QTableWidgetItem *m_item_4_5;


    QTableWidgetItem *m_item_5_1;
    QTableWidgetItem *m_item_5_2;
    QTableWidgetItem *m_item_5_3;
    QProgressBar *m_item_5_4;
    QTableWidgetItem *m_item_5_5;

    QTableWidgetItem *m_item_6_1;
    QTableWidgetItem *m_item_6_2;
    QTableWidgetItem *m_item_6_3;
    QProgressBar *m_item_6_4;
    QTableWidgetItem *m_item_6_5;

    QTableWidgetItem *m_item_7_1;
    QTableWidgetItem *m_item_7_2;
    QTableWidgetItem *m_item_7_3;
    QProgressBar *m_item_7_4;
    QTableWidgetItem *m_item_7_5;

    QTableWidgetItem *m_item_8_1;
    QTableWidgetItem *m_item_8_2;
    QTableWidgetItem *m_item_8_3;

    QTimer *m_timer;
    QTimer *m_load_version_timer;

    bool get_soft_version( const int type);
    QString read_file( const QString path);
    void setChecked(bool isChecked);
    int create_msgid(); 
    int open_fifo();
    void clear_str_lws(char *s);
    void show_loaded_status();
    void should_enable_upgrade();
private slots:
    void m_10gCheckBox_slot();
    void m_40gCheckBox_slot();
    void m_100geCheckBox_slot();
    void m_16gCheckBox_slot();
    void m_1geCheckBox_slot();
    void m_2_5gCheckBox_slot();
    void m_common_CheckBox_slot();
    void m_cancel_button_slot();
    void m_upgrade_button_slot();
    void m_rootfs_CheckBox_slot();
    void timer_timeout();
    void load_version_timer();
};

#endif // UPDATE_SOFT_H
