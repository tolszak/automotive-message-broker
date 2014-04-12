#ifndef AMB_ABSTRACT_PROPERTY_H
#define AMB_ABSTRACT_PROPERTY_H
#include <QObject>
#include "abstractpropertytype.h"

class AmbZone: public QObject {
    Q_OBJECT
public:
    enum ZoneFlag {
        None =   Zone::None,
        Front = Zone::Front,
        Middle = Zone::Middle,
        Right = Zone::Right,
        Left = Zone::Left,
        Rear = Zone::Rear,
        Center = Zone::Center,
        LeftSide = Zone::LeftSide,
        RightSide = Zone::RightSide,
        FrontSide = Zone::FrontSide,
        BackSide = Zone::BackSide,
        FronRight = Zone::Front | Zone::Right,
        FrontLeft = Zone::Front | Zone::Left,
        MiddleRight = Zone::Middle | Zone::Right,
        MiddleLeft = Zone::Middle | Zone::Left,
        RearRight = Zone::Rear | Zone::Right,
        RearLeft = Zone::Rear | Zone::Left
    };
    Q_DECLARE_FLAGS(Zones, ZoneFlag)
};


class AmbAbstractProperty: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString propertyName READ propertyName NOTIFY propertyNameChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(AmbZone::Zones zones READ zones WRITE setZones NOTIFY zonesChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
public:
    AmbAbstractProperty(QObject *parent);
    virtual ~AmbAbstractProperty();

    QString propertyName() const;
    bool enabled() const;
    AmbZone::Zones zones() const;
    bool isValid() const;

public slots:
    virtual void setEnabled(bool enabled);
    void setZones(AmbZone::Zones zones);
signals:
    void propertyNameChanged(QString propertyName);
    void enabledChanged(bool enabled);
    void zonesChanged(AmbZone::Zones zones);
    void isValidChanged(bool isValid);
protected slots:
    void setPropertyName(QString propertyName);
    void setIsValid(bool isValid);
    void connect();
    void disconnect();
private:
    QString m_propertyName;
    bool m_enabled;
    AmbZone::Zones m_zones;
    bool m_isValid;
};











#endif //AMB_ABSTRACT_PROPERTY_H
