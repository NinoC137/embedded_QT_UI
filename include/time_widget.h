#ifndef __TIME_WIDGET_H__
#define __TIME_WIDGET_H__

#include "widget.h"

#include <QDateTime>

class Time_Widget : public QWidget{
public:
    Time_Widget(QWidget *parent = nullptr);
    // ~Time_Widget();

private slots:
    void updateTime(void);

private:
    QLabel *dateLabel;
    QLabel *timeLabel;

};

#endif