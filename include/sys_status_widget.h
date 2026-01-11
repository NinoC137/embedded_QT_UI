#ifndef __SYS_STATUS_WIDGET_H__
#define __SYS_STATUS_WIDGET_H__

#include "widget.h"

class Sys_Sta_Widget : public QWidget{
public:
    Sys_Sta_Widget(QWidget *parent = nullptr);
    ~Sys_Sta_Widget();

private:
    QPushButton *updateBtn;

    QLabel *statusPic;
    QLabel *currentState;
};

#endif