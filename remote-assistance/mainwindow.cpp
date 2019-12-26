#include "mainwindows.h"
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
#include <DThemeManager>

MainWindow::MainWindow(DWidget *parent)
    : DMainWindow(parent)
{
    initUi();
}

void MainWindow::showRecourseWindow()
{
    m_rw = new recourseWindow(this);
    m_rw->setMinimumSize(360, 320);
    //Dtk::Widget::moveToCenter(rw);
    m_rw->move(x(), y());
    m_verCode = m_rw->getVerCode();
    //12-25
    this->hide();
    m_rw->setFixedSize(360, 320);
    m_rw->show();
}

void MainWindow::showAssistWindow()
{
    m_aw = new assistWindow(this);
    m_aw->setMinimumSize(360, 320);
    //Dtk::Widget::moveToCenter(aw);
    m_aw->move(x(), y());
    m_aw->setVerCode(m_verCode);
    //12-25
    this->hide();
    m_aw->setFixedSize(360, 320);
    m_aw->show();
}


MainWindow::~MainWindow()
{

}

void MainWindow::initUi()
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor("#F5F5F8"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    DWidget *titleWidget = new DWidget();
    DLabel *titleLabel = new DLabel(titleWidget);
    titleLabel->setText("远程协助");
    QFont titleFont("Courier", 11);
    titleFont.setBold(true);
    titleFont.setLetterSpacing(QFont::PercentageSpacing, 110);
    titleLabel->setFont(titleFont);
    QHBoxLayout *hTitleBarlayout = new QHBoxLayout(titleWidget);
    hTitleBarlayout->addStretch();
    hTitleBarlayout->setContentsMargins(130, 0, 0, 0);
    hTitleBarlayout->setSpacing(2);
    hTitleBarlayout->addWidget(titleLabel);
    hTitleBarlayout->addStretch();
    titleWidget->setLayout(hTitleBarlayout);
    titlebar()->setCustomWidget(titleWidget);
    titlebar()->setStyleSheet("background-color: #F5F5F8;");

    m_centerWindow = new DWidget();
    setCentralWidget(m_centerWindow);

    DLabel *titleIcon = new DLabel(this);
    titleIcon->setPixmap(DHiDPIHelper::loadNxPixmap(":/images/tower.png"));

    m_tipLabel = new DLabel(this);
    m_tipLabel->setText("这是由deppin团队开发的远程协助应用，如果您在使""\n"
                        "用过程中遇到了困难，或者有人求助于你，请点击下""\n"
                        "      　　　方的我要求助和帮助别人。");
    QFont font("Courier", 8);
    m_tipLabel->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::gray);
    m_tipLabel->setPalette(pe);

    QPixmap pixHand(":/images/hand.png");
    QImage imagHand = ConvertImageToTransparent(pixHand);

    QHBoxLayout *hlayoutRb = new QHBoxLayout(this);
    DLabel *recButtonRbText = new DLabel("我要求助");
    DLabel *recButtonRbIcon = new DLabel(this);
    recButtonRbIcon->setPixmap(QPixmap::fromImage(imagHand));
    hlayoutRb->addStretch();
    hlayoutRb->addWidget(recButtonRbIcon);
    hlayoutRb->addWidget(recButtonRbText);
    hlayoutRb->addStretch();

    m_recourseButton = new DPushButton(this);
    m_recourseButton->setBackgroundRole(QPalette::Button);
    m_recourseButton->setFixedSize(QSize(165, 35));
    m_recourseButton->setIconSize(QSize(50, 50));
    m_recourseButton->setLayout(hlayoutRb);
    connect(m_recourseButton, SIGNAL(clicked(bool)), this,  SLOT(showRecourseWindow()));


    QPixmap pixHeart(":/images/heart.png");
    QImage imagHeart = ConvertImageToTransparent(pixHeart);

    QHBoxLayout *hlayoutAb = new QHBoxLayout(this);
    DLabel *hlayoutAbTest = new DLabel("帮助别人");
    DLabel *hlayoutAbIcon = new DLabel(this);
    hlayoutAbIcon->setPixmap(QPixmap::fromImage(imagHeart));
    hlayoutAb->addStretch();
    hlayoutAb->addWidget(hlayoutAbIcon);
    hlayoutAb->addWidget(hlayoutAbTest);
    hlayoutAb->addStretch();

    m_assistButton = new DPushButton(this);
    m_assistButton->setIconSize(QSize(50, 40));
    m_assistButton->setFixedSize(QSize(165, 35));
    m_assistButton->setBackgroundRole(QPalette::Button);
    m_assistButton->setLayout(hlayoutAb);
    connect(m_assistButton, SIGNAL(clicked(bool)), this,  SLOT(showAssistWindow()));

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(10);
    vlayout->addStretch();
    vlayout->addWidget(titleIcon, 0, Qt::AlignCenter);
    vlayout->addWidget(m_tipLabel, 0, Qt::AlignCenter);
    vlayout->addWidget(m_recourseButton, 0, Qt::AlignCenter);
    vlayout->addWidget(m_assistButton, 0,  Qt::AlignCenter);
    vlayout->addStretch();
    m_centerWindow->setLayout(vlayout);
}

QImage MainWindow::ConvertImageToTransparent(QPixmap qPixmap)
{
    QImage image = qPixmap.toImage();
    image = image.convertToFormat(QImage::Format_ARGB32);
    union myrgb {
        uint rgba;
        uchar rgba_bits[4];
    };
    myrgb *mybits = (myrgb *)image.bits();
    int len = image.width() * image.height();
    while (len -- > 0) {
        mybits->rgba_bits[3] = (mybits->rgba == 0xFFFFFFFF) ? 0 : 255;
        mybits++;
    }
    return image;
}





