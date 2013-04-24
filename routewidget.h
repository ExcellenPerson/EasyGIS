#ifndef ROUTEWIDGET_H
#define ROUTEWIDGET_H

#include <QtGui>
#include <QtSql>
#include <QWidget>
#include <qmath.h>
#include "graphics.h"
#include "maptilesframe.h"
#include "locationwidget.h"
#include "soundmanager.h"

#include <assert.h>

class SoundManager;

class RouteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RouteWidget(QWidget *parent = 0);
    void initSqlDatabase();
    QList<QList<QPoint> > convertCoordinate(QList<QList<QPoint> > pointList);
    void initAbsPoints();

    QPoint findNearestSpotLocation(QPoint pos);
    QString findNearestSpotName(QPoint pos);

    QString findSpotName(QPoint pos, bool* isFound = 0);
    QPoint findSpotLocation(QString spotName, bool* isFound = 0);

    bool isPlacingStartLocation;   //�û��Ƿ����ڷ������
    bool isPlacingEndLocation;     //�û��Ƿ����ڷ����յ�

    LocationWidget *userLocationWidget;
    LocationWidget *searchResultWidget;

    LocationWidget *startWidget;
    LocationWidget *endWidget;

    SoundManager *sm;

    bool isRoute;
    bool m_isRouteByCoor;//�Ƿ���������������Ѱ·

    //����������ֵ��������ϵ�Ƿ�������������ϵ������������
    QList<QPoint> getRouteTurningPoints(QPoint startPoint, QPoint endPoint);

signals:
    
public slots:
    //���ڽ��շ��������Ĳۺ���
    void locationMultipleSlot(const QPoint& scalePos, int newScaleLevel, int oldScaleLevel);
    //���ʵ�ʴ����ڷ�������λ�ã����������·��ͼ��
    void clientWinLocSlot(QPoint newPos, QPoint oldPos);
    //���չؼ������ֵĲۺ���
    void receiveNodeNameSlot(QString startName, QString endName);
    //���չؼ�������Ĳۺ���
    void receiveCoordinateSlot(QPoint startPosServerCoordinate, QPoint endPosServerCoordinate, bool isRouteByCoor = true);

    //�ж������յ��Ƿ񶼱����ã����������������û���·������
    void locationSet(bool isRouteByCoor = true);

private:
    QPen pen;
    Graphics* g;
    int multiple;
    int width;
    int height;

    QSqlDatabase db;

    QList<QList<QPoint> > absPointsList;//������ͨ�����λ��
    QPoint actualPos;//����ʵ�ʴ���λ��
    MapTilesFrame* mapFrame;

    bool isStartLocationSet;  //����Ƿ��ѱ����ú�
    bool isEndLocationSet;    //�յ��Ƿ��ѱ����ú�

    QPoint routeStartNode;
    QPoint routeEndNode;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);

};

#endif // ROUTEWIDGET_H
