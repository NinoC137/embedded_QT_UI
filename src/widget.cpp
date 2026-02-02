#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    /* ========= 顶层窗口属性 ========= */
    setFixedSize(480, 480);
    setWindowTitle("Widget");
    setStyleSheet("background-color: #0D0D0D;");

    /* ========= Stack ========= */
    auto *stack = new QStackedWidget(this);

    
    /* =========================================================
    *  Page 0 : 主界面（Time + WiFi + State）
    * ========================================================= */
   QWidget *mainPage = new QWidget(stack);
   mainPage->setAutoFillBackground(true);
   mainPage->setStyleSheet("background-color: #0D0D0D;");
   
   auto *mainGrid = new QGridLayout(mainPage);
   
//    auto *gif = new gifPlayer(stack);
//    stack->addWidget(gif); // index 0
//    stack->setCurrentWidget(gif);

//    connect(gif, &gifPlayer::playFinished, this, [stack, gif, mainPage]() {
//         // qDebug() << "GIF play finished, switch to main page.";
//         stack->setCurrentWidget(mainPage);
//         stack->removeWidget(gif);
//         gif->deleteLater();
//    });

    /* =======================关闭程序定时重启==============================*/
    QString buttonStyle =
        "background-color:rgb(203, 40, 40);"
        "border-radius: 80px;"  // 增加圆角半径，使按钮更加圆润
        "color: rgb(227, 181, 5);"  // 烫金色字体
        "font-size: 20px;"
        "padding: 10px 10px;";  // 增加左右内边距，使按钮宽度增加
    // 创建按钮面板
    
    /* ========= 上层：时间 + 日期 ========= */
    auto *timeLayout = new QVBoxLayout;
    Time_Widget *timeContainer = new Time_Widget(mainPage);
    
    auto *RebootBtnLayout = new QHBoxLayout;
    QPushButton *buttonReboot = new QPushButton("Reboot", mainPage);
    buttonReboot->setStyleSheet(buttonStyle);
    RebootBtnLayout->addWidget(new QWidget(mainPage)); // 占位，左侧空白
    RebootBtnLayout->addWidget(buttonReboot);
    RebootBtnLayout->addWidget(new QWidget(mainPage)); // 占位，右侧空白
    connect(buttonReboot, SIGNAL(clicked()), this, SLOT(onReboot()));  
    
    timeLayout->addLayout(RebootBtnLayout);
    timeLayout->addWidget(timeContainer);
    mainGrid->addLayout(timeLayout, 1, 0, 2, 2);

    /* ========= 下层：wifi + state ========= */
    auto *bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(3, 3, 3, 3);

    /* ---------- WiFi 区域 ---------- */
    WiFi_Widget *wifiContainer = new WiFi_Widget(mainPage);
    bottomLayout->addWidget(wifiContainer);

    /* ---------- State 区域 ---------- */
    Sys_Sta_Widget *stateContainer = new Sys_Sta_Widget(mainPage);
    bottomLayout->addWidget(stateContainer);

    mainGrid->addLayout(bottomLayout, 3, 0);

    /* =========================================================
     *  Page 1 : WiFi 设置界面
     * ========================================================= */
    wifiset *wifiSetWidget = new wifiset(stack);
    wifiSetWidget->setAutoFillBackground(true);
    wifiSetWidget->setStyleSheet("background-color: #0D0D0D;");


    /* ========= Stack 注册 ========= */
    stack->addWidget(mainPage);      // index 0
    stack->addWidget(wifiSetWidget); // index 1
    // stack->setCurrentWidget(mainPage);

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

void Widget::onReboot()
{
    QString appPath = QCoreApplication::applicationFilePath();

    QProcess::startDetached(
        "bash",
        QStringList() << "-c" << QString("sleep 1 && exec %1 </dev/null >/dev/null 2>&1").arg(appPath)
    );

    qApp->quit();
}