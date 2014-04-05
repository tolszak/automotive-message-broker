#include <QDebug>
#include <QtQml/QtQml>

#include "ambqtquick.h"
#include "ambqt.h"

void AmbQtQuickPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("amb"));
    Q_UNUSED(uri);
    qmlRegisterType<AmbProperty>(uri,0,1,"AutomotivePropertyItem");
}


