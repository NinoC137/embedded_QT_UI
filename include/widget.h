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

#include "virtualKeyboard.h"

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

};
#endif // WIDGET_H
