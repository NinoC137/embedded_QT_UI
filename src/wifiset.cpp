#include "wifiset.h"

#include <QDebug>

#define MAX_CONF_LEN 256
#define MAX_LINE_LEN 256
#define WPA_FILE_PATH "/etc/wpa_supplicant/wpa_supplicant.conf"

static void *wifi_connect_thread_handler(void *arg);
void wificonnect(const char* ssid, const char* password);

struct WifiNetwork {
    QString ssid;
    int signalLevel;
    QString flags;
};

typedef struct {
    char ssid[64];
    char passwd[64];
} wifi_connect_info_t;

QVector<WifiNetwork> m_wifiNetworks;

void wifiset::onResultsFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        // 解析扫描结果
        parseScanResults(m_scanResults);
    } else {
        m_dropdownSSID->clear();
        m_dropdownSSID->addItem("Failed to get results (exit code: " + QString::number(exitCode) + ")");
    }

    m_scanResults.clear();
}

void wifiset::parseScanResults(const QString &results)
{
    m_wifiNetworks.clear();

    // 按行分割结果
    QStringList lines = results.split('\n');

    // 跳过标题行
    if (lines.size() > 1) {
        for (int i = 1; i < lines.size(); i++) {
            QString line = lines[i].trimmed();
            if (line.isEmpty()) continue;

            // 按制表符分割每行数据
            QStringList fields = line.split('\t', Qt::SkipEmptyParts);
            if (fields.size() >= 5) {
                WifiNetwork network;
                network.ssid = fields[4];
                network.signalLevel = fields[2].toInt();
                network.flags = fields[3];

                // 过滤空SSID
                if (!network.ssid.isEmpty()) {
                    m_wifiNetworks.append(network);
                }
            }
        }
    }
}


//槽函数实现
void wifiset::onDropdownSSIDChanged(int index)
{
    if (index >= 0 && index < m_wifiNetworks.size()) {
        // 获取选中的WiFi网络信息
        const WifiNetwork &network = m_wifiNetworks[index];

        // 更新文本框显示SSID
        m_textAreaSSID->setText(network.ssid);
        m_textAreaRSSI->setText(QString::number(network.signalLevel) + " dBm");
        m_textAreaMgnt->setText(network.flags);
    } else {
        // 未选择有效网络时清空文本框
        m_textAreaSSID->clear();
        m_textAreaRSSI->clear();
        m_textAreaMgnt->clear();
    }
}

wifiset::wifiset(QWidget* parent)
    : QWidget(parent)
{
    // 设置主窗口背景颜色
    setStyleSheet("background-color: #0D0D0D;");
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // 创建列表面板
    QWidget *panelList = new QWidget(this);
    panelList->setStyleSheet("background-color: #1F1F1F;");
    QHBoxLayout *listLayout = new QHBoxLayout(panelList);
    // 创建WLAN标签
    QLabel *labelWLAN = new QLabel("WLAN", panelList);
    labelWLAN->setStyleSheet("color: #545454; font-size: 48px;");
    listLayout->addWidget(labelWLAN,1);
    // 创建SSID下拉菜单
    m_dropdownSSID = new QComboBox(panelList);
    m_dropdownSSID->addItems({"---", "---", "---", "---", "---", "---"});
    m_dropdownSSID->setStyleSheet("color: #545454; font-size: 32px; background-color: #000000; border: 1px solid #333333; padding: 10px 20px;border-radius: 10px;");
    connect(m_dropdownSSID, SIGNAL(currentIndexChanged(int)), this, SLOT(onDropdownSSIDChanged(int)));
    listLayout->addWidget(m_dropdownSSID,3);
    mainLayout->addWidget(panelList,1);
    // 创建信息标签和文本框
    QWidget *infoWidget = new QWidget(this);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    // SSID信息
    QHBoxLayout *ssidLayout = new QHBoxLayout();
    QLabel *labelSSID = new QLabel("SSID", infoWidget);
    labelSSID->setStyleSheet("color: #FFFFFF; font-size: 48px;");
    m_textAreaSSID = new QLineEdit("---", infoWidget);
    m_textAreaSSID->setFocusPolicy(Qt::NoFocus);
    m_textAreaSSID->setStyleSheet("color: #A9A8A8; font-size: 32px; background-color: rgba(255, 255, 255, 15); border: 1px solid rgba(255, 255, 255, 25); padding: 10px 20px;border-radius: 10px;");
    ssidLayout->addWidget(labelSSID, 1); // 标签占1份空间
    ssidLayout->addWidget(m_textAreaSSID, 3); // 输入框占3份空间
    infoLayout->addLayout(ssidLayout);
    // Passwd信息
    QHBoxLayout *pwLayout = new QHBoxLayout();
    QLabel *labelPW = new QLabel("Passwd", infoWidget);
    labelPW->setStyleSheet("color: #FFFFFF; font-size: 48px;");
    m_textAreaPW = new QLineEdit("*******", infoWidget);
    m_textAreaPW->setStyleSheet("color: #A9A8A8; font-size: 32px; background-color: rgba(255, 255, 255, 15); border: 1px solid rgba(255, 255, 255, 25); padding: 10px 20px;border-radius: 10px;");
    pwLayout->addWidget(labelPW,1);
    pwLayout->addWidget(m_textAreaPW,3);
    infoLayout->addLayout(pwLayout);
    connect(m_textAreaPW, &QLineEdit::selectionChanged, this, &wifiset::onLineEditClicked);
    // RSSI信息
    QHBoxLayout *rssiLayout = new QHBoxLayout();
    QLabel *labelRSSI = new QLabel("RSSI", infoWidget);
    labelRSSI->setStyleSheet("color: #FFFFFF; font-size: 48px;");
    m_textAreaRSSI = new QLineEdit("--dbm", infoWidget);
    m_textAreaRSSI->setStyleSheet("color: #A9A8A8; font-size: 32px; background-color: rgba(255, 255, 255, 15); border: 1px solid rgba(255, 255, 255, 25); padding: 10px 20px;border-radius: 10px;");
    m_textAreaRSSI->setReadOnly(true);
    rssiLayout->addWidget(labelRSSI,1);
    rssiLayout->addWidget(m_textAreaRSSI,3);
    infoLayout->addLayout(rssiLayout);
    // MGMT信息
    QHBoxLayout *mgmtLayout = new QHBoxLayout();
    QLabel *labelMGMT = new QLabel("PSK", infoWidget);
    labelMGMT->setStyleSheet("color: #FFFFFF; font-size: 48px;");
    m_textAreaMgnt = new QLineEdit("------", infoWidget);
    m_textAreaMgnt->setStyleSheet("color: #A9A8A8; font-size: 32px; background-color: rgba(255, 255, 255, 15); border: 1px solid rgba(255, 255, 255, 25);padding: 10px 20px;border-radius: 10px;");
    m_textAreaMgnt->setReadOnly(true);
    mgmtLayout->addWidget(labelMGMT,1);
    mgmtLayout->addWidget(m_textAreaMgnt,3);
    infoLayout->addLayout(mgmtLayout);
    mainLayout->addWidget(infoWidget,4);
    QString buttonStyle =
            "background-color: #404040;"
            "border-radius: 80px;"  // 增加圆角半径，使按钮更加圆润
            "color: rgb(227, 181, 5);"  // 烫金色字体
            "font-size: 20px;"
            "padding: 10px 10px;";  // 增加左右内边距，使按钮宽度增加

    // 创建按钮面板
    QWidget *panelBtn = new QWidget(this);
    panelBtn->setStyleSheet("background-color: #1F1F1F;");
    QHBoxLayout *btnLayout = new QHBoxLayout(panelBtn);
    // 创建扫描按钮
    QPushButton *buttonScan = new QPushButton("Scan", panelBtn);
    buttonScan->setStyleSheet(buttonStyle);
    connect(buttonScan, SIGNAL(clicked()), this, SLOT(onButtonScanClicked()));
    btnLayout->addWidget(buttonScan);
    // 创建连接按钮
    m_buttonConnect = new QPushButton("Connect", panelBtn);
    m_buttonConnect->setStyleSheet(buttonStyle);
    connect(m_buttonConnect, SIGNAL(clicked()), this, SLOT(onButtonConnectClicked()));
    btnLayout->addWidget(m_buttonConnect);
    // 创建断开连接按钮
    QPushButton *m_buttonDiscon = new QPushButton("Discon", panelBtn);
    m_buttonDiscon->setStyleSheet(buttonStyle);
    connect(m_buttonDiscon, SIGNAL(clicked()), this, SLOT(onButtonDisconClicked()));
    btnLayout->addWidget(m_buttonDiscon);
    // 创建返回按钮
    QPushButton *buttonBack = new QPushButton("Back", panelBtn);
    buttonBack->setStyleSheet(buttonStyle);
    connect(buttonBack, SIGNAL(clicked()), this, SLOT(on_btn_back_clicked()));
    btnLayout->addWidget(buttonBack);
    mainLayout->addWidget(panelBtn,1);

    //创建虚拟键盘
    m_virtualKeyboard = new VirtualKeyboard(this);
    m_virtualKeyboard->hide();
    connect(m_virtualKeyboard, &VirtualKeyboard::closed, m_virtualKeyboard, &QWidget::hide);

    //唤起虚拟键盘
    connect(m_textAreaPW, &QLineEdit::selectionChanged, this, &wifiset::onLineEditClicked);
    //构造函数中链接信号与槽
    connect(m_dropdownSSID, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &wifiset::onDropdownSSIDChanged);


}

void wifiset::onScanOutputReady()
{
    if(m_wifiScanProcess)
        m_scanResults += m_wifiScanProcess->readAllStandardOutput();
}

void wifiset::onScanErrorReady()
{
    if(m_wifiScanProcess)
        qDebug() << "Scan error:" << m_wifiScanProcess->readAllStandardError();
}


void wifiset::scanFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        // 扫描命令成功，等待一段时间后获取结果
        QTimer::singleShot(3000, this, [this]() {
            // 执行scan_results命令获取扫描结果
            QProcess *resultsProcess = new QProcess(this);
            connect(resultsProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                    this, &wifiset::onResultsFinished);
            connect(resultsProcess, &QProcess::readyReadStandardOutput, this, [resultsProcess, this]() {
                m_scanResults += resultsProcess->readAllStandardOutput();
            });

            resultsProcess->start("wpa_cli", QStringList() << "-i" << "wlan0" << "scan_results");
        });
    } else {
        // 扫描命令失败
        m_dropdownSSID->clear();
        m_dropdownSSID->addItem("Scan failed (exit code: " + QString::number(exitCode) + ")");
    }
}

void wifiset::onButtonScanClicked()
{
    m_dropdownSSID->clear();
    m_dropdownSSID->addItem("Scanning...");

    // 记录当前时间，用于超时判断
    m_scanStartTime = QDateTime::currentDateTime();

    m_wifiScanProcess = new QProcess(this);
    connect(m_wifiScanProcess, &QProcess::readyReadStandardOutput, this, &wifiset::onScanOutputReady);
    connect(m_wifiScanProcess, &QProcess::readyReadStandardError, this, &wifiset::onScanErrorReady);
    connect(m_wifiScanProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),this,&wifiset::scanFinished);

    m_wifiScanProcess->start("wpa_cli", QStringList() << "-i" << "wlan0" << "scan");
}


void wifiset::onLineEditClicked()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    if (lineEdit) {
        // 设置当前目标LineEdit
        m_virtualKeyboard->setTargetLineEdit(lineEdit);

        // qDebug() << "VK parent:" << parentWidget();

        // 计算屏幕尺寸
        // QScreen *screen = QGuiApplication::primaryScreen();
        // QRect screenGeometry = screen->geometry();

        // 计算键盘位置（屏幕底部中央）
        QRect screenGeometry = parentWidget()->geometry();
        int keyboardWidth = m_virtualKeyboard->width();
        int keyboardHeight = m_virtualKeyboard->height();
        int x = (screenGeometry.width() - keyboardWidth) / 2;
        int y = screenGeometry.height() - keyboardHeight;

        // 将坐标转换为相对于当前窗口的坐标
        m_virtualKeyboard->move(x,y);

        // 显示键盘
        m_virtualKeyboard->show();
    }
}

static void *wifi_connect_thread_handler(void *arg)
{
    wifi_connect_info_t *wifi = (wifi_connect_info_t *)arg;

    wificonnect(wifi->ssid, wifi->passwd);
    free(wifi);

    return NULL;
}

void wificonnect(const char* ssid, const char* password)
{
    qDebug() << "ssid" << ssid << " " << "password"<< password;
    FILE *wpa_supplicant_pipe;
    char buffer[MAX_CONF_LEN];

    // open wpa_supplicant pipe
    wpa_supplicant_pipe = popen("wpa_cli", "w");
    if (wpa_supplicant_pipe == NULL) {
        perror("popen");
        exit(1);
    }
    printf("connect test\n");
    // set network ssid adn psk
    memset(buffer,0,MAX_CONF_LEN);
    snprintf(buffer, MAX_CONF_LEN, "set_network 0 ssid \"%s\"\n", ssid);
    fputs(buffer, wpa_supplicant_pipe);

    memset(buffer,0,MAX_CONF_LEN);
    snprintf(buffer, MAX_CONF_LEN, "set_network 0 psk \"%s\"\n", password);
    fputs(buffer, wpa_supplicant_pipe);

    // save wifi conf
    fputs("save_config\n", wpa_supplicant_pipe);
    pclose(wpa_supplicant_pipe);

    // save wifi conf to /etc/wpa_supplicant.conf
    FILE *file = fopen(WPA_FILE_PATH, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return ;
    }

    FILE *temp_file = fopen("temp_wpa_supplicant.conf", "w");
    if (temp_file == NULL) {
        printf("Failed to create temporary file.\n");
        fclose(file);
        return ;
    }

    char line[MAX_LINE_LEN];
    int inside_network_block = 0;

    while (fgets(line, MAX_LINE_LEN, file)) {
        // Enter network={} block
        if (strstr(line, "network={")) {
            inside_network_block = 1;
            fputs(line, temp_file);
            continue;
        }
        // Exit network={} block
        if (strstr(line, "}")) {
            inside_network_block = 0;
        }
        // Inside network={} block
        if (inside_network_block) {
            if (strstr(line, "ssid=")) {
                memset(buffer,0,MAX_CONF_LEN);
                sprintf(buffer, "        ssid=\"%s\"\n",ssid);
                fputs(buffer, temp_file);
            }
            else if (strstr(line, "psk=")) {
                memset(buffer,0,MAX_CONF_LEN);
                sprintf(buffer, "        psk=\"%s\"\n",password);
                fputs(buffer, temp_file);
            }
            else {
                fputs(line, temp_file);
            }
        }
        else {
            fputs(line, temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(WPA_FILE_PATH);
    rename("temp_wpa_supplicant.conf", WPA_FILE_PATH);
    //printf("SSID and PSK replaced successfully.\n");

    // reconnect wifi
    // system("killall -9 wpa_cli");
    system("killall -9 wpa_supplicant");
    system("killall -9 udhcpc");

    QThread::sleep(1);
    system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf");
    QThread::sleep(1);
    system("wpa_cli reconfigure &");
    QThread::sleep(5);
    system("udhcpc -i wlan0 &");
    return ;
}

void wifiset::onButtonConnectClicked()
{
    int index = m_dropdownSSID->currentIndex();
    if(index < 0 || index >= m_wifiNetworks.size())
    {
        qDebug() << "connect failed";
        return;
    }

    m_currentSSID = m_wifiNetworks[index].ssid;
    QString passwd = m_textAreaPW->text();

    if (passwd.isEmpty() && !m_wifiNetworks[index].flags.contains("open", Qt::CaseInsensitive)) {
        qDebug() << "input passwd";
        return;
    }

    if(!m_currentSSID.isEmpty() &&
            passwd.length() >= 8 &&
            passwd.length() < 64)
    {
        wifi_connect_info_t *wifi = (wifi_connect_info_t *)malloc(sizeof(wifi_connect_info_t));
        ::strcpy(wifi->ssid, m_currentSSID.toUtf8().constData());
        ::strcpy(wifi->passwd, passwd.toUtf8().constData());

        pthread_t wifi_connect_thread;
        pthread_create(&wifi_connect_thread, NULL, wifi_connect_thread_handler, wifi);
        pthread_detach(wifi_connect_thread);
        qDebug() << "passwd is" << passwd;
    }
}

void wifiset::onButtonDisconClicked()
{
    char command[128];
    snprintf(command, sizeof(command), "wpa_cli -i %s disconnect", "wlan0");
    int ret = system(command);
    if (ret == -1) {
        perror("system");
        return;
    }

    snprintf(command, sizeof(command), "ifconfig %s 0.0.0.0", "wlan0");
    ret = system(command);
    if (ret == -1) {
        perror("system");
        return;
    }

    return;
}

void wifiset::on_btn_back_clicked()
{
    emit returnToMain();
}