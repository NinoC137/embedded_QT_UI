#include "widget.h"

#include "wifi_widget.h"
#include "sys_status_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    /* ========= 顶层窗口属性 ========= */
    setFixedSize(480, 480);
    setWindowTitle("Widget");

    auto *mainGrid = new QGridLayout(this);
    mainGrid->setSizeConstraint(QLayout::SetFixedSize);

    /* ========= 上层：时间 + 日期 ========= */
    auto *timeLayout = new QVBoxLayout;

    QLabel *time = new QLabel(__DATE__, this);
    time->setMinimumSize(460, 110);
    time->setAlignment(Qt::AlignCenter);

    QLabel *date = new QLabel(__TIME__, this);
    date->setMinimumSize(460, 110);
    date->setAlignment(Qt::AlignCenter);


    timeLayout->addWidget(time);
    timeLayout->addWidget(date);

    mainGrid->addLayout(timeLayout, 0, 1);

    /* ========= 下层：wifi + state ========= */
    auto *bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(3, 3, 3, 3);

    /* ---------- WiFi 区域 ---------- */
    WiFi_Widget *wifiContainer = new WiFi_Widget(this);

    bottomLayout->addWidget(wifiContainer);

    /* ---------- State 区域 ---------- */
    Sys_Sta_Widget *stateContainer = new Sys_Sta_Widget(this);

    bottomLayout->addWidget(stateContainer);

    mainGrid->addLayout(bottomLayout, 2, 1);
}
