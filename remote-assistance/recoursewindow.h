#ifndef RECOURSEWINDOW_H
#define RECOURSEWINDOW_H

#include <DMainWindow>
#include <DApplication>
#include <DWidget>
#include <DPushButton>
#include <DLabel>
#include <DLineEdit>
#include <DSuggestButton>

DWIDGET_USE_NAMESPACE

class recourseWindow : public DMainWindow
{
    Q_OBJECT
public:
    recourseWindow(QWidget *parent = nullptr);
    ~recourseWindow();
    QString genVerCode();
    QString getVerCode() { return m_verCode; }

private:
    void initUi();
    DLabel *verificationcode;
    DLabel *tipLabel;
    DLabel *tipLabelSec;
    QWidget *centerWindow;
    DSuggestButton *cancelButton;
    DPushButton *copyButton;
    QTimer *timer;
    QString m_verCode;

public slots:
    void geneVerCodeUi();
    void copyVerCodeUi();
    void copyVerCode();

};

#endif // RECOURSEWINDOW_H
