#include "time_widget.h"

void Time_Widget::updateTime(void){
    static int net_update_count = 0;
    
    static QDateTime utcDataTime;
    static QDateTime current;
    static QTime currentTime;
    QString timeStr;

    if(net_update_count > 60*60 || net_update_count == 0){
        net_update_count = 1;

        // qDebug("update time from net clock");

        utcDataTime = QDateTime::currentDateTimeUtc();
        current = utcDataTime.addSecs(8 * 3600); // Convert to Beijing Time (UTC+8)
    
        currentTime = current.time();
        timeStr = QString("%1:%2:%3")
                              .arg(currentTime.hour(), 2, 10, QChar('0'))
                              .arg(currentTime.minute(), 2, 10, QChar('0'))
                              .arg(currentTime.second(), 2, 10, QChar('0'));
    } else {
        net_update_count++;
       
        currentTime = currentTime.addSecs(1);

        timeStr = QString("%1:%2:%3")
                              .arg(currentTime.hour(), 2, 10, QChar('0'))
                              .arg(currentTime.minute(), 2, 10, QChar('0'))
                              .arg(currentTime.second(), 2, 10, QChar('0'));    
    }


    // qDebug() << "Current Time:" << timeStr;

    QLabel *lb_date = this->dateLabel;
    QLabel *lb_time = this->timeLabel;

    QDate currentDate = current.date();
    // qDebug() << "Current Date:" << currentDate.toString();

    QString dateStr = currentDate.toString("yyyy-MM-dd dddd");
    // qDebug() << "Formatted Date:" << dateStr;

    lb_date->setText(dateStr);
    lb_time->setText(timeStr);
}

Time_Widget::Time_Widget(QWidget *parent)
    : QWidget(parent)
{
    timeLabel = new QLabel(__DATE__, this);
    // timeLabel->setMinimumSize(460, 110);
    timeLabel->setAlignment(Qt::AlignCenter);
    setStyleSheet("background-color: #0D0D0D; border-radius: 20px;");

    dateLabel = new QLabel(__TIME__, this);
    // dateLabel->setMinimumSize(460, 110);
    dateLabel->setAlignment(Qt::AlignCenter);

    disableMouse(dateLabel);
    disableMouse(timeLabel);

    timeLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: #f4ea2a;");
    dateLabel->setStyleSheet("font-size: 20px; color: #A9A8A8;");
    timeLabel->setFixedHeight(120);
    dateLabel->setFixedHeight(60);


    // 垂直布局
    auto *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(-18);
    vLayout->addWidget(timeLabel);
    vLayout->addWidget(dateLabel);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Time_Widget::updateTime);
    timer->start(1000);

    updateTime(); // Initial update
}