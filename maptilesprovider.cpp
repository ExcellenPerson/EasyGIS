/*============================================================
 * ��ͼ��Ƭ�ṩģ��
 * ���� �� ���ݵ�ͼ��ʾģ������󣬴������ϻ�ȡ��Ƭ���ݣ�����ʹ�ã�
 *        ���߱�������ƣ��Զ�����ȡ�������ݱ����ڻ����У��´�ȡ��ʱ�����ٴ�������ȡ
 * ģ����Ҫ�ԣ� ��Ҫ
 *
 * ���� �� ����ܲ
 *
 * -----------------------------------------------------------
 * ������� ��
 *
 * 2013.3.14 �� MapTilesFrame �ж�������ģ��
 * 2013.3.16 ���������ڴ�й©����
 *
 ============================================================*/

#include "maptilesprovider.h"

/*============================================================
 * MapTilesProvider ���캯��
 * ����˵�� �� ��ʼ������ı�����������������ɵ��źŵ������Զ���Ĳۺ���
 *
 * ������Ҫ�ԣ� ��Ҫ
 *
 * ����˵�� �� ָ�򸸶����ָ�루Qt Ĭ�ϲ�����
 * ����ֵ  :  ��
 * ����    �� ����ܲ
 * -----------------------------------------------------------
 * ������� ��
 *
 ============================================================*/

MapTilesProvider::MapTilesProvider(QObject *parent) :
    QObject(parent)
{
    setMapType("/NormalMap/");
    setDataSource(LocalData);
    setServerAddress(QCoreApplication::applicationDirPath() + "/���ߵ�ͼ��/");
    setTileFileType(".png");

    connect(&nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(slotFinished(QNetworkReply*)));

}


/*============================================================
 * ��Ƭ��ȡ����
 * ����˵�� �� ���Դӻ����м��ص�ͼ��ʾģ���������Ƭ����������ָ��
 *           ���������
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
QImage *MapTilesProvider::getTile(const QString &tileName, bool getDirectPath)
{
    //Note: QImage ʹ����Implicit Data Sharing������һ��QImage�����ٶȺܿ�

    QString path;

    if(getDirectPath)
    {
        path = getServerAddress() + tileName;
    }
    else
    {
        path = getServerAddress() + getMapType() + tileName + getTileFileType();
    }

    qDebug() << "path: " << path;

    if(imgCache[path] != NULL)
    {
        qDebug() << "tile is CACHED" << path;
        return new QImage(*imgCache[path]);
    }
    else
    {
        if(getDataSource() == NetworkData)
        {
            QNetworkRequest request;
            request.setUrl(QUrl(path));
            request.setAttribute(QNetworkRequest::User, path);
            request.setAttribute(QNetworkRequest::UserMax, getDirectPath);

            qDebug() << "tile is NOT CACHED, sending request: " << request.url().toString();

            nam.get(request);

            return NULL;
        }
        else
        {
            qDebug() << "reading local tile file " << path;
            QImage *img = new QImage(path);
            if(img->isNull())
            {
                qDebug() << "file not exist or corrupted";
                return NULL;
            }
            else
            {
                if(!imgCache.insert(path, img))
                {
                    qDebug() << "ERROR!!! failed to insert local file " << path << " into cache";
                    qDebug() << "Cache size at this time" << imgCache.size();
                }
                return new QImage(*img);
            }
        }

    }
}

//QImage *MapTilesProvider::getImage(const QString &imagePath)
//{
//    //Note: QImage ʹ����Implicit Data Sharing������һ��QImage�����ٶȺܿ�

//    QString path(getServerAddress() + imagePath);
//    qDebug() << "path: " << path;

//    if(imgCache[path] != NULL)
//    {
//        qDebug() << "tile is CACHED" << path;
//        return new QImage(*imgCache[path]);
//    }
//    else
//    {
//        if(getDataSource() == NetworkData)
//        {
//            QNetworkRequest request;
//            request.setUrl(QUrl(path));
//            request.setAttribute(QNetworkRequest::User, path);

//            qDebug() << "tile is NOT CACHED, sending request: " << request.url().toString();

//            nam.get(request);

//            return NULL;
//        }
//        else
//        {
//            qDebug() << "reading local tile file " << path;
//            QImage *img = new QImage(path);
//            if(img->isNull())
//            {
//                qDebug() << "file not exist or corrupted";
//                return NULL;
//            }
//            else
//            {
//                if(!imgCache.insert(path, img))
//                {
//                    qDebug() << "ERROR!!! failed to insert local file " << path << " into cache";
//                    qDebug() << "Cache size at this time" << imgCache.size();
//                }
//                return new QImage(*img);
//            }
//        }

//    }
//}

QString MapTilesProvider::getMapType() const
{
    return m_mapType;
}

void MapTilesProvider::setMapType(QString mapType)
{
    m_mapType = mapType;
    emit mapTypeChanged(mapType);
}

MapTilesProvider::DataSource MapTilesProvider::getDataSource() const
{
    return m_dataSource;
}

void MapTilesProvider::setDataSource(MapTilesProvider::DataSource dataSource)
{
    m_dataSource = dataSource;
    emit dataSourceChanged(dataSource);
}

QString MapTilesProvider::getServerAddress() const
{
    return QString(m_serverAddress);
}

void MapTilesProvider::setServerAddress(const QString &newServerAddress)
{
    if(!QUrl(newServerAddress).isValid())
    {
        //qDebug() << "NOT A VALID newServerAddress!";
        return;
    }
    else
    {
        m_serverAddress = newServerAddress;
        emit serverAddressChanged(m_serverAddress);
    }
}

QString MapTilesProvider::getTileFileType() const
{
    return QString(m_tileFileType);
}

void MapTilesProvider::setTileFileType(const QString &newTileFileType)
{
    m_tileFileType = newTileFileType;
    emit tileFileTypeChanged(m_tileFileType);
}

void MapTilesProvider::slotFinished(QNetworkReply *reply)
{
    QString path = reply->request().attribute(QNetworkRequest::User).toString();
    bool getDirectPath = reply->request().attribute(QNetworkRequest::UserMax).toBool();

    qDebug() << "slotFinished called for tile: " << path;

    if(reply->error())
    {
        qDebug() << "network reply ERROR for tile: " << path;
        return;
    }

    QImage *replyImg = new QImage(QImage::fromData(reply->readAll()));

    //�ж����紫�ص�ͼƬ�Ƿ���
    if(replyImg->isNull())
    {
        qDebug() << "reply data is NOT A VALID IMAGE for tile: " << path;
        return;
    }
    else
    {
        if(!imgCache.insert(path, replyImg))
        {
            qDebug() << "ERROR!!! failed to insert " << path << " into cache";
            qDebug() << "Cache size at this time" << imgCache.size();
            return;
        }
    }

    //QImage *tileImg = new QImage(*replyImg);

    if(getDirectPath)
    {
        emit readyImage(path);
    }
    else
    {
        emit readyTile(path);
    }
}
