#include "widget.h"

#include "wifi_widget.h"
#include "sys_status_widget.h"
#include "time_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    /* ========= 顶层窗口属性 ========= */
    setFixedSize(480, 480);
    setWindowTitle("Widget");

    auto *mainGrid = new QGridLayout(this);
    // mainGrid->setSizeConstraint(QLayout::SetFixedSize);

    /* ========= 上层：时间 + 日期 ========= */    
    Time_Widget *timeContainer = new Time_Widget(this);
    
    auto *timeLayout = new QVBoxLayout;
    timeLayout->addWidget(timeContainer);
    mainGrid->addLayout(timeLayout, 0, 0, 2, 2);
    // mainGrid->addWidget(timeContainer, 0, 0, 2, 2);

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

    /* ========= WiFi 设置界面 ========= */
    // 关闭wifi设置界面返回主界面信号槽连接
    wifiSetWidget = new wifiset(this);
    connect(wifiSetWidget,&wifiset::returnToMain,this,&Widget::onWifiWidgetClosed);
}

void Widget::onWifiWidgetClosed()
{
    if (wifiSetWidget) {
        wifiSetWidget->hide();  // 隐藏子界面
    }

    this->show();  // 回到主界面
}