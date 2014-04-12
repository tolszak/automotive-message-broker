#include "ambvehiclespeed.h"
int AmbVehicleSpeed::value() const
{
    return m_value;
}

void AmbVehicleSpeed::setValue(int arg)
{
    if (m_value != arg) {
        m_value = arg;
        emit valueChanged(arg);
    }
}
