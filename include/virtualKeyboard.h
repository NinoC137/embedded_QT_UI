#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QList>

class VirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualKeyboard(QWidget *parent = nullptr);

    // 设置目标 QLineEdit
    void setTargetLineEdit(QLineEdit *target);

signals:
    void closed();

private slots:
    void onKeyClicked();
    void onBackspaceClicked();
    void onSpaceClicked();
    void onEnterClicked();
    void onCloseClicked();

private:
    QLineEdit* m_targetLineEdit;
    QList<QPushButton*> m_keyButtons;
};

#endif // VIRTUALKEYBOARD_H
