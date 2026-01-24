#include "wifi_widget.h"

static inline auto disableMouse = [](QWidget *w) {
    w->setAttribute(Qt::WA_TransparentForMouseEvents);
};

void WiFi_Widget::updateWiFiStatus(){
    QLabel* lb_wifiname = this->wifiname;
    QLabel* lb_wifiip = this->wifiip;
    QProcess process;
    process.start("wpa_cli", QStringList() << "status");
    if(!process.waitForStarted() || !process.waitForFinished(3000)){
        qWarning() << "failed to run wpa_cli command";
        return;
    }

    // qDebug("get a update signal");
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    QString ssid;
    QString ip;
    bool scanning = false;
    
    for(const QString& line : lines){
        if(line.contains("wpa_state=SCANNING")){
            scanning = true;
            break;
        }
        else if(line.startsWith("ssid=")) {
            ssid = line.section('=', 1).trimmed();
        }
        else if(line.startsWith("ip_address=")){
            ip = line.section('=', 1).trimmed();
        }
    }
    
    QFont normalFont = lb_wifiname->font();
    if(scanning || ssid.isEmpty()){
        lb_wifiname->setText("----");
        lb_wifiname->setStyleSheet("color= #ffffff; background: transparent; border: none;");
        
        lb_wifiip->setText("No IP");
        lb_wifiip->setStyleSheet("color= #ffffff; background: transparent; border: none;");
    }else{
        lb_wifiname->setText(ssid);
        lb_wifiname->setStyleSheet("color: #f4ea2a; background: transparent; border: none; font-size: 16px;");
        
        if(ip.isEmpty()){
            lb_wifiip->setText("No ip");
            lb_wifiip->setStyleSheet("color: #f4ea2a; background: transparent; border: none;");
        }else{
            lb_wifiip->setText(ip);
            lb_wifiip->setStyleSheet("color: #f4ea2a; background: transparent; border: none; font-size: 14px;");
        }
    }
}

void WiFi_Widget::onWifiButtonClicked()
{

}


WiFi_Widget::WiFi_Widget(QWidget* parent){
    /* ---------- WiFi 区域 ---------- */
    setFixedSize(220, 220);
    setStyleSheet("background-color:rgb(191, 191, 191); border-radius: 20px;");

    wifiBtn = new QPushButton(this);
    wifiBtn->setGeometry(30, 45, 160, 120);
    wifiBtn->setStyleSheet(
        "background-color:rgb(191, 191, 191);"
        "border-radius: 20px;"
        "color: #f4ea2a;"
        "font-size: 24px;"
        "padding: 10px;"
    );

    wifipic = new QLabel(this);
    wifipic->setGeometry(45, 52, 60, 60);
    wifipic->setPixmap(QPixmap("../img/wifi.png"));
    wifipic->setFixedSize(40, 40);
    wifipic->setAlignment(Qt::AlignCenter);
    wifipic->setAttribute(Qt::WA_TransparentForMouseEvents);

    wifiname = new QLabel(this);
    wifiname->setGeometry(45, 90, 120, 40); 
    wifiname->setAlignment(Qt::AlignCenter);
    wifiname->setStyleSheet("color: #f4ea2a; background: transparent; font-size: 12px;");

    wifiip = new QLabel(this);
    wifiip->setGeometry(45, 120, 121, 41);
    wifiip->setAlignment(Qt::AlignCenter);
    wifiip->setStyleSheet("color: #A9A8A8; background: transparent; font-size: 10px;");

    disableMouse(wifiname);
    disableMouse(wifiip);

    updateWiFiStatus();

    //连接信号与槽
    connect(wifiBtn, &QPushButton::clicked, 
        this, &WiFi_Widget::updateWiFiStatus);

    //lamda表达式,上抛信号
    connect(wifiBtn, &QPushButton::clicked, this, [this]() {
        emit requestOpenWifiSettings();
    });
}

WiFi_Widget::~WiFi_Widget(){
    //在qt中,各个组件都是由parent进行统一生命周期管理的
    //我们不需要再额外delete它们,只需要其具有parent就好
    //通常建议使用new QPushButton(this);这种方式来指定parent,也就是在当前组件下追加child
    
    // delete this->wifiname;
    // delete this->wifiip;
    // delete this->wifiBtn;
    // delete this->wifipic;
}
