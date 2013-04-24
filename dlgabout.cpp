/*======================================
 *�����ڵ���ͼ˵��ģ��
 *���� �� �µ�����һ���Ի�����ʾ���ڡ�����ͼ˵���������Ϣ
 *ģ����Ҫ�� �� һ��
 *
 *���� �� ���
 *
 *--------------------------------------
 *���������
 *
 *======================================*/
#include "dlgabout.h"
#include "ui_dlgabout.h"

DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAbout)
{
    ui->setupUi(this);
}

DlgAbout::~DlgAbout()
{
    delete ui;
}

void DlgAbout::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DlgAbout::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void DlgAbout::on_quitPushButton_clicked()
{
    this->close();
}


