#include "mypaintlayer.h"

///////////////////////////////////////////////
///Конструктор слоя
///////////////////////////////////////////////
MyPaintLayer::MyPaintLayer(MarbleWidget* widget) : m_widget(widget), m_index(0)
{
    // nothing to do
}

QStringList MyPaintLayer::renderPosition() const
{
    // We will paint in exactly one of the following layers.
    // The current one can be changed by pressing the '+' key
    QStringList layers = QStringList() << "SURFACE" << "HOVERS_ABOVE_SURFACE";
    layers << "ORBIT" << "USER_TOOLS" << "STARS";

    int index = m_index % layers.size();
    return QStringList() << layers.at(index);
}

///////////////////////////////////
///Обработка событий при работе с картой
///////////////////////////////////
bool MyPaintLayer::eventFilter(QObject *obj, QEvent *event)
{
    // Adjust the current layer when '+' is pressed
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Plus) {
            ++m_index;
            return true;
        }
    }

    return false;
}
///////////////////////////////////////////
///
///////////////////////////////////////////
GeoDataCoordinates MyPaintLayer::approximate(const GeoDataCoordinates &base, qreal angle, qreal dist) const
{
    // This is just a rough estimation that ignores projections.
    // It only works for short distances. Don't use in real code.
    GeoDataCoordinates::Unit deg = GeoDataCoordinates::Degree;
    return GeoDataCoordinates ( base.longitude(deg) + 1.5 * dist * sin(angle),
                base.latitude(deg) + dist * cos(angle), 0.0, deg);
}

////////////////////////////////////////////
///Отрисовка маркера и текущего положения
////////////////////////////////////////////
bool MyPaintLayer::render( GeoPainter *painter, ViewportParams *viewport,
    const QString& renderPos, GeoSceneLayer * layer )
{
    // Have window title reflect the current paint layer
    m_widget->setWindowTitle(renderPosition().first());
    painter->setRenderHint(QPainter::Antialiasing, true);
    GeoDataCoordinates home(latitude,longitude , 0.0, GeoDataCoordinates::Degree);

    QImage marker;
    marker.load("marker_active.png");
    QTransform myTransform;

    myTransform.rotate(course);

    marker=marker.transformed(myTransform);
    painter->drawImage(home, marker);

    return true;
}
