/*===================================================
 *������ģ��
 *����˵�������������ڣ���ʾ�����и����ܰ�ť�Լ���������л�Ч��
 *ģ����Ҫ�ԣ���Ҫ
 *���ߣ����
 *
 *---------------------------------------------------
 *���������
 *2013.3.19 �����϶������洰�����ָ����շ�����״
 *==================================================*/
#include "widget.h"
#include "ui_widget.h"
#include "dlgabout.h"
#include "dlgsetup.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    dingWei = true;
    chaZhao = true;
    m_isSilenced = false;
    isshowShortestRoute = true;
    ui->xunluWidget->setVisible(false);
    ui->locatingLabel->setVisible(false);
}

Widget::~Widget()
{
    delete ui;
}

MapTilesFrame *Widget::getMapFrame()
{
    return ui->mapFrame;
}

RouteWidget *Widget::getRouteWidget()
{
    return ui->routeWidget;
}

//ʹ�����ڿ�����ק
void Widget::mousePressEvent(QMouseEvent *event)
{
    ui->windowWidget->setCursor(Qt::ClosedHandCursor);
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
void Widget::mouseReleaseEvent(QMouseEvent *)
{
    qDebug() << "mouseRelease called";
    ui->windowWidget->setCursor(Qt::OpenHandCursor);
}

//�ر�ϵͳ
void Widget::on_exitPushButton_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry"); //�ر�����ľ���պ�Ч��
    animation->setDuration(1000);
    animation->setStartValue(QRect(0, 0, 1210, 770));
    animation->setEndValue(QRect(605, 0, 0, 770));
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start();
    //�ó���ȴ�1��رգ�ʵ�ֹرվ���Ч��
    QTime t;
    t.start();
    while(t.elapsed()<1000)
        QCoreApplication::processEvents();
    QCoreApplication::quit();
}

//�Ŵ��ͼ��ť
void Widget::on_enlargeToolButton_clicked()
{
    ui->mapFrame->zoomIn();
}

//��С��ͼ��ť
void Widget::on_shrinkToolButton_clicked()
{
    ui->mapFrame->zoomOut();
}

//���ڡ�����;˵����ť
void Widget::on_aboutDlgToolButton_clicked()
{
    DlgAbout *dlg = new DlgAbout(this);
    dlg->setWindowFlags(Qt::FramelessWindowHint);
    dlg->setAttribute(Qt::WA_TranslucentBackground);
    dlg->exec();
}

/*=======================================================================================
 *����������ҡ���ťʱ�������Ѱ·������壬���ҹ��ܵ�sousuoLineEdiit��sousuoToolButton�ͻ��������ҳ��֣�
 *�γ�һ�ֽ�Ѱ·��������Ƴ�ȥ��Ч��
 *
 =======================================================================================*/
void Widget::on_searchToolButton_clicked()
{
    chaZhao = true;
    if(xunLu == true)
    {
        //���������������
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 0, 40));
        animation->setEndValue(QRect(875, 300, 170, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 0, 50));
        animation1->setEndValue(QRect(1045, 295, 70, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();

        //Ѱ·��������Ƴ�
        ui->xunluWidget->setVisible(true);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 220, 480));
        animation4->setEndValue(QRect(1100, 210, 0, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else
    {
        //���������������
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 0, 40));
        animation->setEndValue(QRect(875, 300, 170, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 0, 50));
        animation1->setEndValue(QRect(1045, 295, 70, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();
    }
    xunLu = false;
}


void Widget::on_locateToolButton_clicked()
{      
    if(xunLu == true)
    {
        //��λ�����������
        QPropertyAnimation *animation = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(860, 215, 0, 70));
        animation->setEndValue(QRect(860, 215, 70, 70));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(930, 230, 0, 40));
        animation1->setEndValue(QRect(930, 230, 170, 40));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();


        //Ѱ·��������Ƴ�
        ui->xunluWidget->setVisible(true);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 220, 480));
        animation4->setEndValue(QRect(1100, 210, 0, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else
    {//��λ�����������
        QPropertyAnimation *animation = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(860, 215, 0, 70));
        animation->setEndValue(QRect(860, 215, 60, 70));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(930, 230, 0, 40));
        animation1->setEndValue(QRect(930, 230, 170, 40));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();
    }
    dingWei = true;
    xunLu = false;

    ui->locatingLabel->setVisible(true);
    QTime t;
    t.start();
    while(t.elapsed()<2000)
        QCoreApplication::processEvents();
    ui->locatingLabel->setVisible(false);

    if(ui->routeWidget->userLocationWidget)
    {
        delete ui->routeWidget->userLocationWidget;
        ui->routeWidget->userLocationWidget = NULL;
    }
    ui->routeWidget->userLocationWidget = new LocationWidget(ui->mapFrame);
    connect(ui->routeWidget->userLocationWidget,
            SIGNAL(serverCoordinateChanged(QPoint,QPoint)),
            ui->routeWidget->sm,
            SLOT(userLocationChangedSlot(QPoint)));
    ui->routeWidget->userLocationWidget->setRandomPos();


    ui->locationLabel->setText(ui->routeWidget->findNearestSpotName(ui->routeWidget->userLocationWidget->getServerCoordinate()) + " ����");
}

void Widget::on_xunluToolButton_clicked()
{
    if (dingWei==true&&chaZhao==true)
    {
        //������������Ƴ�
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 170, 40));
        animation->setEndValue(QRect(1100, 300, 0, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 70, 50));
        animation1->setEndValue(QRect(1100, 295, 0, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();

        //��λ��������Ƴ�
        QPropertyAnimation *animation2 = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation2->setDuration(1000);
        animation2->setStartValue(QRect(860, 215, 70, 70));
        animation2->setEndValue(QRect(1100, 215, 0, 70));
        animation2->setEasingCurve(QEasingCurve::InOutQuad);
        animation2->start();

        QPropertyAnimation *animation3 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation3->setDuration(1000);
        animation3->setStartValue(QRect(930, 230, 170, 40));
        animation3->setEndValue(QRect(1100, 230, 0, 40));
        animation3->setEasingCurve(QEasingCurve::InOutQuad);
        animation3->start();

        //Ѱ·�����������
        ui->xunluWidget->setVisible(true);
        ui->zizhuxunluWidget->setVisible(false);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 0, 480));
        animation4->setEndValue(QRect(880, 210, 220, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else if(dingWei == false && chaZhao == true)
    {
        //������������Ƴ�
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 170, 40));
        animation->setEndValue(QRect(1100, 300, 0, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 70, 50));
        animation1->setEndValue(QRect(1100, 295, 0, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();
        //Ѱ·�����������
        ui->xunluWidget->setVisible(true);
        ui->zizhuxunluWidget->setVisible(false);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 0, 480));
        animation4->setEndValue(QRect(880, 210, 220, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else if(dingWei == true && chaZhao == false)
    {
        //��λ��������Ƴ�
        QPropertyAnimation *animation2 = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation2->setDuration(1000);
        animation2->setStartValue(QRect(860, 215, 70, 70));
        animation2->setEndValue(QRect(1100, 215, 0, 70));
        animation2->setEasingCurve(QEasingCurve::InOutQuad);
        animation2->start();

        QPropertyAnimation *animation3 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation3->setDuration(1000);
        animation3->setStartValue(QRect(930, 230, 170, 40));
        animation3->setEndValue(QRect(1100, 230, 0, 40));
        animation3->setEasingCurve(QEasingCurve::InOutQuad);
        animation3->start();

        //Ѱ·�����������
        ui->xunluWidget->setVisible(true);
        ui->zizhuxunluWidget->setVisible(false);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 0, 480));
        animation4->setEndValue(QRect(880, 210, 220, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    xunLu = true;
    dingWei = false;
    chaZhao = false;

}

void Widget::on_qidianToolButton_clicked()
{
    QCursor my(QPixmap(":/images/start.png"));//�������ͼ��Ϊ��ɫ���ͼ��
    ui->routeWidget->setCursor(my);
    ui->routeWidget->raise();
    ui->routeWidget->isPlacingStartLocation = true;
}

void Widget::on_zhongdianToolButton_clicked()
{

    QCursor my(QPixmap(":/images/end.png"));//�������ͼ��Ϊ��ɫ�յ�ͼ��
    ui->routeWidget->setCursor(my);
    ui->routeWidget->raise();
    ui->routeWidget->isPlacingEndLocation = true;

}

void Widget::on_volumeToolButton_clicked()
{
    if(m_isSilenced == false)
    {
        ui->routeWidget->sm->setSilenced(true);
        ui->volumeToolButton->setIcon(QIcon(":/images/jingyin.png"));
        m_isSilenced = true;
    }
    else
    {
        ui->routeWidget->sm->setSilenced(false);
        ui->volumeToolButton->setIcon(QIcon(":/images/shengyin.png"));
        m_isSilenced = false;
    }
}

/*�������öԻ���
 *������ɫ��Ϊ͸��
 */
void Widget::on_setupDlgToolButton_clicked()
{
    DlgSetup *dlgset = new DlgSetup(this);
    dlgset->setWindowFlags(Qt::FramelessWindowHint);
    dlgset->setAttribute(Qt::WA_TranslucentBackground);
    dlgset->exec();
}


void Widget::on_pushButton_clicked()
{
    showMinimized();
}


void Widget::on_quedingToolButton_clicked()
{
    bool ok = false;
    QPoint startPos(ui->routeWidget->findSpotLocation(ui->qidianLineEdit->text(), &ok));

    if(!ok)
        return;

    QPoint endPos(ui->routeWidget->findSpotLocation(ui->zhongdianLineEdit->text(), &ok));

    if(!ok)
        return;

    if(ui->routeWidget->startWidget != NULL)
    {
        delete ui->routeWidget->startWidget;
        ui->routeWidget->startWidget = NULL;
    }
    ui->routeWidget->startWidget = new LocationWidget(ui->mapFrame);
    ui->routeWidget->startWidget->setPixmap(QPixmap(":/images/start.png"));
    ui->routeWidget->startWidget->setServerCoordinate(startPos);

    if(ui->routeWidget->endWidget != NULL)
    {
        delete ui->routeWidget->endWidget;
        ui->routeWidget->endWidget = NULL;
    }
    ui->routeWidget->endWidget = new LocationWidget(ui->mapFrame);
    ui->routeWidget->endWidget->setPixmap(QPixmap(":/images/end.png"));
    ui->routeWidget->endWidget->setServerCoordinate(endPos);

    ui->routeWidget->locationSet(false);
}

void Widget::on_zizhuxunluToolButton_clicked()
{
    if(isshowShortestRoute == true)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(ui->zizhuxunluWidget, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(0, 210, 0, 280));
        animation->setEndValue(QRect(0, 210, 220, 280));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();
        isshowShortestRoute = false;
    }
    else
    {
        QPropertyAnimation *animation = new QPropertyAnimation(ui->zizhuxunluWidget, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(0, 210, 220, 280));
        animation->setEndValue(QRect(0, 210, 0, 280));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();
        isshowShortestRoute = true;
    }

    ui->zizhuxunluWidget->setVisible(true);
}

void Widget::on_sousuoToolButton_clicked()
{
    if(ui->routeWidget->searchResultWidget)
    {
        delete ui->routeWidget->searchResultWidget;
        ui->routeWidget->searchResultWidget = NULL;
    }

    if(ui->sousuoLineEdit->text().isEmpty())
    {
        return;
    }

    bool isFound = false;
    QPoint pos = ui->routeWidget->findSpotLocation(ui->sousuoLineEdit->text(), &isFound);

    if(!isFound)
    {
        return;
    }

    ui->routeWidget->searchResultWidget = new LocationWidget(ui->mapFrame, pos);

}

void Widget::on_recommendRouteToolButton_clicked()
{
    static bool isSet = false;
    if(!isSet)
    {
        isSet = true;
        ui->mapFrame->provider().setMapType("/SuggestedRouteMap/");
        //ui->recommendRouteToolButton->setText(tr("ȡ������"));
    }
    else
    {
        isSet = false;
        ui->mapFrame->provider().setMapType("/NormalMap/");
    }
}

void Widget::on_beautifulRouteToolButton_clicked()
{
    static bool isSet = false;
    if(!isSet)
    {
        isSet = true;
        ui->mapFrame->provider().setMapType("/ScenicRouteMap/");
        //ui->recommendRouteToolButton->setText(tr("ȡ������"));
    }
    else
    {
        isSet = false;
        ui->mapFrame->provider().setMapType("/NormalMap/");
    }
}

void Widget::on_quxiaoButton_clicked()
{
    ui->routeWidget->isRoute = false;
    ui->routeWidget->m_isRouteByCoor = false;
    ui->routeWidget->update();
    if(ui->routeWidget->startWidget != NULL)
    {
        delete ui->routeWidget->startWidget;
        ui->routeWidget->startWidget = NULL;
    }
    if(ui->routeWidget->endWidget != NULL)
    {
        delete ui->routeWidget->endWidget;
        ui->routeWidget->endWidget = NULL;
    }
    if(ui->routeWidget->userLocationWidget != NULL)
    {
        delete ui->routeWidget->userLocationWidget;
        ui->routeWidget->userLocationWidget = NULL;
    }
    if(ui->routeWidget->searchResultWidget != NULL)
    {
        delete ui->routeWidget->searchResultWidget;
        ui->routeWidget->searchResultWidget = NULL;
    }
}
