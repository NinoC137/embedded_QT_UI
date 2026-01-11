#include "sys_status_widget.h"

Sys_Sta_Widget::Sys_Sta_Widget(QWidget *parent){
    this->setFixedSize(220, 220);

    updateBtn = new QPushButton(this);
    updateBtn->setGeometry(30, 45, 160, 120);

    statusPic = new QLabel("TextLabel", this);
    statusPic->setGeometry(45, 60, 60, 60);
    statusPic->setPixmap(QPixmap("../img/sys_status.png"));
    statusPic->setFixedSize(40, 40);
    statusPic->setAlignment(Qt::AlignCenter);
    statusPic->setAttribute(Qt::WA_TransparentForMouseEvents);

    currentState = new QLabel("TextLabel", this);
    currentState->setGeometry(45, 120, 121, 41);
    currentState->setAlignment(Qt::AlignCenter);

    disableMouse(statusPic);
    disableMouse(currentState);
}

Sys_Sta_Widget::~Sys_Sta_Widget(){
    //在qt中,各个组件都是由parent进行统一生命周期管理的
    //我们不需要再额外delete它们,只需要其具有parent就好
    //通常建议使用new QPushButton(this);这种方式来指定parent,也就是在当前组件下追加child

    // delete this->updateBtn;
    // delete this->statusPic;
    // delete this->currentState;
}