#include "mainwindows.h"
#include "assistwindow.h"
#include <DMainWindow>
#include <QMessageBox>
#include <DWidgetUtil>
#include <QPainter>
#include <qpixmap.h>
#include <QPalette>
#include <DBlurEffectWidget>
#include <DFloatingButton>
#include <DHiDPIHelper>
#include <DImageButton>
#include <QBitmap>
#include "dtitlebar.h"
#include <DButtonBox>
#include <QTimer>
#include <QThread>
#include <DSpinner>

assistWindow::assistWindow(DWidget *parent)
    : DMainWindow(parent)
{
    initUi();
}

assistWindow::~assistWindow()
{

}

void assistWindow::setVerCode(QString verCode)
{
    m_verCode = verCode;
}

void assistWindow::initUi()
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    centerWindow = new QWidget();
    setCentralWidget(centerWindow);

    QWidget *titleWidget = new QWidget();
    DLabel *titleBarIcon = new DLabel(titleWidget);
    titleBarIcon->setFixedSize(25, 25);
    titleBarIcon->setPixmap(DHiDPIHelper::loadNxPixmap(":/images/heartSec.png").scaled(titleBarIcon->width(),
                                                                                       titleBarIcon->height(),
                                                                                       Qt::KeepAspectRatio,
                                                                                       Qt::SmoothTransformation));
    DLabel *titleLabel = new DLabel(titleWidget);
    titleLabel->setText("帮助别人");
    QFont titleFont("Courier", 11);
    titleFont.setBold(true);
    titleFont.setLetterSpacing(QFont::PercentageSpacing, 110);
    titleLabel->setFont(titleFont);
    QHBoxLayout *hTitleBarlayout = new QHBoxLayout(titleWidget);
    hTitleBarlayout->addStretch();
    hTitleBarlayout->setContentsMargins(100, 0, 0, 0);
    hTitleBarlayout->setSpacing(2);
    hTitleBarlayout->addWidget(titleBarIcon);
    hTitleBarlayout->addWidget(titleLabel);
    hTitleBarlayout->addStretch();
    titleWidget->setLayout(hTitleBarlayout);
    titlebar()->setCustomWidget(titleWidget);
    titlebar()->setStyleSheet("background-color: #F5F5F8;");

    lineEdit = new QLineEdit(this);
    lineEdit->setFixedSize(330, 80);
    QFont lineEditFont("Courier", 40);
    lineEdit->setFont(lineEditFont);
    lineEdit->setAlignment(Qt::AlignCenter);

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(setCompleter(const QString &)));

    tipLabel = new DLabel(this);
    tipLabel->setText("请在上方输入你想帮助的人提供给你的验证码，完成后点击连接开始""\n"
                      "                      ""协助对方");
    QFont tip_font("Courier", 8);
    tipLabel->setFixedWidth(330);
    tipLabel->setWordWrap(true);
    tipLabel->setAlignment(Qt::AlignTop);
    tipLabel->adjustSize();
    tipLabel->setFont(tip_font);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::gray);
    tipLabel->setPalette(pe);

    cancelButton = new DSuggestButton(this);
    cancelButton->setBackgroundRole(QPalette::Button);
    cancelButton->setText("取消");
    cancelButton->setFixedSize(QSize(190, 40));
    connect(cancelButton, &DPushButton::clicked, this, &assistWindow::changeWindow);
//    connect(cancelButton, &DPushButton::clicked, this, [ = ] {
//        this->close();
//   });

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor("#F5F5F8"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 30, 0, 30);
    vlayout->setSpacing(30);
    vlayout->addWidget(lineEdit, 0, Qt::AlignCenter);
    vlayout->addWidget(tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(cancelButton, 0, Qt::AlignCenter);
    vlayout->addStretch();
    centerWindow->setLayout(vlayout);
}

void assistWindow::connectSuc()
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setWindowTitle(QString("我要求助"));
    centerWindow = new QWidget();
    setCentralWidget(centerWindow);

    tipLabel = new DLabel(this);
    tipLabel->setText("您正在进行远程协助");
    QFont tip_font1("Courier", 16);
    //tipLabel->setFixedWidth(300);
    tip_font1.setLetterSpacing(QFont::PercentageSpacing, 120);
    tipLabel->setFont(tip_font1);

    DLabel *tipLabel1 = new DLabel(this);
    tipLabel1->setText("  当前已有一个连接会话，如果您想建立新的连接，请先断开");
    QFont tip_font("Courier", 8);
    tipLabel1->setFixedWidth(300);
    tipLabel1->setFont(tip_font);
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::gray);
    tipLabel1->setPalette(pal);

    cancelButton = new DSuggestButton(this);
    cancelButton->setBackgroundRole(QPalette::Button);
    cancelButton->setText("断开");
    cancelButton->setFixedSize(QSize(190, 40));
    //12-25
//    connect(cancelButton, &DPushButton::clicked, this, [ = ] {
//        this->close();
//    });
    connect(cancelButton, &DPushButton::clicked, this, &assistWindow::changeWindow);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 30, 0, 30);
    vlayout->setSpacing(15);
    vlayout->addWidget(tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(tipLabel1, 0, Qt::AlignCenter);
    vlayout->addWidget(cancelButton, 0, Qt::AlignCenter);
    centerWindow->setLayout(vlayout);
}

void assistWindow::connectFaild()
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setWindowTitle(QString("我要求助"));
    centerWindow = new QWidget();
    setCentralWidget(centerWindow);

    tipLabel = new DLabel(this);
    tipLabel->setText("连接失败");
    QFont tip_font1("Courier", 20);
    //tipLabel->setFixedSize(300,60);
    tip_font1.setLetterSpacing(QFont::PercentageSpacing, 120);
    tipLabel->setWordWrap(true);
    tipLabel->setAlignment(Qt::AlignTop);
    tipLabel->adjustSize();
    tipLabel->setFont(tip_font1);
    QPalette pe;
    pe.setColor(QPalette::WindowText, QColor("#FC952E"));
    tipLabel->setPalette(pe);

    DLabel *tipLabel1 = new DLabel(this);
    tipLabel1->setText("网络连接状态不是很稳定，点击重试按钮再次尝试连接");
    QFont tip_font("Courier", 8);

    tipLabel1->setFont(tip_font);
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::gray);
    tipLabel1->setPalette(pal);

    cancelButton = new DSuggestButton(this);
    cancelButton->setBackgroundRole(QPalette::Button);
    cancelButton->setText("重试");
    cancelButton->setFixedSize(QSize(190, 40));
    connect(cancelButton, &DPushButton::clicked, this, &assistWindow::initUi);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 30, 0, 30);
    vlayout->setSpacing(15);
    vlayout->addWidget(tipLabel, 0, Qt::AlignCenter | Qt::AlignCenter);
    vlayout->addWidget(tipLabel1, 0, Qt::AlignCenter | Qt::AlignCenter);
    vlayout->addWidget(cancelButton, 0, Qt::AlignCenter | Qt::AlignCenter);
    centerWindow->setLayout(vlayout);
}

void assistWindow::changeButtonState()
{
    disconnect(cancelButton, &DPushButton::clicked, this, &assistWindow::changeWindow);
    cancelButton->setText("连接");
    connect(cancelButton, SIGNAL(clicked(bool)), this,  SLOT(connection()));
}

void assistWindow::changeButtonStateToCancle()
{
    if (cancelButton->text() == "连接") {
        disconnect(cancelButton, SIGNAL(clicked(bool)), this,  SLOT(connection()));
        cancelButton->setText("取消");
        connect(cancelButton, &DPushButton::clicked, this, &assistWindow::changeWindow);
    }

}

void assistWindow::connection()
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setWindowTitle(QString("我要求助"));
    centerWindow = new QWidget();
    setCentralWidget(centerWindow);

    DSpinner *spinner = new DSpinner();
    spinner->start();
    spinner->setFixedSize(QSize(50, 50));

    tipLabel = new DLabel(this);
    tipLabel->setText("正在建立连接，请稍后...");
    QFont font("Courier", 10);
    tipLabel->setFont(font);

    cancelButton = new DSuggestButton(this);
    cancelButton->setBackgroundRole(QPalette::Button);
    cancelButton->setText("取消");
    cancelButton->setFixedSize(QSize(190, 40));
//    connect(cancelButton, &DPushButton::clicked, this, [ = ] {
//        this->close();
//    });

    connect(cancelButton, &QPushButton::clicked, this, [ = ] {
        emit isCancle();
        // 显示父窗体
        assistWindow::initUi();
    });

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 30, 0, 30);
    vlayout->setSpacing(15);
    vlayout->addWidget(spinner, 0, Qt::AlignCenter);
    vlayout->addWidget(tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(cancelButton, 0, Qt::AlignCenter);
    centerWindow->setLayout(vlayout);


    timer = new QTimer(this);
    // 设置5秒时间等待
    timer->setInterval(5 * 1000);
    //timer->setSingleShot(false);
    timer->start();
    connect(this, &assistWindow::isCancle, this, [ = ] {
        timer->stop();
    });
    const QString editText = lineEdit->text();
    connect(timer, &QTimer::timeout, this, [ = ] {
        changeWindowState(editText);
    });
    // 设置5秒的等待
//    QEventLoop eventloop;
//    QTimer::singleShot(5 * 1000, &eventloop, SLOT(quit()));
//    eventloop.exec();
}

bool assistWindow::isVerCodeValid(const QString &str)
{
    int length = str.length();
    for (int i = 0; i < length; i++) {
        if (str[i] < '0' || str[i] > '9') {
            QMessageBox::warning(nullptr, "warning", "校验码输入非法，请输入6位0~9之间的数字！");
            return false;
        }
    }

    if (length > 6) {
        QMessageBox::warning(nullptr, "warning", "校验码不能大于6位");
        return false;
    } else if (length == 6) {
        return true;
    }
    return false;
}

void assistWindow::setCompleter(const QString &str)
{
    int length = str.length();
    if (length < 6) {
        changeButtonStateToCancle();
    }

    if (length > 6) {
        QMessageBox::warning(nullptr, "warning", "校验码不能大于6位");
        return;
    } else {
        for (int i = 0; i < length; i++) {
            if (str[i] < '0' || str[i] > '9') {
                QMessageBox::warning(nullptr, "warning", "校验码输入非法，请输入6位0~9之间的数字！");
                return;
            }
        }
    }

    if (length == 6) {
        changeButtonState();
    }
}

void assistWindow::changeWindow()
{
    QWidget::close();
    // 显示父窗体
    parentWidget()->move(x(), y());
    parentWidget()->show();
}

void assistWindow::changeWindowState(const QString &str)
{
    timer->stop();
    if (m_verCode == str) {
        connectSuc();
    } else {
        connectFaild();
    }
}
