#ifndef __WIFISET_H__
#define __WIFISET_H__   

#include "widget.h"

#include <QDateTime>

class wifiset : public QWidget{
    Q_OBJECT
public:
    wifiset(QWidget *parent = nullptr) : QWidget(parent){ 
        uiinit();
    }
    // ~wifiset();
private slots:
    void onDropdownSSIDChanged(int index);
    void onLineEditClicked();
private:
    void uiinit();

    VirtualKeyboard *m_virtualKeyboard;

    QProcess *m_wifiScanProcess;
    QProcess *m_wifiConnectProcess;

    QDateTime m_scanStartTime;
    QString my_scanResult;
    QString my_currentSSID;

    QTimer *m_connectionTimer;

    QComboBox *m_dropdownSSID;
    QLineEdit *m_textAreaSSID;
    QLineEdit *m_textAreaPW;
    QLineEdit *m_textAreaRSSI;
    QLineEdit *m_textAreaMgnt;

    QPushButton *m_buttonConnect;
    QPushButton *m_buttonDiscon;
    
};

#endif