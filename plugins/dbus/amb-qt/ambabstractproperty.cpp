#include "ambabstractproperty.h"

AmbAbstractProperty::AmbAbstractProperty(QObject *parent): QObject(parent), m_enabled(true)
{

}

AmbAbstractProperty::~AmbAbstractProperty()
{

}

QString AmbAbstractProperty::propertyName() const
{
    return m_propertyName;
}

bool AmbAbstractProperty::enabled() const
{
    return m_enabled;
}

AmbZone::Zones AmbAbstractProperty::zones() const
{
    return m_zones;
}

bool AmbAbstractProperty::isValid() const
{
    return m_isValid;
}

void AmbAbstractProperty::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        emit enabledChanged(enabled);
    }
}

void AmbAbstractProperty::setZones(AmbZone::Zones zones)
{
    if (m_zones != zones) {
        m_zones = zones;
        emit zonesChanged(zones);
    }
}

void AmbAbstractProperty::setIsValid(bool isValid)
{
    if (m_isValid != isValid) {
        m_isValid = isValid;
        emit isValidChanged(isValid);
    }
}

void AmbAbstractProperty::setPropertyName(QString propertyName)
{
    if (m_propertyName != propertyName) {
        m_propertyName = propertyName;
        emit propertyNameChanged(propertyName);
    }
}

void AmbAbstractProperty::connect()
{

}

void AmbAbstractProperty::disconnect()
{

}
