#ifndef ASSISTWINDOW_H
#define ASSISTWINDOW_H

#include <DMainWindow>
#include <DApplication>
#include <DWidget>
#include <DPushButton>
#include <DLabel>
#include <DLineEdit>
#include <DTextEdit>
#include <DSuggestButton>

DWIDGET_USE_NAMESPACE

class assistWindow : public DMainWindow
{
    Q_OBJECT
public:
    assistWindow(DWidget *parent = nullptr);
    ~assistWindow();
    void setVerCode(QString verCode);

    bool isVerCodeValid(const QString &str);
    void changeButtonStateToCancle();
private:
    void initUi();
    DLabel *m_tipLabel;
    DWidget *m_centerWindow;
    DSuggestButton *m_cancelButton;
    QLineEdit *m_lineEdit;
    QString m_verCode;
    QTimer *m_timer;
    void connectSuc();
    void connectFaild();

signals:
    void isCancle();

public slots:
    void changeButtonState();
    void connection();
    void setCompleter(const QString &str);
    void changeWindow();
    void changeWindowState(const QString &str);
};


#endif // ASSISTWINDOW_H
