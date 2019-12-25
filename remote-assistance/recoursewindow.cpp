#include "mainwindows.h"
#include "recoursewindow.h"
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
#include <QFile>
#include <QClipboard>
#include <DSpinner>
#include <DThemeManager>

recourseWindow::recourseWindow(DWidget *parent)
    : DMainWindow(parent)
{
    m_verCode = genVerCode();
    initUi();
    timer = new QTimer(this);
    // 设置2秒时间等待
    timer->setInterval(2 * 1000);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, this, &recourseWindow::geneVerCodeUi);
    timer->start();
}

recourseWindow::~recourseWindow()
{

}

//生成一个6位的随机数
QString recourseWindow::genVerCode()
{
    QString qstr = "";
    for (int i = 0; i < 6; i++) {
        qint64 randomNumber = rand() % 10;
        qstr += QString("%1").arg(randomNumber);
    }
    return qstr;
}

void recourseWindow::initUi()
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    centerWindow = new QWidget();
    setCentralWidget(centerWindow);

    QWidget *titleWidget = new QWidget();
    DLabel *titleBarIcon = new DLabel(titleWidget);
    titleBarIcon->setFixedSize(28, 28);
    titleBarIcon->setPixmap(DHiDPIHelper::loadNxPixmap(":/images/handSec.png").scaled(titleBarIcon->width(),
                                                                                      titleBarIcon->height(),
                                                                                      Qt::KeepAspectRatio,
                                                                                      Qt::SmoothTransformation));
    DLabel *titleLabel = new DLabel(titleWidget);
    titleLabel->setText("我要求助");
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

    auto spinner = new DSpinner;
    spinner->start();
    spinner->setFixedSize(QSize(50, 50));

    tipLabel = new DLabel(this);
    tipLabel->setText("正在生成验证码，请稍后...");
    QFont font("Courier", 10);
    tipLabel->setFont(font);

    cancelButton = new DSuggestButton(this);
    cancelButton->setText("取消");
    cancelButton->setFixedSize(QSize(190, 40));
    connect(cancelButton, &QPushButton::clicked, this, [ = ] {
        QWidget::close();
        // 显示父窗体
        parentWidget()->move(x(), y());
        parentWidget()->show();
    });

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor("#F5F5F8"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 30, 0, 30);
    vlayout->setSpacing(15);
    vlayout->addWidget(spinner, 0, Qt::AlignCenter);
    vlayout->addWidget(tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(cancelButton, 0, Qt::AlignCenter);
    centerWindow->setLayout(vlayout);
}

void recourseWindow::geneVerCodeUi()
{
    timer->stop();
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setWindowTitle(QString("我要求助"));
    centerWindow = new QWidget();
    setCentralWidget(centerWindow);
    verificationcode = new DLabel(this);
    verificationcode->setText(m_verCode);
    QFont vf_font("Courier", 30);
    vf_font.setBold(false);
    vf_font.setWeight(QFont::Light);
    vf_font.setLetterSpacing(QFont::PercentageSpacing, 180);
    vf_font.setBold(false);
    verificationcode->setFont(vf_font);

    QHBoxLayout *hVerCodeLayout = new QHBoxLayout();
    hVerCodeLayout->addStretch();
    hVerCodeLayout->addSpacing(30);
    hVerCodeLayout->addWidget(verificationcode);
    hVerCodeLayout->addStretch();

    tipLabel = new DLabel(this);
    tipLabel->setText("要开始共享您的桌面，请将上述验证码提供给协助您的成员，他们输""\n"
                      "           入验证码后，您的共享会话会立即开始");
    QFont tip_font("Courier", 8);
    tipLabel->setFixedWidth(320);
    tipLabel->setFont(tip_font);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::gray);
    tipLabel->setPalette(pe);

    copyButton = new DSuggestButton(this);
    copyButton->setBackgroundRole(QPalette::Button);
    copyButton->setText("复制");
    copyButton->setFixedSize(QSize(190, 40));

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor("#F5F5F8"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 30, 0, 30);
    vlayout->setSpacing(15);
    vlayout->addLayout(hVerCodeLayout);
    vlayout->addWidget(tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(copyButton, 0, Qt::AlignCenter);
    centerWindow->setLayout(vlayout);
    connect(copyButton, &QPushButton::clicked, this, &recourseWindow::copyVerCodeUi);
    connect(copyButton, &QPushButton::clicked, this, &recourseWindow::copyVerCode);
}

void recourseWindow::copyVerCodeUi()
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setWindowTitle(QString("我要求助"));
    centerWindow = new QWidget();
    setCentralWidget(centerWindow);

    verificationcode = new DLabel(this);
    verificationcode->setText("  成功复制到粘贴板");
    QFont vf_font("Courier", 20);
    vf_font.setBold(false);
    vf_font.setWeight(QFont::Light);
    vf_font.setLetterSpacing(QFont::PercentageSpacing, 120);
    verificationcode->setFont(vf_font);

    QFont tip_font("Courier", 10);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::gray);

    QFont lableFont("Courier", 9);
    tipLabel = new DLabel(this);
    tipLabel->setText("           正在等待链接,请稍后...");
    tipLabel->setPalette(pe);
    tipLabel->setFont(lableFont);
    tipLabelSec = new DLabel(this);
    tipLabelSec->setText("     连接成功后，此界面会自动隐藏到任务栏");
    tipLabelSec->setPalette(pe);
    tipLabelSec->setFont(lableFont);

    QVBoxLayout *vlayoutInner = new QVBoxLayout();
    vlayoutInner->addStretch();
    vlayoutInner->setSpacing(5);
    vlayoutInner->addWidget(tipLabel);
    vlayoutInner->addWidget(tipLabelSec);
    vlayoutInner->addStretch();

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addStretch();
    hlayout->addLayout(vlayoutInner);
    hlayout->addStretch();

    copyButton = new DSuggestButton(this);
    copyButton->setBackgroundRole(QPalette::Button);
    copyButton->setText("返回");
    copyButton->setFixedSize(QSize(190, 40));
    connect(copyButton, &QPushButton::clicked, this, [ = ] {
        QWidget::close();
        // 显示父窗体
        parentWidget()->show();
    });

    QHBoxLayout *hButtonLayout = new QHBoxLayout();
    hButtonLayout->addStretch();
    hButtonLayout->addSpacing(35);
    hButtonLayout->addWidget(copyButton);
    hButtonLayout->addStretch();

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 30, 0, 0);
    vlayout->setSpacing(30);
    vlayout->addWidget(verificationcode, 0, Qt::AlignCenter | Qt::AlignCenter);
    vlayout->addLayout(hlayout);
    vlayout->addLayout(hButtonLayout);
    vlayout->setContentsMargins(0, 0, 30, 0);
    vlayout->addStretch();
    centerWindow->setLayout(vlayout);
}

void recourseWindow::copyVerCode()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_verCode);
}







