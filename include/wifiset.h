#ifndef __WIFISET_H__
#define __WIFISET_H__   

#include <QWidget>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QGuiApplication>
#include <QThread>

#include "virtualKeyboard.h"

class wifiset : public QWidget{
    Q_OBJECT
public:
    wifiset(QWidget *parent = nullptr) : QWidget(parent){ 
        uiinit();
    }
    // ~wifiset();

    void scanFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onResultsFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void parseScanResults(const QString &results);

signals:
    void returnToMain();

private slots:
    void onDropdownSSIDChanged(int index);
    void onLineEditClicked();
    void onButtonConnectClicked();
    void onButtonScanClicked();
    void onButtonDisconClicked();
    void on_btn_back_clicked();
    void onScanErrorReady();
    void onScanOutputReady();
    
private:
    void uiinit();

    VirtualKeyboard *m_virtualKeyboard;

    QProcess *m_wifiScanProcess;
    QProcess *m_wifiConnectProcess;

    QDateTime m_scanStartTime;
    QString m_scanResults;
    QString m_currentSSID;

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