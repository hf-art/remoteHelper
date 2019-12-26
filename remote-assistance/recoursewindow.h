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
    recourseWindow(DWidget *parent = nullptr);
    ~recourseWindow();
    QString genVerCode();
    QString getVerCode() { return m_verCode; }

private:
    void initUi();
    DLabel *m_verificationcode;
    DLabel *m_tipLabel;
    DLabel *m_tipLabelSec;
    DWidget *m_centerWindow;
    DSuggestButton *m_cancelButton;
    DPushButton *m_copyButton;
    QTimer *m_timer;
    QString m_verCode;

public slots:
    void geneVerCodeUi();
    void copyVerCodeUi();
    void copyVerCode();

};

#endif // RECOURSEWINDOW_H
