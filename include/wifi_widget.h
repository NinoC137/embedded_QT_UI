#ifndef WIFI_WIDGET_H
#define WIFI_WIDGET_H

#include "widget.h"


class WiFi_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit WiFi_Widget(QWidget* parent = nullptr);
    ~WiFi_Widget();

private slots:
    void updateWiFiStatus();

private:
    QPushButton *wifiBtn;
    
    QLabel *wifipic;

    QLabel *wifiname;
    QLabel *wifiip;
};

#endif // WIFI_WIDGET_H
