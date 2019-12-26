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
    MainWindow(DWidget *parent = nullptr);
    ~MainWindow();

    QImage ConvertImageToTransparent(QPixmap qPixmap);
private:
    void initUi();
    DLabel *m_tipLabel;
    DWidget *m_centerWindow;
    DPushButton *m_recourseButton;
    DPushButton *m_assistButton;
    recourseWindow *m_rw;
    assistWindow *m_aw;
    QString m_verCode;

public slots:
    void showRecourseWindow();
    void showAssistWindow();
};

#endif // MAINWINDOWS_H
