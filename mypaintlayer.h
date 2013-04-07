#ifndef MYPAINTLAYER_H
#define MYPAINTLAYER_H

#include <marble/MarbleWidget.h>
#include <marble/MarbleMap.h>
#include <marble/MarbleModel.h>
#include <marble/GeoPainter.h>
#include <marble/LayerInterface.h>

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>

using namespace Marble;

class MyPaintLayer : public QObject, public LayerInterface
{
public:
    // Constructor
    MyPaintLayer(MarbleWidget* widget);

    // Implemented from LayerInterface
    virtual QStringList renderPosition() const;

    // Implemented from LayerInterface
    virtual bool render( GeoPainter *painter, ViewportParams *viewport,
    const QString& renderPos = "NONE", GeoSceneLayer * layer = 0 );

    // Overriding QObject
    virtual bool eventFilter(QObject *obj, QEvent *event);

    GeoDataCoordinates approximate(const GeoDataCoordinates &base, qreal angle, qreal dist) const;

    double latitude;
    double longitude;

private:
    MarbleWidget* m_widget;

    int m_index;
};


#endif // MYPAINTLAYER_H
