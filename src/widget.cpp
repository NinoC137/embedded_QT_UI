#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    /* ========= 顶层窗口属性 ========= */
    setFixedSize(480, 480);
    setWindowTitle("Widget");

    /* ========= Stack ========= */
    auto *stack = new QStackedWidget(this);

    /* =========================================================
     *  Page 0 : 主界面（Time + WiFi + State）
     * ========================================================= */
    QWidget *mainPage = new QWidget(stack);
    auto *mainGrid = new QGridLayout(mainPage);

    /* ========= 上层：时间 + 日期 ========= */
    Time_Widget *timeContainer = new Time_Widget(mainPage);
    auto *timeLayout = new QVBoxLayout;
    timeLayout->addWidget(timeContainer);
    mainGrid->addLayout(timeLayout, 0, 0, 2, 2);

    /* ========= 下层：wifi + state ========= */
    auto *bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(3, 3, 3, 3);

    /* ---------- WiFi 区域 ---------- */
    WiFi_Widget *wifiContainer = new WiFi_Widget(mainPage);
    bottomLayout->addWidget(wifiContainer);

    /* ---------- State 区域 ---------- */
    Sys_Sta_Widget *stateContainer = new Sys_Sta_Widget(mainPage);
    bottomLayout->addWidget(stateContainer);

    mainGrid->addLayout(bottomLayout, 2, 1);

    /* =========================================================
     *  Page 1 : WiFi 设置界面
     * ========================================================= */
    wifiset *wifiSetWidget = new wifiset(stack);

    /* ========= Stack 注册 ========= */
    stack->addWidget(mainPage);      // index 0
    stack->addWidget(wifiSetWidget); // index 1
    stack->setCurrentWidget(mainPage);

    /* ========= 顶层布局 ========= */
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->addWidget(stack);


    /* =========================================================
     *  信号与槽（页面切换的唯一入口）
     * ========================================================= */

    // WiFi 按键 → 打开 WiFi 设置页
    connect(wifiContainer,
            &WiFi_Widget::requestOpenWifiSettings,
            this,
            [stack, wifiSetWidget]() {
                stack->setCurrentWidget(wifiSetWidget);
            });

    // WiFi 设置页 → 返回主界面
    connect(wifiSetWidget,
            &wifiset::returnToMain,
            this,
            [stack, mainPage]() {
                stack->setCurrentWidget(mainPage);
            });
}


void Widget::showWifiSettings()
{
    stack->setCurrentWidget(wifiSetWidget);
}

void Widget::showMainPage()
{
    stack->setCurrentWidget(mainPage);
}

void Widget::onWifiWidgetClosed()
{
    if (wifiSetWidget) {
        wifiSetWidget->hide();  // 隐藏子界面
    }

    this->show();  // 回到主界面
}