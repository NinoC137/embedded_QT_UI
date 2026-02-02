#include "media_player.h"
#include <QDebug>
#include <QFile>
#include <QPixmap>
#include <QDir>

gifPlayer::gifPlayer(QWidget *parent) : QWidget(parent) {
    vediolabel = new QLabel(this);
    vediolabel->setMinimumSize(480, 480);
    vediolabel->setScaledContents(true);

    QString exeDir = QCoreApplication::applicationDirPath();
    QString frameDir = exeDir + "/gif/"; // 存放 PNG 的目录

    QDir dir(frameDir);
    QStringList nameFilter;
    nameFilter << "gif_frame_*.png";
    QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);

    if (files.isEmpty()) {
        qDebug() << "No frames found in" << frameDir;
        return;
    }

    std::sort(files.begin(), files.end(), [](const QString &a, const QString &b){
        // 提取文件名中的数字   
        QRegExp re("\\d+");
        int numA = -1, numB = -1;
        if(re.indexIn(a) != -1) numA = re.cap(0).toInt();
        if(re.indexIn(b) != -1) numB = re.cap(0).toInt();
        return numA < numB;
    });

    // 生成完整路径列表
    for (const QString &file : files) {
        framePaths.append(frameDir + file);
    }

    currentFrame = 0;

    // 设置定时器播放
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]() {
        if(currentFrame >= framePaths.size()) {
            timer->stop();
            emit playFinished();
            return;
        }

        QPixmap pix(framePaths[currentFrame]);
        vediolabel->setPixmap(pix);

        currentFrame++;
    });

    double totalDurationMs = 1300.0;        // 1.3 秒
    int frameCount = framePaths.size();      // 124
    int interval = static_cast<int>(totalDurationMs / frameCount);

    timer->start(interval);
}
