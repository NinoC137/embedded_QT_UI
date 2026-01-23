#ifndef WIFI_WIDGET_H
#define WIFI_WIDGET_H

#include <QWidget>

#include <QProcess>
#include <QString>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "wifiset.h"


class WiFi_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit WiFi_Widget(QWidget* parent = nullptr);
    ~WiFi_Widget();

signals:
    void requestOpenWifiSettings();

private slots:
    void updateWiFiStatus();
    void onWifiButtonClicked();

private: 
    QPushButton *wifiBtn;
    
    QLabel *wifipic;

    QLabel *wifiname;
    QLabel *wifiip;
    
};

#endif // WIFI_WIDGET_H
