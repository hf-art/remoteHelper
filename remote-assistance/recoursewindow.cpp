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
    m_timer = new QTimer(this);
    // 设置2秒时间等待
    m_timer->setInterval(2 * 1000);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &recourseWindow::geneVerCodeUi);
    m_timer->start();
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
    m_centerWindow = new DWidget();
    setCentralWidget(m_centerWindow);

    DWidget *titleWidget = new DWidget();
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

    DSpinner *spinner = new DSpinner;
    spinner->start();
    spinner->setFixedSize(QSize(50, 50));

    m_tipLabel = new DLabel(this);
    m_tipLabel->setText("正在生成验证码，请稍后...");
    QFont font("Courier", 10);
    m_tipLabel->setFont(font);

    m_cancelButton = new DSuggestButton(this);
    m_cancelButton->setText("取 消");
    m_cancelButton->setFixedSize(QSize(190, 40));
    connect(m_cancelButton, &QPushButton::clicked, this, [ = ] {
        DWidget::close();
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
    vlayout->addWidget(m_tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(m_cancelButton, 0, Qt::AlignCenter);
    m_centerWindow->setLayout(vlayout);
}

void recourseWindow::geneVerCodeUi()
{
    //this->layout()->removeWidget(m_centerWindow);
    m_timer->stop();
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setWindowTitle(QString("我要求助"));
    m_centerWindow = new DWidget();
    setCentralWidget(m_centerWindow);
    m_verificationcode = new DLabel(this);
    m_verificationcode->setText(m_verCode);
    QFont vf_font("Courier", 30);
    vf_font.setBold(false);
    vf_font.setWeight(QFont::Light);
    vf_font.setLetterSpacing(QFont::PercentageSpacing, 180);
    vf_font.setBold(false);
    m_verificationcode->setFont(vf_font);

    QHBoxLayout *hVerCodeLayout = new QHBoxLayout();
    hVerCodeLayout->addStretch();
    hVerCodeLayout->addSpacing(30);
    hVerCodeLayout->addWidget(m_verificationcode);
    hVerCodeLayout->addStretch();

    m_tipLabel = new DLabel(this);
    m_tipLabel->setText("要开始共享您的桌面，请将上述验证码提供给协助您的成员，他们输""\n"
                        "           入验证码后，您的共享会话会立即开始");
    QFont tip_font("Courier", 8);
    m_tipLabel->setFixedWidth(320);
    m_tipLabel->setFont(tip_font);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::gray);
    m_tipLabel->setPalette(pe);

    m_copyButton = new DSuggestButton(this);
    m_copyButton->setBackgroundRole(QPalette::Button);
    m_copyButton->setText("复 制");
    m_copyButton->setFixedSize(QSize(190, 40));

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor("#F5F5F8"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 30, 0, 30);
    vlayout->setSpacing(15);
    vlayout->addLayout(hVerCodeLayout);
    vlayout->addWidget(m_tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(m_copyButton, 0, Qt::AlignCenter);
    m_centerWindow->setLayout(vlayout);
    connect(m_copyButton, &QPushButton::clicked, this, &recourseWindow::copyVerCodeUi);
    connect(m_copyButton, &QPushButton::clicked, this, &recourseWindow::copyVerCode);
}

void recourseWindow::copyVerCodeUi()
{
    //this->layout()->removeWidget(m_centerWindow);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setWindowTitle(QString("我要求助"));
    m_centerWindow = new DWidget();
    setCentralWidget(m_centerWindow);

    m_verificationcode = new DLabel(this);
    m_verificationcode->setText("  成功复制到粘贴板");
    QFont vf_font("Courier", 20);
    vf_font.setBold(false);
    vf_font.setWeight(QFont::Light);
    vf_font.setLetterSpacing(QFont::PercentageSpacing, 120);
    m_verificationcode->setFont(vf_font);

    QFont tip_font("Courier", 10);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::gray);

    QFont lableFont("Courier", 9);
    m_tipLabel = new DLabel(this);
    m_tipLabel->setText("           正在等待链接,请稍后...");
    m_tipLabel->setPalette(pe);
    m_tipLabel->setFont(lableFont);
    m_tipLabelSec = new DLabel(this);
    m_tipLabelSec->setText("     连接成功后，此界面会自动隐藏到任务栏");
    m_tipLabelSec->setPalette(pe);
    m_tipLabelSec->setFont(lableFont);

    QVBoxLayout *vlayoutInner = new QVBoxLayout();
    vlayoutInner->addStretch();
    vlayoutInner->setSpacing(5);
    vlayoutInner->addWidget(m_tipLabel);
    vlayoutInner->addWidget(m_tipLabelSec);
    vlayoutInner->addStretch();

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addStretch();
    hlayout->addLayout(vlayoutInner);
    hlayout->addStretch();

    m_copyButton = new DSuggestButton(this);
    m_copyButton->setBackgroundRole(QPalette::Button);
    m_copyButton->setText("返 回");
    m_copyButton->setFixedSize(QSize(190, 40));
    connect(m_copyButton, &QPushButton::clicked, this, [ = ] {
        DWidget::close();
        // 显示父窗体
        parentWidget()->show();
    });

    QHBoxLayout *hButtonLayout = new QHBoxLayout();
    hButtonLayout->addStretch();
    hButtonLayout->addSpacing(35);
    hButtonLayout->addWidget(m_copyButton);
    hButtonLayout->addStretch();

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 30, 0, 0);
    vlayout->setSpacing(30);
    vlayout->addWidget(m_verificationcode, 0, Qt::AlignCenter | Qt::AlignCenter);
    vlayout->addLayout(hlayout);
    vlayout->addLayout(hButtonLayout);
    vlayout->setContentsMargins(0, 0, 30, 0);
    vlayout->addStretch();
    m_centerWindow->setLayout(vlayout);
}

void recourseWindow::copyVerCode()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_verCode);
}







