#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QApplication>
#include <QWidget>
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include <QList>
#include <QString>

class gifPlayer : public QWidget {
    Q_OBJECT

public:
    gifPlayer(QWidget *parent = nullptr);

signals:
    void playFinished();

private slots:
    // 播放视频
    // void playVideo() {
    // }

private:
    QLabel *vediolabel;
    QMovie *movie;
    int currentFrame;

    QList<QString> framePaths;
    QTimer *timer;
};

#endif