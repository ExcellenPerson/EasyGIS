/*============================================================
 * ��ͼ��Ƭ��ʾģ��
 * ���� �� ������Ҫ��ʾ�ĵ�ͼ������ʾ��Щ��ͼ��Ƭ��
 *        ���ṩ��Ӧ API �ӿڣ���Ӧ�û������Ϸ�����ͼ���ƶ���ͼ�ӿڵȲ���
 * ģ����Ҫ�ԣ� ��Ҫ
 *
 * ���� �� ����ܲ
 *
 * -----------------------------------------------------------
 * ������� ��
 *
 ============================================================*/

#include "maptilesframe.h"
#include "ui_maptilesframe.h"

MapTilesFrame::MapTilesFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapTilesFrame)
{
    ui->setupUi(this);

    connect(this, SIGNAL(movingStatusChanged(bool)),
            this, SLOT(updateMouseCursor(bool)));
    connect(this, SIGNAL(viewportCoordinateChanged(QPoint,QPoint)),
            this, SLOT(update()));
    connect(this, SIGNAL(scaleLevelChanged(const QPoint&,int,int)),
            this, SLOT(adjustViewportOnZoom(const QPoint&,int,int)));

    connect(&tileProvider, SIGNAL(mapTypeChanged(QString)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(dataSourceChanged(DataSource)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(serverAddressChanged(QString)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(tileFileTypeChanged(QString)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(readyTile(QString)), this, SLOT(update()));


//    connect(this, SIGNAL(scaleLevelChanged(QPoint,int,int)),
//            this,SLOT(setAppropriateSize()));

    //mapFilesPath = QCoreApplication::applicationDirPath() + "/map";

    tileWidth = 375;
    tileHeight = 250;

    for(int i = 1; i <= MAX_LEVEL; i++)
    {
        scaleMaps[i].width = 2 * i * tileWidth;
        scaleMaps[i].height = 2 * i * tileHeight;
    }

    biggerMapRect = new QImage();
    //biggerMapRect = QImage(9999, 9999, QImage::Format_RGB16);

    m_viewportCoordinate = QPoint(0,height());
    //setViewportCoordinate(QPoint(0,height()));
    setMoving(false);

    //������scaleLevelΪһ�������ڵ�ֵ 0 ��Ȼ�����zoomInʹ�ӿڷ����������������С���Ӽ���
    m_scaleLevel = 1;
    //zoomIn();

    setViewportCoordinate(QPoint(0,height()));

}



MapTilesFrame::~MapTilesFrame()
{
    delete ui;
}

void MapTilesFrame::setMoving(bool movingStatus)
{
    m_isMoving = movingStatus;
    emit movingStatusChanged(movingStatus);
}

QPoint MapTilesFrame::getViewportCoordinate() const
{
    return m_viewportCoordinate;
}

void MapTilesFrame::setViewportCoordinate(const QPoint &requestedPos)
{
    QPoint adjustedPoint;
    QPoint oldPoint(m_viewportCoordinate);

    adjustedPoint.rx() = qBound(0, requestedPos.x(), scaleMaps[getScaleLevel()].width - getViewportWidth());
    adjustedPoint.ry() = qBound(getViewportHeight(), requestedPos.y(), scaleMaps[getScaleLevel()].height);

    m_viewportCoordinate = adjustedPoint;

    qDebug() << "Requested viewportPoint: " << requestedPos << ", adjustedPoint" << adjustedPoint;
    emit viewportCoordinateChanged(adjustedPoint, oldPoint);

}

int MapTilesFrame::getViewportWidth() const
{
    if(width() > scaleMaps[getScaleLevel()].width)
    {
        //isViewportBiggerThanWholeMap = true;
        return scaleMaps[getScaleLevel()].width;
    }
    else
    {
        //isViewportBiggerThanWholeMap = false;
        return width();
    }
}

int MapTilesFrame::getViewportHeight() const
{
    if(height() > scaleMaps[getScaleLevel()].height)
    {
        //isViewportBiggerThanWholeMap = true;
        return scaleMaps[getScaleLevel()].height;
    }
    else
    {
        //isViewportBiggerThanWholeMap = false;
        return height();
    }
}

QSize MapTilesFrame::getViewportSize() const
{
    return QSize(getViewportWidth(), getViewportHeight());
}

int MapTilesFrame::getScaleLevel() const
{
    return m_scaleLevel;
}

void MapTilesFrame::setScaleLevel(const QPoint& scalePos, int scaleLevel)
{
    int oldLevel = m_scaleLevel;

    if(scaleLevel >= 1 && scaleLevel <= MAX_LEVEL)
    {
        m_scaleLevel = scaleLevel;
        emit scaleLevelChanged(scalePos, scaleLevel, oldLevel);
    }
}

void MapTilesFrame::zoomIn(const QPoint& zoomPos)
{
    setScaleLevel(zoomPos, getScaleLevel() + 1);
}

void MapTilesFrame::zoomOut(const QPoint& zoomPos)
{
    setScaleLevel(zoomPos, getScaleLevel() - 1);
}

void MapTilesFrame::zoomIn()
{
    qDebug() << "zooming IN . . . viewport coordinate: " << getViewportCoordinate();
    qDebug() << "viewport size: " << size();

//    ZoomEffect *ze = createZoomEffect(QPoint(width() / 2, height() / 2));
//    ze->zoomInEffect();

    //���ӿ����ĵ�Ϊ�ӵ����ţ�����������ϵ��
    QPoint viewportCenterPoint(getViewportCoordinate().x() + getViewportWidth() / 2,
                               getViewportCoordinate().y() - getViewportHeight() / 2);
    zoomIn(viewportCenterPoint);
}

void MapTilesFrame::zoomOut()
{
    qDebug() << "zooming OUT . . . viewport coordinate: " << getViewportCoordinate();
    qDebug() << "viewport size: " << size();

//    ZoomEffect *ze = createZoomEffect(QPoint(width() / 2, height() / 2));
//    ze->zoomOutEffect();

    //���ӿ����ĵ�Ϊ�ӵ����ţ�����������ϵ��
    QPoint viewportCenterPoint(getViewportCoordinate().x() + getViewportWidth() / 2,
                               getViewportCoordinate().y() - getViewportHeight() / 2);
    zoomOut(viewportCenterPoint);
}

QPoint MapTilesFrame::convertServerCoordinateToDifferentScaleLevel(QPoint serverCoordinate, int newScaleLevel, int oldScaleLevel)
{
    return serverCoordinate * newScaleLevel / oldScaleLevel;
}

QPoint MapTilesFrame::convertRelativeCoordinateToServerCoordinate(QPoint relativeCoordinate, int newScaleLevel, int oldScaleLevel)
{
    QPoint nowPos(relativeCoordinate.x() * getViewportWidth() / width() + getViewportCoordinate().x(),
                   getViewportCoordinate().y() - relativeCoordinate.y() * getViewportHeight() / height());
    return nowPos * newScaleLevel / oldScaleLevel;
}


QPoint MapTilesFrame::convertRelativeCoordinateToServerCoordinate(QPoint relativeCoordinate)
{
    return convertRelativeCoordinateToServerCoordinate(relativeCoordinate, 1, 1);
}

QPoint MapTilesFrame::convertServerCoordinateToRelativeCoordinate(QPoint serverCoordinate)
{
    return convertServerCoordinateToRelativeCoordinate(serverCoordinate, 1, 1);
}

QPoint MapTilesFrame::convertServerCoordinateToRelativeCoordinate(QPoint serverCoordinate, int newScaleLevel, int oldScaleLevel)
{
    QPoint nowPos(serverCoordinate * newScaleLevel / oldScaleLevel);
    return QPoint(nowPos.x() - getViewportCoordinate().x(), getViewportCoordinate().y() - nowPos.y());
}

bool MapTilesFrame::isInCurrentViewport(QPoint serverCoordinate, int scaleLevel)
{
    QPoint relativePoint(convertServerCoordinateToRelativeCoordinate(serverCoordinate, getScaleLevel(), scaleLevel));
    return QRect(0,0,getViewportWidth(),getViewportHeight()).contains(relativePoint);
}

bool MapTilesFrame::isInMap(QPoint serverCoordinate, int scaleLevel)
{
    QRect mapRect(0, 0, scaleMaps[scaleLevel].width, scaleMaps[scaleLevel].height);
    return mapRect.contains(serverCoordinate);
}

double MapTilesFrame::distance(QPoint point1, QPoint point2)
{
    QPoint diff(point1 - point2);
    return sqrt(pow((double)diff.x(), 2) + pow((double)diff.y(), 2));
}

MapTilesProvider &MapTilesFrame::provider()
{
    return tileProvider;
}

bool MapTilesFrame::isMoving() const
{
    return m_isMoving;
}

void MapTilesFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        timeToGo = 4;
        setMoving(true);

        dragPosition = QPoint(event->x(), this->height() - event->y());
        event->accept();
    }
}

void MapTilesFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (isMoving())
    {
        //������ļ������ﵽһ������ʱ������Ӧһ������ƶ��¼�
        ++timeToGo;
        if(timeToGo >= 5)
        {
            timeToGo = 0;

            setViewportCoordinate(getViewportCoordinate() - QPoint(event->x(), this->height() - event->y()) + dragPosition);
            //move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
}

void MapTilesFrame::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setMoving(false);
    event->accept();
}

void MapTilesFrame::wheelEvent(QWheelEvent *event)
{
    ZoomEffect *ze = createZoomEffect(event->pos());
    QPoint zoomPos(convertRelativeCoordinateToServerCoordinate(event->pos()));

    if (event->orientation() == Qt::Vertical) {
        if(event->delta() > 0)
        {
            ze->zoomInEffect();
            zoomIn(zoomPos);
        }
        else
        {
            ze->zoomOutEffect();
            zoomOut(zoomPos);
        }
    }

    event->accept();
}

void MapTilesFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

//    if(getScaleLevel() == 1)
//    {
//        setFixedSize(750,500);
//        updateGeometry();
//    }
//    else
//    {
//        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
//        updateGeometry();
//    }

    prepareViewport(getViewportCoordinate());

    if(isViewportBiggerThanWholeMap)
    {
        int scaleLevel = getScaleLevel();
        QPoint startDrawPoint((width() - scaleMaps[scaleLevel].width) / 2,
                (height() - scaleMaps[scaleLevel].height) / 2);
        p.drawImage(startDrawPoint, *biggerMapRect, clippedMapRect);
    }
    else
    {
        p.drawImage(rect(), *biggerMapRect, clippedMapRect);
    }

}

void MapTilesFrame::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resizeEvent called, newSize: " << event->size() << ", oldSize: " << event->oldSize();

    qDebug() << "viewport Coordinate: " << getViewportCoordinate();

    //update();
    //setViewportCoordinate(getViewportCoordinate());

    //ͨ�����������ӿ�����ķ�����ʹ�ӿ������ݵõ�����
    //setViewportCoordinate(getViewportCoordinate());

}

/*============================================================
 * �������ָ����ʽ����
 *��Qt �ۺ�����
 * ����˵��  �� ���ݵ�ǰ�û��Ƿ����϶���ͼ���任���Ӧ�����ָ����ʽ
 * ������Ҫ�� �� һ��
 *
 * ����˵�� �� �����������û����ƶ��ӿ�ʱ�ñ���Ϊ�棬��֮Ϊ��
 * ����ֵ  :  ��
 * ����    �� ��ɡ�����ܲ
 * -----------------------------------------------------------
 * ������� ��
 *
 ============================================================*/
void MapTilesFrame::updateMouseCursor(bool currentMovingStatus)
{
    //qDebug() << "updateMousecursor called, CurrentMovingStatus = " << currentMovingStatus;
    if(currentMovingStatus)
    {
        setCursor(Qt::ClosedHandCursor);
    }
    else
    {
        setCursor(Qt::OpenHandCursor);
    }
}

/*============================================================
 * �ӿ�׼������
 *��Qt �ۺ�����
 * ����˵�� �� ���㲢���ؿͻ��˵�ͼ�ӿ�����Ҫ��ʾ�ĵ�ͼ��Ƭ
 *           �� paintEvent ʹ��
 * ������Ҫ�ԣ� ��Ҫ
 *
 * ����˵�� �� �µ��ӿ�����
 *           ���ӿ�����Ϊ�ӿڵ����Ͻǵ����꣬�Է���������ϵΪ��׼��
 * ����ֵ  :  ��
 * ����    �� ����ܲ
 * -----------------------------------------------------------
 * ������� ��
 *
 ============================================================*/
bool MapTilesFrame::prepareViewport(QPoint newPos)
{
    //��������Ҫ��ɵľ��������ǣ����㲢׼����biggerMapRect��clippedMapRect���� paintEvent ʹ��

    qDebug() << "newPos: " << newPos;

    static QString tilePath("/s%1/s%2_%3-%4_%5-%6");
    //static QImage tile(tileWidth, tileHeight, QImage::Format_RGB16);
    static QImage noDataImage(":/images/notile.bmp");    //��ʾ���������ݡ���ͼƬռλ��������һ����Ƭ��С��

    int scaleLevel = getScaleLevel();     //��ǰ���ż���

    int w = width();
    int h = height();

    int mapWidth = scaleMaps[scaleLevel].width;
    int mapHeight = scaleMaps[scaleLevel].height;

     //�ж��û���ʾ�ӿڴ�С�Ƿ���ڵ�ǰ���ż�������ŵ�ͼ��С
    if(w > mapWidth || h > mapHeight)
    {
        qDebug() << "viewport is bigger that whole map!";
        qDebug() << "viewportSize: " << size() << ", mapWidth: " << mapWidth << ", mapHeight: " << mapHeight;
        isViewportBiggerThanWholeMap = true;

        w = mapWidth;
        h = mapHeight;
    }
    else
    {
        isViewportBiggerThanWholeMap = false;
    }

    //���ӿ������X��Y�Ƿ��ڵ�ͼ�������ϣ���X/Y����ֵ�Ƿ��ܱ�����Ƭ���/�߶�������
    //���ж��ں�������л��õ�
    bool isSpecialX = false;
    bool isSpecialY = false;
    if(newPos.x() % tileWidth == 0)
    {
        isSpecialX = true;
        //qDebug() << "Special X!!!";
    }
    if(newPos.y() % tileHeight == 0)
    {
        isSpecialY = true;
        //qDebug() << "Special Y!!!";
    }

    //��Ϊ�ӿ������������Ƭ�����½ǵ㣬���¼��д�����Գɹ�ʵ�ֻ�ȡ�õ�����
    QPoint tileBottomLeftCorner;
    tileBottomLeftCorner.rx() = newPos.x() - newPos.x() % tileWidth;
    tileBottomLeftCorner.ry() = newPos.y() - newPos.y() % tileHeight;
    if(isSpecialY)
    {
        tileBottomLeftCorner.ry() -= tileHeight;
    }

    //����biggerMapRectռ�˼��С�������Ƭ
    //numRows��ʾ��ռ������numColumns��ʾ��ռ����
    //TODO: �㷨�д��Ż�
    int numRows;
    int numColumns;
    if(isViewportBiggerThanWholeMap)
    {
        numRows = mapHeight / tileHeight;
        numColumns = mapWidth / tileWidth;
    }
    else
    {
        int ceiling = newPos.y() / tileHeight * tileHeight + (newPos.y() % tileHeight == 0 ? 0 : tileHeight);
        int bottom = (newPos.y() - h) / tileHeight * tileHeight;
        numRows = (ceiling - bottom) / tileHeight;

        int rightX = newPos.x() + width();
        int rightOffset = ((rightX % tileWidth == 0) ? 0 : tileWidth);
        int leftBorder = newPos.x() - newPos.x() % tileWidth;
        int rightBorder = rightX / tileWidth * tileWidth + rightOffset;
        numColumns = (rightBorder - leftBorder) / tileWidth;
    }


    //biggerMapRect��Ҫ�Ĵ�С
    //���biggerMapRect�����������´����㹻���biggerMapRect
    QSize biggerMapRectSize(numColumns * tileWidth, numRows * tileHeight);
    if(biggerMapRect->width() < biggerMapRectSize.width()
            || biggerMapRect->height() < biggerMapRectSize.height())
    {
        delete biggerMapRect;
        biggerMapRect = new QImage(biggerMapRectSize, QImage::Format_RGB16);
    }


    QPainter painter(biggerMapRect);  //painter������Ϊstatic��
    QString finalPath;     //���յ���Ƭ�ļ���ַ

    //biggerMapRectҪ���ص�ÿ����Ƭ�����½���������Ͻ�����
    QPoint tempBL;
    QPoint tempTR;

//    qDebug() << "tileBottomLeftCorner:" << tileBottomLeftCorner;
//    qDebug() << "viewportCoordinate:" << newPos;
//    qDebug() << "viewportSize:" << size();
//    qDebug() << "numRows: " << numRows << ", numColumns: " << numColumns;

    QImage *tile = NULL;
    for(int m = 0; m < numRows; m++)
    {
        for(int n = 0; n < numColumns; n++)
        {
            tempBL.rx() = tileBottomLeftCorner.x() + n * tileWidth;
            tempBL.ry() = tileBottomLeftCorner.y() - m * tileHeight;
            tempTR.rx() = tempBL.x() + tileWidth;
            tempTR.ry() = tempBL.y() + tileHeight;

            //qDebug() << "tempBL" << tempBL;
            //qDebug() << "tempTR" << tempTR;

            finalPath = tilePath.arg(scaleLevel)
                    .arg(scaleLevel)
                    .arg(tempBL.x())
                    .arg(tempBL.y())
                    .arg(tempTR.x())
                    .arg(tempTR.y());
            //qDebug() << "loading tile(" << m << "," << n << "): " << finalPath;

            tile = tileProvider.getTile(finalPath);
            if(!tile)
            {
                qDebug() << "No tile data: " << finalPath;
                tile = &noDataImage;
                painter.drawImage(n * tileWidth, m * tileHeight, *tile);
            }
            else
            {
                painter.drawImage(n * tileWidth, m * tileHeight, *tile);
                delete tile;
                tile = NULL;
            }

        }
    }

    //��Ϊ�ӿ���biggerMapRect�е�������꣨����ϵΪQt����ϵ���Ƿ���������ϵ��
    QPoint viewportRelativeCoodinate(newPos.x() % tileWidth,
                                     isSpecialY ? 0 : (tileHeight - newPos.y() % tileHeight));

    //��Ϊ��biggerMapRect�ü������ģ�����Ҫ��������Ļ�ϵ���ʾ����
    clippedMapRect = QRect(viewportRelativeCoodinate,
                           isViewportBiggerThanWholeMap ? QSize(mapWidth, mapHeight): size());

    qDebug() << "biggerMapRect" << biggerMapRect->size();
    qDebug() << "clippedMapRect: " << clippedMapRect;

    return isViewportBiggerThanWholeMap;
}

ZoomEffect *MapTilesFrame::createZoomEffect(const QPoint &pos)
{
    ZoomEffect *ze = new ZoomEffect(this);
    ze->move(pos.x() - ze->width() / 2, pos.y() - ze->height() / 2);
    QTimer::singleShot(400,ze,SLOT(deleteLater()));
    return ze;
}

void MapTilesFrame::adjustViewportOnZoom(const QPoint &scalePos, int newScaleLevel, int oldScaleLevel)
{
    qDebug() << "adjustViewportOnZoom called. scalePos: " << scalePos;
    qDebug() << "newScaleLevel: " << newScaleLevel << ", oldScaleLevel: " << oldScaleLevel;

    //�û�ָ�������ŵ㣨scalePos��������ӿ�λ�ã�viewportCoordinate�������ƫ��
    QPoint diff(scalePos - getViewportCoordinate());

    //�û�ָ�������ŵ㣨scalePos�����µķ��������µ�����λ�ã��Է���������ϵΪ��׼��
    QPoint newScalePos(scalePos * newScaleLevel / oldScaleLevel);

    setViewportCoordinate(newScalePos - diff);
}
