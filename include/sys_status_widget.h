#ifndef __SYS_STATUS_WIDGET_H__
#define __SYS_STATUS_WIDGET_H__

#include "widget.h"

class Sys_Sta_Widget : public QWidget{
public:
    Sys_Sta_Widget(QWidget *parent = nullptr);
    ~Sys_Sta_Widget();

private slots:
    void updateSysStatus(void);

private:
    QPushButton *updateBtn;

    QLabel *statusPic;
    QLabel *sys_cpuUsed;
    QLabel *sys_memUsed;
};

#endif