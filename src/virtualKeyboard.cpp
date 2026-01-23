#include "virtualKeyboard.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QGuiApplication>

VirtualKeyboard::VirtualKeyboard(QWidget *parent)
    : QWidget(parent),
      m_targetLineEdit(nullptr)
{
    // 键盘样式
    setStyleSheet("background-color: #1F1F1F; border-radius: 10px;");

    // setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    // setAttribute(Qt::WA_TranslucentBackground);

    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 键位定义
    QList<QString> keyRows;
    keyRows << "1234567890"
            << "qwertyuiop"
            << "asdfghjkl"
            << "_-zxcvbnm,.";

    for (const QString &row : keyRows) {
        QHBoxLayout *rowLayout = new QHBoxLayout();
        for (QChar c : row) {
            QPushButton *button = new QPushButton(QString(c), this);
            button->setStyleSheet(
                        "background-color: #404040;"
                        "border-radius: 10px;"
                        "color: rgb(227, 181, 5);"
                        "font-size: 16px;"
                        "padding: 10px;"
                        );
            button->setFocusPolicy(Qt::NoFocus);
            connect(button, &QPushButton::clicked, this, &VirtualKeyboard::onKeyClicked);
            rowLayout->addWidget(button);
            m_keyButtons.append(button);
        }
        mainLayout->addLayout(rowLayout);
    }

    // 功能键
    QHBoxLayout *funcLayout = new QHBoxLayout();

    QPushButton *backspaceBtn = new QPushButton("Del", this);
    backspaceBtn->setStyleSheet(
                "background-color: #f0f0f0;"
                "border-radius: 10px;"
                "color: rgb(0, 0, 0);"
                "font-size: 16px;"
                "padding: 10px;"
                );
    backspaceBtn->setFocusPolicy(Qt::NoFocus);
    connect(backspaceBtn, &QPushButton::clicked, this, &VirtualKeyboard::onBackspaceClicked);
    funcLayout->addWidget(backspaceBtn);

    QPushButton *spaceBtn = new QPushButton("Space", this);
    spaceBtn->setStyleSheet(
                "background-color: #f0f0f0;"
                "border-radius: 10px;"
                "color: rgb(0, 0, 0);"
                "font-size: 16px;"
                "padding: 10px;"
                );
    spaceBtn->setFocusPolicy(Qt::NoFocus);
    connect(spaceBtn, &QPushButton::clicked, this, &VirtualKeyboard::onSpaceClicked);
    funcLayout->addWidget(spaceBtn);

    QPushButton *enterBtn = new QPushButton("Enter", this);
    enterBtn->setStyleSheet(
                "background-color: #f0f0f0;"
                "border-radius: 10px;"
                "color: rgb(0, 0, 0);"
                "font-size: 16px;"
                "padding: 10px;"
                );
    enterBtn->setFocusPolicy(Qt::NoFocus);
    connect(enterBtn, &QPushButton::clicked, this, &VirtualKeyboard::onEnterClicked);
    funcLayout->addWidget(enterBtn);

    QPushButton *closeBtn = new QPushButton("Close", this);
    closeBtn->setStyleSheet(
                "background-color: #f0f0f0;"
                "border-radius: 10px;"
                "color: rgb(0, 0, 0);"
                "font-size: 16px;"
                "padding: 10px;"
                );
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, &QPushButton::clicked, this, &VirtualKeyboard::onCloseClicked);
    funcLayout->addWidget(closeBtn);

    mainLayout->addLayout(funcLayout);

    // 设置固定大小
    // QScreen *screen = window()->screen();
    // QRect screenGeometry = screen->availableGeometry();
    // int keyboardWidth = screenGeometry.width();
    // int keyboardHeight = 350;
    // setFixedSize(keyboardWidth, keyboardHeight);

    if(parent){
        int keyboardWidth = KEYBOARDWIDTH;
        int keyboardHeight = KEYBOARDHEIGHT;
        setFixedSize(keyboardWidth, keyboardHeight);
    }else{
        qDebug("VirtualKeyboard: parent is null, cannot set size based on parent.");
        return;
        // QScreen *screen = QGuiApplication::primaryScreen();
        // QRect screenGeometry = screen->availableGeometry();
        // int keyboardWidth = screenGeometry.width();
        // int keyboardHeight = 350;
        // setFixedSize(keyboardWidth, keyboardHeight);
    }

}

void VirtualKeyboard::setTargetLineEdit(QLineEdit *target)
{
    m_targetLineEdit = target;
}

void VirtualKeyboard::onKeyClicked()
{
    if (!m_targetLineEdit) return;
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button) m_targetLineEdit->insert(button->text());
}

void VirtualKeyboard::onBackspaceClicked()
{
    if(m_targetLineEdit) m_targetLineEdit->backspace();
}

void VirtualKeyboard::onSpaceClicked()
{
    if(m_targetLineEdit) m_targetLineEdit->insert(" ");
}

void VirtualKeyboard::onEnterClicked()
{
    if(m_targetLineEdit) {
        emit closed();
        m_targetLineEdit->returnPressed();
    }
}

void VirtualKeyboard::onCloseClicked()
{
    emit closed();
}
