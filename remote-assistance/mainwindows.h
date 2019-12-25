#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H

#include <DMainWindow>
#include <DApplication>
#include <DWidget>
#include <DPushButton>
#include <DLabel>
#include <DLineEdit>
#include <DFloatingButton>
#include "recoursewindow.h"
#include "assistwindow.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage ConvertImageToTransparent(QPixmap qPixmap);
private:
    void initUi();
    DLabel *tipLabel;
    QWidget *centerWindow;
    DPushButton *recourseButton;
    DPushButton *assistButton;
    recourseWindow *rw;
    assistWindow *aw;
    QString m_verCode;

public slots:
    void showRecourseWindow();
    void showAssistWindow();
};

#endif // MAINWINDOWS_H
