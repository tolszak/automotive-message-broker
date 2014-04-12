#ifndef AMBQT_H
#define AMBQT_H

#include <QObject>
#include <QDBusInterface>
#include <QDebug>

class AmbProperty: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString propertyName READ propertyName WRITE setPropertyName NOTIFY propertyNameChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int zone READ zone WRITE setZone NOTIFY zoneChanged)
    Q_PROPERTY(double time READ time NOTIFY timeChanged)

public:
    AmbProperty(QObject * parent = 0);
    QString propertyName() const;
    QVariant value() const;
    int zone() const;
    double time() const;

Q_SIGNALS:
    void propertyNameChanged(QString arg);
    void valueChanged(QVariant arg);
    void zoneChanged(int arg);
    void timeChanged(double arg);

public Q_SLOTS:
    void setPropertyName(QString arg);
    void setValue(QVariant arg);
    void setZone(int arg);

private Q_SLOTS:
    void propertyChangedSlot(QString, QVariantMap values, QStringList);

private:
    void reconnect();
    double m_time;
    QVariant m_value;
    QString m_propertyName;
    QString m_interfaceName;
    QString m_objectPath;
    int m_zone;
};

#endif // AMBQT_H
