/*======================================
 *�����á�ģ��
 *���� �� �µ�����һ���Ի��򣬶ԡ�����ͼ˵��������л�������
 *ģ����Ҫ�� �� ��Ҫ
 *
 *���� �� ��ɣ�����ܲ
 *
 *--------------------------------------
 *���������
 *
 *======================================*/
#include "dlgsetup.h"
#include "ui_dlgsetup.h"

DlgSetup::DlgSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetup)
{
    ui->setupUi(this);

    master = (Widget *)parentWidget();
    if(master == NULL)
    {
        QMessageBox::critical(this, tr("��Ϸ����"), tr("��������"), QMessageBox::Ok);
        QCoreApplication::quit();
    }

    //��ȡ��������
    if(master->getMapFrame()->provider().getDataSource() == MapTilesProvider::NetworkData)
    {
        on_netDataToolButton_clicked();
    }
    else
    {
        on_localDataToolButton_clicked();
    }

    if(master->getRouteWidget()->sm->isSilenced())
    {
        on_endVoiceToolButton_clicked();
    }
    else
    {
        on_startVoiceToolButton_clicked();
    }


}

DlgSetup::~DlgSetup()
{
    delete ui;
}

void DlgSetup::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DlgSetup::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}


void DlgSetup::on_exitPushButton_clicked()
{
    this->reject();
}

void DlgSetup::on_quxiaoPushButton_clicked()
{
    this->reject();
}

void DlgSetup::on_quedingPushButton_clicked()
{
    if(m_isSilenced)
    {
        master->getRouteWidget()->sm->setSilenced(true);
    }
    else
    {
        master->getRouteWidget()->sm->setSilenced(false);
    }


    //�жϵ�ַ������ĵ�ַ�Ƿ�Ϸ���
    if(m_useNetworkDataChecked)
    {
        //TODO: isValid()�е�����
        if(QUrl(ui->dizhiLineEdit->text()).isValid())
        {
            master->getMapFrame()->provider().setDataSource(MapTilesProvider::NetworkData);
            master->getMapFrame()->provider().setServerAddress(ui->dizhiLineEdit->text().trimmed());
        }
        else
        {
            QMessageBox::critical(this,
                                  tr("��������ַ���Ϸ�"),
                                  tr("������ĵ�ͼ���ݷ�������ַ���Ϸ����뷵���������룡"),
                                  QMessageBox::Ok);
            return;
        }
    }
    else
    {
        master->getMapFrame()->provider().setDataSource(MapTilesProvider::LocalData);
        master->getMapFrame()->provider().setServerAddress(QDir::fromNativeSeparators(ui->dizhiLineEdit->text()).trimmed());
    }
    accept();
}

//�����ͼ�����ߵ�ͼradiobutton��ʵ��
void DlgSetup::on_netDataToolButton_clicked()
{
    m_useNetworkDataChecked = true;
    ui->netDataToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->localDataToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->addressLabel->setStyleSheet("border-image:url(:/images/serverAddr.png)");
    if(master->getMapFrame()->provider().getDataSource() == MapTilesProvider::NetworkData)
    {
        ui->dizhiLineEdit->setText(master->getMapFrame()->provider().getServerAddress());
    }
    else
    {
        ui->dizhiLineEdit->setText("http://");
    }
    ui->browsePushButton->hide();
}

void DlgSetup::on_localDataToolButton_clicked()
{
    m_useNetworkDataChecked = false;
    ui->localDataToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->netDataToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->addressLabel->setStyleSheet("border-image:url(:/images/localAddr.png)");
    if(master->getMapFrame()->provider().getDataSource() == MapTilesProvider::LocalData)
    {
        ui->dizhiLineEdit->setText(master->getMapFrame()->provider().getServerAddress());
    }
    else
    {
        ui->dizhiLineEdit->setText("");
    }
    ui->browsePushButton->show();
}

//Q���ͼ����ͨ��ͼradiobutton��ʵ��
void DlgSetup::on_commonMapToolButton_clicked()
{
    ui->commonMapToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->qMapToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
}

void DlgSetup::on_qMapToolButton_clicked()
{
    ui->commonMapToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->qMapToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
}

//�����������ر�����radiobutton��ʵ��
void DlgSetup::on_startVoiceToolButton_clicked()
{
    m_isSilenced = false;
    ui->startVoiceToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->endVoiceToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
}

void DlgSetup::on_endVoiceToolButton_clicked()
{
    m_isSilenced = true;
    ui->startVoiceToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->endVoiceToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
}

//��ȡ���ص�ͼ��ַ
void DlgSetup::on_browsePushButton_clicked()
{
    //QFileDialog::HideNameFilterDetails

    QString dir = QFileDialog::getExistingDirectory(this, tr("ѡ�����ߵ�ͼ�ļ���"),
                                                    QCoreApplication::applicationDirPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->dizhiLineEdit->setText(QDir::toNativeSeparators(dir));
}
