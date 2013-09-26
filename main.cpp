#include <QtGui/QApplication>
#include "update_soft.h"
#include <QtGui/QMessageBox>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <X11/Xlib.h>
#include <pthread.h>

static int failed_time = 0;

//x server code
#define exit_if_fail(ret_val)		\
do {								\
        if (0 != ((int)(ret_val)))		\
                goto lzExit;				\
} while (0)

char  str_mcookie[256] = "";
char  str_auth_server[256] = "/tmp/xauth-server";
char  str_auth_client[256] = "/tmp/xauth-v300";
volatile bool g_bMonirotExit = FALSE;
Display *_x11_display=NULL;
pid_t pid_x11_server  = -1;
int   gen_mcookie ();
int   gen_authfile (const char* str_authfile);

int   x11_server_invoke ();
int   x11_manager_invoke ();
static int  x11_server_wait ();

static int x11_server_wait()
{
    bool bX11Running=FALSE;

    while ((g_bMonirotExit == FALSE) && (pid_x11_server > 0) &&
            (_x11_display == NULL))
    {
        _x11_display = XOpenDisplay(NULL);
        if (_x11_display == NULL)
        {
            //velog(VELOG_INFO, "X Server not up, wait ...");
            printf("X Server not up, wait ...\n");
            sleep(1);
        }
    }

    if (_x11_display != NULL)
    {
        bX11Running = TRUE;
        //velog(VELOG_INFO, "X Server now up!");
        printf("X Server now up!\n");
    }

    return bX11Running;
}


static int ve_get_pidof(const char* execmd)
{
        int ret_val = 0;
        int exe_pid = -1;

        char* tmpptr = NULL;
        DIR*  dp = NULL;
        struct dirent *entry = NULL;
        char  baseexec[256] = "";
        char  tempname[256] = "";

        /* extract exec's basename */
        ret_val = ((execmd == NULL) || (execmd[0] == '\0')) ? 1 : ret_val;
        exit_if_fail(ret_val);
        strcpy(tempname, execmd);
        tmpptr = basename(tempname);
        ret_val = (tmpptr == NULL) ? 2 : ret_val;
        exit_if_fail(ret_val);
        strcpy(baseexec, tmpptr);

        dp = opendir("/proc");
        ret_val = (dp == NULL) ? 11 : ret_val;
        exit_if_fail(ret_val);

        while ((exe_pid < 0) && ((entry = readdir(dp)) != NULL))
        {
                struct stat statbuf;
                char  fullname_exe[256] = "";
                char  fullname_cmd[256] = "";
                char  baseproc[256] = "";

                /* extract proc exe's fullname_exe and fullname_cmd */
                if (!isdigit(entry->d_name[0]))
                        continue;
                sprintf(tempname, "/proc/%s", entry->d_name);
                if (lstat(tempname, &statbuf) != 0)
                        continue;
                if (!S_ISDIR(statbuf.st_mode))
                        continue;
                /* read fullname_exe */
                sprintf(tempname, "/proc/%s/exe", entry->d_name);
                if (readlink(tempname, fullname_exe, sizeof(fullname_exe)) <= 0)
                {
                        fullname_exe[0] = '\0';
                }
                /* read fullname_cmd */
                sprintf(tempname, "/proc/%s/cmdline", entry->d_name);
                do {
                int fd_cmdline=open(tempname, O_RDONLY);
                if (fd_cmdline < 0)
                {
                        fullname_cmd[0] = '\0';
                }
                else
                {
                        int ret_read=read(fd_cmdline, fullname_cmd, sizeof(fullname_cmd)-1);
                        ret_read = (ret_read < 0) ? 0 : ret_read;
                        fullname_cmd[ret_read] = '\0';
                        close(fd_cmdline);
                }
                } while (0);
                if ((fullname_exe[0] == '\0') && (fullname_cmd[0] == '\0'))
                {
                        continue;
                }

                if ((exe_pid < 0) && (fullname_exe[0] != '\0'))
                {
                        /* extract proc exe's basename */
                        strcpy(tempname, fullname_exe);
                        tmpptr = basename(tempname);
                        if (tmpptr != NULL)
                        {
                                strcpy(baseproc, tmpptr);

                                /* compare whether exec's name and proc's name match */
                                if (strcmp(baseproc, baseexec) == 0)
                                {
                                        exe_pid = atoi(entry->d_name);
                                }
                        }
                }
                if ((exe_pid < 0) && (fullname_cmd[0] != '\0'))
                {
                        /* extract proc exe's basename */
                        strcpy(tempname, fullname_cmd);
                        tmpptr = basename(tempname);
                        if (tmpptr != NULL)
                        {
                                strcpy(baseproc, tmpptr);

                                /* compare whether exec's name and proc's name match */
                                if (strcmp(baseproc, baseexec) == 0)
                                {
                                        exe_pid = atoi(entry->d_name);
                                }
                        }
                }
        }

        /* check whether getpid success */
        ret_val = (exe_pid < 0) ? 101 : ret_val;
        exit_if_fail(ret_val);

lzExit:
        if (dp != NULL)
                closedir(dp);
        return (ret_val == 0) ? exe_pid : -1;
}

int gen_mcookie()
{
        FILE* pfMcookie = NULL;
        char* ptrTmp = NULL;
        char  szTmp[256] = "";

        /* generage mcookie to /tmp/tmp-mcookie file */
        unlink("/tmp/tmp-mcookie");
        system("mcookie > /tmp/tmp-mcookie");

        /* read mcookie to szTemp */
        pfMcookie = fopen("/tmp/tmp-mcookie", "rb");
        if (pfMcookie != NULL)
        {
                fgets(szTmp, sizeof(szTmp)-1, pfMcookie);
                szTmp[sizeof(szTmp)-1] = '\0';
                fclose(pfMcookie);
        }

        /* save mcookie to str_mcookie global variable */
        if ((ptrTmp = strchr(szTmp, '\r')) != NULL)
                ptrTmp[0] = '\0';
        if ((ptrTmp = strchr(szTmp, '\n')) != NULL)
                ptrTmp[0] = '\0';
        strcpy(str_mcookie, szTmp);
        unlink("/tmp/tmp-mcookie");
        return 0;
}

int gen_authfile(const char* str_authfile)
{
        char szTmp[512] = "";

        unlink(str_authfile);

        sprintf(szTmp, "xauth -q -f \"%s\" add :0 . \"%s\"",
                                str_authfile, str_mcookie);
        system(szTmp);
        chmod(str_authfile, 0664);

        return 0;
}

int fork_uiapp_srvs()
{
        /* generate server and client auth files */
        if (access(str_auth_server, R_OK) != 0)
        {
                //velog(VELOG_INFO, "generate auth files ...");
                printf("generate auth files ...\n");
                gen_mcookie();
                gen_authfile(str_auth_server);
                gen_authfile(str_auth_client);
        }
        /* export DISPLAY and XAUTHORITY environment */
        if (getenv("DISPLAY") == NULL)
                setenv("DISPLAY", ":0", 1);
        if (getenv("XAUTHORITY") == NULL)
                setenv("XAUTHORITY", str_auth_client, 1);

        /* execute X11 Server */
        x11_server_invoke();
        return 0;
}

int x11_server_invoke()
{
        int   ret_val = 0;
        pid_t pid_exec = -1;
        char  str_exec_path[256] = "";

        mark_invoke_xsrv:
        /* execute X server application */
        pid_exec = -1;
        if (pid_exec < 0)
        {
                strcpy(str_exec_path, "/usr/bin/X");
                pid_exec = ve_get_pidof(str_exec_path);
        }
        if (pid_exec < 0)
        {
                strcpy(str_exec_path, "/usr/bin/Xorg");
                pid_exec = ve_get_pidof(str_exec_path);
        }
        if (pid_exec > 0)
        {
                printf( "X server already running(pid=%d) ...\n",pid_exec);
                return 0;
                //exit(0);
                //exit_no_condition();
        }
        if (g_bMonirotExit != FALSE)
        {
                exit(0);
                //exit_no_condition();
        }

        /* fork and exec X server application */
        pid_x11_server = fork();
        if (pid_x11_server == 0)
        {
                int   fd_tty = -1;
                pid_t pid_this=getpid();

                /* open tty6 as default IO device */
   /*             fd_tty = open("/dev/tty6", O_RDWR);
                if (fd_tty > 0)
                {
                        close(2);
                        close(1);
                        close(0);
                        dup2(fd_tty, 0);
                        dup2(fd_tty, 1);
                        dup2(fd_tty, 2);
                }
*/
                setpgid(0, pid_this);

                /* exec X server application */
                strcpy(str_exec_path, "/usr/bin/X");
                execl(str_exec_path, "X",
                                         ":0", "-br",
                                         "-auth", str_auth_server,
                                         "vt7",
                                          NULL);
                exit(0);
        }
        if (g_bMonirotExit != FALSE)
        {
            exit(0);
           //     exit_no_condition();
        }

        /* wait X server to run up */
        x11_server_wait();

        if ((g_bMonirotExit == FALSE) &&
                (pid_x11_server < 0))
        {
                //velog(VELOG_WARN, "X server terminated abnormally, redo invoke X server again ...");
                goto mark_invoke_xsrv;
        }

lzExit:
        return (ret_val == 0) ? 0 : -1;
}

//end x server


int mount_disc()
{
    int ret;
    char cmd[256];

    //char cmd[64] = {0};
    sprintf(cmd,"mount -o remount,rw /");
    ret=system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
    {
        printf("failed to exec command %s,%s\n",cmd, strerror(errno));
    }

    printf("mount disc...\n");
    system("mkdir /mnt/usb >/dev/null 2>&1");
    system("umount /mnt/usb >/dev/null 2>&1");
    strcpy(cmd,"mount /dev/sd[bc][1-9] /mnt/usb -t vfat >/dev/null 2>&1");
    ret=system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
    {
        //QApplication a(0, NULL);

        //#QMessageBox::critical(NULL, "Not Found USB", "Please insert USB than reboot system", QMessageBox::Ok);
        MessageBox *msg = new MessageBox(QMessageBox::Critical, "Not Found USB", "Please insert USB, then reboot system", QMessageBox::NoButton, NULL, Qt::SubWindow);
        msg->exec();
        //a.exec();
        system("poweroff");
        exit(0);
    }
    return 0;
}

int umount_disc()
{
    printf("unmount disc...\n");
    system("umount /dev/usb >/dev/null");

    return 0;
}

int update_application(char *image_name)
{
    int ret;
    char dir[256];
    char cmd[256];
    pid_t ppid;

    printf("----------------------------------\n");
    printf("Try to upgrade %s...\n",image_name);

    printf("Checking the new image %s \n",image_name);
    sprintf(cmd,"tar -tf /mnt/usb/ux400-local-%s.tar.gz >>/dev/null", image_name);
    ret = system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))

    {
        printf("the image %s is a invalid package\n", image_name);
        ret = -1;
        goto ERR1;
    }
    printf("The new image %s is a valid package\nRemoving the old module\n", image_name);

    ppid = getppid();

    sprintf(cmd,"mkdir -p /usr/local/%s >>/dev/null",image_name);
    ret=system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
    {
        printf("system not have this old module,failed to exute command %s,%s\n",cmd, strerror(errno));
        printf("install the image %s failed\n", image_name);
        ret = -1;
        goto ERR1;
    }

    sprintf(cmd,"rm -rf /usr/local/%s-bak;cp -a /usr/local/%s /usr/local/%s-bak >>/dev/null",image_name, image_name, image_name);
    ret=system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
    {
        printf("backup the old module failed; failed to exute command %s,%s\n",cmd, strerror(errno));
        printf("install the image %s failed\n", image_name);
        ret = -1;
        goto ERR1;
    }
    if (strcmp(image_name, "100ge") == 0)
        sprintf(cmd,"rm -fr /usr/local/%s /usr/local/40ge /usr/local/%s.md5 >>/dev/null",image_name);
    else
        sprintf(cmd,"rm -fr /usr/local/%s /usr/local/%s.md5 >>/dev/null",image_name);
    ret=system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
    {
        printf("remove the old module failed; failed to exute command %s,%s\n",cmd, strerror(errno));
        printf("install the image %s failed\n", image_name);
        ret = -1;
        goto ERR1;
    }

    getcwd(dir,sizeof(dir));
    chdir("/usr/local");
    //sprintf(cmd,"bar -E /tmp/ux400-local-%s.tar.gz |tar zxf - >/dev/null", image_name);

    printf("Installing the new image %s\n",image_name);
    sprintf(cmd,"bar -f /tmp/fifo-%d -E /mnt/usb/ux400-local-%s.tar.gz |tar zxf - >>/dev/null",ppid, image_name);
    ret = system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
    {
        printf("install the image %s failed\n", image_name);
        sprintf(cmd,"rm -rf /usr/local/%s;mv /usr/local/%s-bak /usr/local/%s >>/dev/null",image_name, image_name, image_name);
        int tmp_ret=0;
        tmp_ret=system(cmd);
        if(tmp_ret == -1 || !WIFEXITED(tmp_ret) || 0 != WEXITSTATUS(tmp_ret))
        {
            printf("failed to exute command %s,%s\n",cmd, strerror(errno));
            ret = -1;
            goto ERR0;
        }
    }
    else
    {
        if (strcmp(image_name, "100ge") == 0)
            sprintf(cmd,"rm -rf /usr/local/%s-bak /home/%s /home/40ge >>/dev/null", image_name, image_name);
        else if (strcmp(image_name, "v300") != 0)
            sprintf(cmd,"rm -rf /usr/local/%s-bak /home/%s >>/dev/null", image_name, image_name);
        else
            sprintf(cmd,"rm -rf /usr/local/%s-bak >>/dev/null", image_name, image_name);
        ret=system(cmd);
        printf("Install the image %s successed, will check md5sum\n", image_name);
        char md5file[128] = {0};
        sprintf(md5file, "/usr/local/%s.md5", image_name);
        if((access(md5file,F_OK))!=-1)
        {
            sprintf(cmd,"md5sum -c --quiet %s", md5file);
            ret=system(cmd);
            if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
            {
                //check failed
                ret = 1;
                failed_time++;
                printf("md5sum %s check failed\n", md5file);
            }
            else
            {
                printf("md5sum %s check successed\n", md5file);
                ret = 0;
            }
            sprintf(cmd,"rm -rf %s", md5file);
            system(cmd);
        }
        else
        {
            printf("not have %s\n", md5file);
            //not find md5 file
            ret = 0;
        }
    }

    ERR0:
    chdir(dir);
    ERR1:
    return ret;
}

void clear_tmp_file()
{
    int cnt,i;
    char cmd[256];

    cnt = sizeof(image_map)/sizeof(IMAGES_MAP);
    for(i=0; i < cnt; i++)
    {
        memset(cmd, 0 ,sizeof(cmd));
        system("rm -rf /tmp/10g /tmp/40g /tmp/100ge /tmp/16g /tmp/1ge /tmp/2g5 /tmp/v300");
        sprintf(cmd,"rm -rf /tmp/%s /tmp/%s >>/dev/null 2>&1", image_map[i].image_name, image_map[i].type);
        system(cmd);
    }
}

int disperse_pack()
{
    int ret,cnt,i,hit;
    char path[256],cmd[256],dir[256];

    hit=0;

    cnt = sizeof(image_map)/sizeof(IMAGES_MAP);
    for(i=0; i < cnt; i++)
    {
        sprintf(path,"/mnt/usb/%s",image_map[i].image_name);
        ret = access(path,F_OK);
        if(0 == ret)
        {
            hit = 1;
            memset(cmd, 0x0, sizeof(cmd));
            //sprintf(cmd,"cp /mnt/usb/%s /tmp/%s", image_map[i].image_name, image_map[i].image_name);
            //system(cmd);
            getcwd(dir,sizeof(dir));
            chdir("/tmp");
            printf("Checking the version of %s\n", image_map[i].image_name);
            //sprintf(cmd, "gunzip -dc /tmp/%s | tar xf - %s", image_map[i].image_name, image_map[i].ver_file_name);
            sprintf(cmd, "tar xf /mnt/usb/%s %s", image_map[i].image_name, image_map[i].ver_file_name);
            ret = system(cmd);
            if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
            {
                printf("The image %s is not the right image, please check it\n", image_map[i].image_name);
                SET_BIT_INVALID(i);
            }
            else
            {
                rename(image_map[i].ver_file_name, image_map[i].type);

            }

            chdir(dir);
        }
        module_ready |= 1<<i;
        //printf("thread module_ready = 0x%x\n", module_ready);
    }

    return hit;
}

int init()
{
    int ret;
    ret=mount_disc();
    if(ret)
        return -1;

    //ret=disperse_pack();

    return 0;
}

int client_get_msgid()
{
    int msgid;
    key_t key;

    if((key=ftok(MSG_FILE, g_ckey))==-1)
    {
        fprintf(stderr,"client:Creat Key Error:%s\n", strerror(errno));
        exit(-1);
    }

    if((msgid=msgget(key, PERM))==-1)
    {
        fprintf(stderr,"client:Creat Message  Error:%s\n", strerror(errno));
        exit(-1);
    }

    return msgid;
}
int is_ack_msg(struct msgtype msg, int msg_type)
{

    if (msg.mtype == msg_type )
        return !strcmp(msg.buffer, "a");
    else
        return 0;
}

//return value:
// 1 : success
// 0 : failed
int send_req(int msgid, int upgrade_type)
{
    struct msgtype msg;
    msg.from = eCLIENT;
    msg.mtype = eSTART;
    memset(msg.buffer, 0, sizeof(msg.buffer));
    sprintf(msg.buffer, "%d", upgrade_type);

    printf("client:req msgid=%d, msgtype=%d, msgbuf=%s\n", msgid, msg.mtype, msg.buffer);
    if (msgsnd(msgid, &msg, MSG_LEN, 0) < 0)
    {
        perror("client:send request msg failed:");
        return 0;
    }
    return 1;
}

//return value:
// 1 : success
// 0 : failed
int request(int msgid, int upgrade_type)
{
    struct msgtype msg;
     if (send_req(msgid, upgrade_type) == 0)
    {
         printf("client:request %d upgrade failed\n", upgrade_type);
         return 0;
     }

     int i = 0;
     int size = 0;
     while (i < 10)
     {
         size = msgrcv(msgid, &msg, MSG_LEN, eSERVER, IPC_NOWAIT);
         if (size < 0)
         {
             //return 0;
         }
         else if (size == 0)
         {
             i++;
             printf("client: wait req ack msg\n");
             usleep(200*1000);
         }
         else
         {
             printf("client:req ack, type = %d ,buf:%s\n", msg.mtype, msg.buffer);
             if (is_ack_msg(msg, eSTART) == 1)
                 return 1;
         }
     }

     perror("client:recv req ack msg failed:");
     return 0;
}

int success(int msgid, int upgrade_type)
{
    struct msgtype msg;
    msg.from = eCLIENT;
    msg.mtype = eSUCC;
    sprintf(msg.buffer, "%d", upgrade_type);

    if (msgsnd(msgid, &msg, MSG_LEN, 0) != 0)
    {
        perror("client:send succ msg failed:");
        return 0;
    }

    int i = 0;
    int size = 0;
    while (i < 10)
    {
        size = msgrcv(msgid, &msg, MSG_LEN,  eSERVER, IPC_NOWAIT);
        if (size < 0)
        {

            //return 0;
        }
        else if (size == 0)
        {
            i++;
            usleep(200*1000);
        }
        else
        {
            printf("client:succ ack, type = %d ,buf:%s\n", msg.mtype, msg.buffer);
            if (is_ack_msg(msg, eSUCC) == 1)
                return 1;
        }
    }

    perror("client:recv succ ack msg failed:");
    return 0;
}

int failed(int msgid, int upgrade_type)
{
    struct msgtype msg;
    msg.mtype = eFAIL;
    msg.from = eCLIENT;
    sprintf(msg.buffer, "%d", upgrade_type);

    if (msgsnd(msgid, &msg, MSG_LEN, 0) != 0)
    {
        perror("client:send failed msg failed:");
        return 0;
    }

    int i = 0;
    int size = 0;
    while (i < 10)
    {
        size = msgrcv(msgid, &msg, MSG_LEN, eSERVER, IPC_NOWAIT);
        if (size < 0)
        {

            //return 0;
        }
        else if (size == 0)
        {
            i++;
            usleep(200*1000);
        }
        else
        {
            printf("client:failed ack, type = %d ,buf:%s\n", msg.mtype, msg.buffer);
            if (is_ack_msg(msg, eFAIL) == 1)
                return 1;
        }
    }

    perror("client: recv failed ack msg failed:");
    return 0;
}

int finished(int msgid)
{
    struct msgtype msg;
    msg.from = eCLIENT;
    msg.mtype = eALL;
    CLEAR_ALL_INVALID();
    upgrade_module |= md5sum_failed&0xFF;
    sprintf(msg.buffer, "%d", upgrade_module);

    if (msgsnd(msgid, &msg, MSG_LEN, 0) != 0)
    {
        perror("client:send finished msg failed:");
        return 0;
    }

    int i = 0;
    int size = 0;
    while (i < 10)
    {
        size = msgrcv(msgid, &msg, MSG_LEN, eSERVER, IPC_NOWAIT);
        if (size < 0)
        {

            //return 0;
        }
        else if (size == 0)
        {
            i++;
            usleep(200*1000);
        }
        else
        {
            printf("client:finished ack, type = %d ,buf:%s\n", msg.mtype, msg.buffer);
            if (is_ack_msg(msg, eALL) == 1)
                return 1;
        }
    }

    perror("cleint:recv finished ack msg failed:");
    return 0;
}

int upgrade()
{
    int ret;

    /* update the application*/
    char cmd[64] = {0};
    sprintf(cmd,"mount -o remount,rw /usr/local/");
    ret=system(cmd);
    if(ret == -1 || !WIFEXITED(ret) || 0 != WEXITSTATUS(ret))
    {
        printf("failed to exec command %s,%s\n",cmd, strerror(errno));
    }

    int msgid = client_get_msgid();
    printf("client:get IPC message queue success,msgid:%d\n", msgid);

    if (IS_UPGRADE_10G())
    {
        failed_time = 0;

        if (request(msgid, e10G))
        {
       REQ_10G:
            ret=update_application("10g");
            if(ret == -1)
            {
                SET_10G_UPGRADE_FAILED();
                failed(msgid, e10G);
            }
            else if (ret == 1)
            {
                if (failed_time < 3)
                    goto REQ_10G;
                else
                {
                    //messagebox
                    SET_10G_MD5FAIL();
                    SET_10G_UPGRADE_FAILED();
                    failed(msgid, e10G);
                }
            }
            else
            {
                success(msgid, e10G);
            }
        }
        else
        {
            SET_10G_UPGRADE_FAILED();
            failed(msgid, e10G);
        }

    }

    //printf("10g upgrade_module = 0x%x\n", upgrade_module);
    if (IS_UPGRADE_40G())
    {
        failed_time = 0;
        if (request(msgid, e40G))
        {
        REQ_40G:
            ret=update_application("40g");
            if(ret == -1)
            {
                SET_40G_UPGRADE_FAILED();
                failed(msgid, e40G);

            }
            else if (ret == 1)
            {
                if (failed_time < 3)
                    goto REQ_40G;
                else
                {
                    SET_40G_MD5FAIL();
                    SET_40G_UPGRADE_FAILED();
                    failed(msgid, e40G);
                }
            }
            else
            {
                success(msgid, e40G);
            }
        }
        else
        {
            SET_40G_UPGRADE_FAILED();
            failed(msgid, e40G);
        }
    }
    //printf("40g upgrade_module = 0x%x\n", upgrade_module);
    if (IS_UPGRADE_100GE())
    {
        failed_time = 0;
        if (request(msgid, e100GE))
        {
        REQ_100GE:
            ret=update_application("100ge");
            if(ret == -1)
            {
                SET_100GE_UPGRADE_FAILED();
                failed(msgid, e100GE);
            }
            else if (ret == 1)
            {
                if (failed_time < 3)
                    goto REQ_100GE;
                else
                {
                    SET_100GE_MD5FAIL();
                    SET_100GE_UPGRADE_FAILED();
                    failed(msgid, e100GE);
                }
            }
            else
            {
                success(msgid, e100GE);
            }
        }
        else
        {
            SET_100GE_UPGRADE_FAILED();
            failed(msgid, e100GE);
        }
    }

    //printf("100ge upgrade_module = 0x%x\n", upgrade_module);
    if (IS_UPGRADE_16G())
    {
        failed_time = 0;
        if (request(msgid, e16G))
        {
        REQ_16G:
            ret=update_application("16g");
            if(ret == -1)
            {
                SET_16G_UPGRADE_FAILED();
                failed(msgid, e16G);
            }
            else if (ret == 1)
            {
                if (failed_time < 3)
                    goto REQ_16G;
                else
                {
                    SET_16G_MD5FAIL();
                    SET_16G_UPGRADE_FAILED();
                    failed(msgid, e16G);
                }
            }
            else
            {
                success(msgid, e16G);
            }
        }
        else
        {
            SET_16G_UPGRADE_FAILED();
            failed(msgid, e16G);
        }
    }
//printf("16g upgrade_module = 0x%x\n", upgrade_module);
    if (IS_UPGRADE_1GE())
    {
        failed_time = 0;
        if (request(msgid, e1GE))
        {
        REQ_1GE:
            ret=update_application("1ge");
            if(ret == -1)
            {
                SET_1GE_UPGRADE_FAILED();
                failed(msgid, e1GE);
            }
            else if (ret == 1)
            {
                if (failed_time < 3)
                    goto REQ_1GE;
                else
                {
                    SET_1GE_MD5FAIL();
                    SET_1GE_UPGRADE_FAILED();
                    failed(msgid, e1GE);
                }
            }
            else
            {
                success(msgid, e1GE);
            }
        }
        else
        {
            SET_1GE_UPGRADE_FAILED();
            failed(msgid, e1GE);
        }
    }
printf("1ge upgrade_module = 0x%x\n", upgrade_module);
    if (IS_UPGRADE_2G5())
    {
        failed_time = 0;
        if (request(msgid, e2G5))
        {
        REQ_2G5E:
            ret=update_application("2g5");
            if(ret == -1)
            {
                SET_2G5_UPGRADE_FAILED();
                failed(msgid, e2G5);
            }
            else if (ret == 1)
            {
                if (failed_time < 3)
                    goto REQ_2G5E;
                else
                {
                    SET_2G5_MD5FAIL();
                    SET_2G5_UPGRADE_FAILED();
                    failed(msgid, e2G5);
                }
            }
            else
            {
                success(msgid, e2G5);
            }
        }
        else
        {
            SET_2G5_UPGRADE_FAILED();
            failed(msgid, e2G5);
        }

    }


    if (IS_UPGRADE_v300())
    {
        failed_time = 0;
        if (request(msgid, eV300))
        {
        REQ_V300:
            ret=update_application("v300");
            if(ret == -1)
            {
                SET_V300_UPGRADE_FAILED();
                failed(msgid, eV300);
            }
            else if (ret == 1)
            {
                if (failed_time < 3)
                    goto REQ_V300;
                else
                {
                    SET_V300_MD5FAIL();
                    SET_V300_UPGRADE_FAILED();
                    failed(msgid, eV300);
                }
            }
            else
            {
                success(msgid, eV300);
            }
        }
        else
        {
            SET_V300_UPGRADE_FAILED();
            failed(msgid, eV300);
        }

    }
    finished(msgid);
    sprintf(cmd,"mount -o remount,ro /usr/local/");
    system(cmd);
    printf("upgrade finished\n");
    //printf("client : md5sum_failed = %x\n", md5sum_failed);

    return 0;
}

void *extract_version(void*)
{
    extract_thread_status = 1;
    disperse_pack();
    extract_thread_status = 0;
}

//int show_upgrade_ui(int argc, char *argv[])
int show_upgrade_ui()
{
    //QApplication a(argc, argv);
    int ret;
    pthread_t tid;
    fork_uiapp_srvs();
    QApplication a(0, NULL);

    clear_tmp_file();
    if (init() != 0)
    {
        printf("ux400 upgrade init failed\n");
        return 0;
    } 

    update_soft w;
    w.show();
    pthread_create(&tid, NULL, extract_version, NULL);
    a.exec();

    /*
    upgrade();
    printf("upgraded upgrade_module = 0x%x\n", upgrade_module);

    if (IS_HAVE_UPGRADE_FAILED())
    {
        int i;
        char msg[128];
        for (i=0; i< sizeof(image_map)/sizeof(IMAGES_MAP); i++)
        {
            if (IS_BIT_UPGRADE_FAILED(i))
            {
                sprintf(msg, "%s module upgraded failed, maybe package is invalid", image_map[i].image_name);
                QMessageBox::critical(NULL, "Upgraded failed", msg, QMessageBox::Ok);
                CLEAR_BIT_UPGRADE_FAILED(i);
            }
        }
        ret = 0;
    }
    else
    {
        if (IS_UPGRADE_NONE())
        {
            QMessageBox *box = new QMessageBox(QMessageBox::Information,"Upgrade Successed","Upgrade Successed, system will halt after 10s");
            QTimer::singleShot(10000,box,SLOT(close()));
            box->exec();//box->show();都可以
            printf("halt\n");
        }
        ret = 1;
    }
    */
    umount_disc();
    clear_tmp_file();

    printf("upgrade rootfs : %d\n", upgrade_rootfs);
    exit(upgrade_rootfs);
    //return ret;
}

int main()
{
    show_upgrade_ui();
}
