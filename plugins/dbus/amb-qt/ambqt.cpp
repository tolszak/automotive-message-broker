#include "ambqt.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QtDebug>
#include <QDBusReply>


const char *ambDBusInterfaceName = "org.automotive.message.broker";
const char *ambDBusObjectPath = "/";
const char *ambManagerDBusInterfaceName = "org.automotive.Manager";

AmbProperty::AmbProperty(QString op, QString iface, QString propName)
    :QObject(), mPropertyName(propName),mInterfaceName(iface), mObjectPath(op),mZone(0)
{
    //connect();
}


void AmbProperty::propertyChangedSlot(QString, QVariantMap values, QStringList )
{
    qDebug() << values;
    mTime = values.value("Time").toDouble();
    if (mValue == values.value(mPropertyName))
        return;

    mValue = values.value(mPropertyName);
    emit valueChanged(mValue);
}

//void AmbProperty::propertyChanged1(QDBusVariant val, double t)
//{
//    qDebug() << "val:" << val.variant() << "t:" << t;
//	mTime = t;
//	mValue = val.variant();

//	signalChanged(mValue);
//}

void AmbProperty::connect()
{

//    if(mDBusInterface)
//    {
//        delete mDBusInterface;
//    }

	if(mObjectPath.isEmpty())
		getObjectPath();

    if(mInterfaceName.isEmpty())
        mInterfaceName = "org.automotive."+mPropertyName;
//    qDebug() << "creating interface";
//	mDBusInterface = new QDBusInterface("org.automotive.message.broker",objectPath(), interfaceName(), QDBusConnection::systemBus(),this);
//    qDebug()<<"Path: "<<objectPath();
//    qDebug()<<"Interface: "<<interfaceName();
//	if(!mDBusInterface->isValid())
//	{
//		qDebug()<<"Failed to create dbus interface for property "<<propertyName();
//		qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
//    } else
//        qDebug() << "interface valid";

    qDebug() << "connecting properties changed";
    qDebug()<<"path: "<<objectPath();
    qDebug()<<"interface: "<<interfaceName();

	if(!QDBusConnection::systemBus().connect("org.automotive.message.broker", objectPath(), "org.freedesktop.DBus.Properties",
                                             "PropertiesChanged", this, SLOT(propertyChangedSlot(QString,QVariantMap,QStringList))))
	{
		qDebug()<<"Failed to connect to signal";
		qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
    } else
        qDebug() << "properties changed valid";

    ///TODO: only use PropertiesChanged...  treat AmbProperty like an object rather than a representation of just a single property in the object
//    qDebug() << "connecting system bus";
//    qDebug()<<"path: "<<objectPath();
//    qDebug()<<"interface: "<<interfaceName();
//    qDebug()<<"signal: "<<propertyName();
//	if(!QDBusConnection::systemBus().connect("org.automotive.message.broker", objectPath(), mInterfaceName,
//											 signalName, this, SLOT(propertyChanged1(QDBusVariant,double))))
//	{
//		qDebug()<<"Failed to connect to signal";
//		qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
//    } else
//        qDebug() << "system bus valid";
}

void AmbProperty::getObjectPath()
{

    QDBusInterface managerIface(ambDBusInterfaceName,
                                ambDBusObjectPath,
                                ambManagerDBusInterfaceName,
                                QDBusConnection::systemBus(), this);

	if(!managerIface.isValid())
	{
        qDebug()<<"Failed to create manager interface:" << managerIface.lastError().message();
		return;
	}
    qDebug() << "finding object object for property:" << mPropertyName << "and Path:" << mZone;
	QDBusReply<QDBusObjectPath> reply = managerIface.call("FindObjectForZone", mPropertyName, mZone);

	if(reply.isValid())
	{
		mObjectPath = reply.value().path();
	}
}
