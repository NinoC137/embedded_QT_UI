#ifndef WIDGET_H
#define WIDGET_H

#include <iostream>

#include <QWidget>

#include <QProcess>
#include <QString>
#include <QDebug>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <QPixmap>
#include <QFile>
#include <QRegularExpression>
#include <QTimer>

#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QScreen>
#include <QGuiApplication>
#include <QThread>
#include <QStackedWidget>

#include "virtualKeyboard.h"
#include "wifi_widget.h"
#include "sys_status_widget.h"
#include "time_widget.h"
#include "wifiset.h"

class QLabel;
class QPushButton;

inline auto disableMouse = [](QWidget *w) {
    w->setAttribute(Qt::WA_TransparentForMouseEvents);
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    //~Widget();
private slots:
    void onWifiWidgetClosed();
    void showMainPage();
    void showWifiSettings();
    void onReboot();

private:
    QStackedWidget *stack;
    QWidget        *mainPage;
    WiFi_Widget    *wifiWidget;
    wifiset        *wifiSetWidget;
};
#endif // WIDGET_H
