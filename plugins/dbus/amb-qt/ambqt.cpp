#include "ambqt.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QtDebug>
#include <QDBusReply>


const char *ambDBusInterfaceName = "org.automotive.message.broker";
const char *ambDBusRootPath = "/";
const char *ambManagerDBusInterfaceName = "org.automotive.Manager";

AmbProperty::AmbProperty(QObject *parent):
    QObject(parent), m_time(0), m_zone(0)
{

}

void AmbProperty::propertyChangedSlot(QString, QVariantMap values, QStringList )
{
    qDebug() << values;
    m_time = values.value("Time").toDouble();
    if (m_value == values.value(m_propertyName))
        return;

    m_value = values.value(m_propertyName);
    emit valueChanged(m_value);
}

QString AmbProperty::propertyName() const
{
    return m_propertyName;
}

QVariant AmbProperty::value() const
{
    return m_value;
}
double AmbProperty::time() const
{
    return m_time;
}

//QString AmbProperty::interfaceName() const
//{
//    return m_interfaceName;
//}

//QString AmbProperty::objectPath() const
//{
//    return m_objectPath;
//}

int AmbProperty::zone() const
{
    return m_zone;
}

void AmbProperty::setPropertyName(QString arg)
{
    if (m_propertyName != arg) {
        m_propertyName = arg;
        reconnect();
        emit propertyNameChanged(arg);
    }
}

void AmbProperty::setValue(QVariant /*arg*/)
{
    qWarning() << "NOT IMPLEMENTED";
}

//void AmbProperty::setInterfaceName(QString arg)
//{
//    if (m_interfaceName != arg) {
//        m_interfaceName = arg;
//        emit interfaceNameChanged(arg);
//    }
//}

//void AmbProperty::setObjectPath(QString arg)
//{
//    if (m_objectPath != arg) {
//        m_objectPath = arg;
//        emit objectPathChanged(arg);
//    }
//}

void AmbProperty::setZone(int arg)
{
    if (m_zone != arg) {
        m_zone = arg;
        emit zoneChanged(arg);
        reconnect();
    }
}

void AmbProperty::reconnect()
{
    if (!m_objectPath.isEmpty()) {
        if (!QDBusConnection::systemBus().disconnect(ambDBusInterfaceName, m_objectPath, "org.freedesktop.DBus.Properties",
                                                     "PropertiesChanged", this, SLOT(propertyChangedSlot(QString,QVariantMap,QStringList)))) {
            qDebug()<<"Failed to disconnect propertyChanged signal for object:" <<m_objectPath;
            qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
        }
        m_objectPath.clear();
    }

    QDBusInterface managerIface(ambDBusInterfaceName,
                                ambDBusRootPath,
                                ambManagerDBusInterfaceName,
                                QDBusConnection::systemBus(), this);

    if(!managerIface.isValid())
    {
        qDebug()<<"Failed to create manager interface:" << managerIface.lastError().message();
        return;
    }

    QDBusReply<QDBusObjectPath> reply = managerIface.call("FindObjectForZone", m_propertyName, m_zone);

    if(reply.isValid())
    {
        m_objectPath = reply.value().path();
    }

    if(!QDBusConnection::systemBus().connect(ambDBusInterfaceName, m_objectPath, "org.freedesktop.DBus.Properties",
                                             "PropertiesChanged", this, SLOT(propertyChangedSlot(QString,QVariantMap,QStringList))))
    {
        qDebug()<<"Failed to connect propertyChange signal for object path:" << m_objectPath;
        qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
    }
}
