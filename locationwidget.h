#ifndef LOCATIONWIDGET_H
#define LOCATIONWIDGET_H

//#include <QWidget>
#include <QtGui>
#include <assert.h>
#include <time.h>
#include "maptilesframe.h"

namespace Ui {
class LocationWidget;
}

class MapTilesFrame;

class LocationWidget : public QWidget
{
    Q_OBJECT
    
public:

    explicit LocationWidget(QWidget *parent, QPoint serverCoordinate = QPoint());
    ~LocationWidget();

    void moveToServerCoordinate(QPoint newPosition);

    void setServerCoordinate(QPoint newPosition);

    QPoint getServerCoordinate() const;

    QPoint getRelativeCoordinate(int scaleLevel) const;

    QPoint getRandomPos() const;
    QPoint getTrulyRandomPos() const;
    QPoint setRandomPos();

    static const QPoint offset;

    void setPixmap(const QPixmap& pixmap);

private:
    Ui::LocationWidget *ui;

    QPoint m_serverPos;

    MapTilesFrame *mapFrame;

    //������Ҫ�������������ӿڣ��������ñ��ؼ�ֻ��ͨ�����÷���������ϵ����������λ�ã�������ͨ����������
    void move(const QPoint &point);
    void setGeometry(const QRect &);

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void serverCoordinateChanged(QPoint newPos, QPoint oldPos);
public slots:
    void checkVisibility();
};

#endif // LOCATIONWIDGET_H
