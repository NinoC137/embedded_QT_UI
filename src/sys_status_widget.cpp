#include "sys_status_widget.h"

static double getMeminfo(){
    QFile file("/proc/meminfo");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    long long total = 0;
    long long available = 0;

    QString output = in.readAll();
    QStringList lines = output.split('\n');

    for(const auto& line : lines){
        // qDebug() << line;
        if(line.startsWith("MemTotal")){
            // qDebug() << line;
            total = line.split(QRegExp("\\s+"))[1].toLongLong();
        }
        if(line.startsWith("MemAvailable")){
            // qDebug() << line;
            available = line.split(QRegExp("\\s+"))[1].toLongLong();
        }
    }

    if( total == 0 ) {
        qDebug("get mem info fail");
        return 0;
    }else{
        return (1.0 - (double)available / total) * 100;
    }
}

static double getCPUinfo(){
    QProcess process;
    // process.start("top", QStringList() << "-bn1 | grep 'Cpu(s)'");
    process.start("bash", QStringList() << "-c" << "top -bn1 | grep 'Cpu(s)'");
    process.waitForFinished(3000);
    QString output = process.readAllStandardOutput();

    // qDebug() << output;

    QRegularExpression regex(R"((\d+(\.\d+)?)\s*id)");
    QRegularExpressionMatch match = regex.match(output);

    if(match.hasMatch()){
        double idle = match.captured(1).toDouble();
        return 100.0 - idle;        
    }else{
        qDebug("get cpu info fail");
        return 0.0;
    }
}


void Sys_Sta_Widget::updateSysStatus(void){
    QLabel* lb_cpuused = this->sys_cpuUsed;
    QLabel* lb_memused = this->sys_memUsed;
    // qDebug() << getMeminfo();
    // qDebug() << getCPUinfo();

    char membuf[256] = {0};    
    snprintf(membuf, sizeof(membuf), "Mem Used:\t %0.2f %%",  getMeminfo());
    char cpubuf[256] = {0};
    snprintf(cpubuf, sizeof(cpubuf), "CPU Usage:\t %0.2f %%", getCPUinfo());

    lb_cpuused->setText(cpubuf);
    lb_memused->setText(membuf);
}

Sys_Sta_Widget::Sys_Sta_Widget(QWidget *parent)
    : QWidget(parent)
{
    // 保持固定大小
    this->setFixedSize(220, 220);

    // 背景暗色 + 圆角
    setStyleSheet("background-color: #0D0D0D; border-radius: 20px;");

    // ===== 主按钮 =====
    updateBtn = new QPushButton(this);
    updateBtn->setGeometry(30, 45, 160, 120);
    updateBtn->setStyleSheet(
        "background-color:rgb(191, 191, 191);"
        "border-radius: 20px;"
        "color: #f4ea2a;"      // 烫金色
        "font-size: 24px;"
        "padding: 10px;"
    );

    QString exeDir = QCoreApplication::applicationDirPath();
    QString imgPath = exeDir + "/img/sys_status.png";

    // ===== 状态图标 =====
    statusPic = new QLabel(this);
    statusPic->setGeometry(45, 45, 60, 60);
    statusPic->setPixmap(QPixmap(imgPath));
    statusPic->setStyleSheet("background: transparent;");
    statusPic->setAlignment(Qt::AlignCenter);

    // ===== CPU / Memory 标签 =====
    sys_cpuUsed = new QLabel("CPU Used", this);
    sys_cpuUsed->setGeometry(45, 110, 130, 20);
    sys_cpuUsed->setAlignment(Qt::AlignCenter);
    sys_cpuUsed->setStyleSheet("color: #f4ea2a; background: transparent; font-size: 12px;");

    sys_memUsed = new QLabel("Memory Used", this);
    sys_memUsed->setGeometry(45, 135, 130, 20);
    sys_memUsed->setAlignment(Qt::AlignCenter);
    sys_memUsed->setStyleSheet("color: #f4ea2a; background: transparent; font-size: 12px;");

    disableMouse(statusPic);
    disableMouse(sys_cpuUsed);
    disableMouse(sys_memUsed);

    // ===== 信号槽 =====
    connect(updateBtn, &QPushButton::clicked,
         this, &Sys_Sta_Widget::updateSysStatus);

    // 定时刷新
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Sys_Sta_Widget::updateSysStatus);
    timer->start(300); // 0.3s刷新一次

    updateSysStatus();
}


Sys_Sta_Widget::~Sys_Sta_Widget(){
    //在qt中,各个组件都是由parent进行统一生命周期管理的
    //我们不需要再额外delete它们,只需要其具有parent就好
    //通常建议使用new QPushButton(this);这种方式来指定parent,也就是在当前组件下追加child

    // delete this->updateBtn;
    // delete this->statusPic;
    // delete this->sys_cpuUsed;
}