#ifndef AMB_VEHICLE_SPEED_H
#define AMB_VEHICLE_SPEED_H
#include "ambabstractproperty.h"

class AmbVehicleSpeed: public AmbAbstractProperty
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    int m_value;

public:

    int value() const;
public slots:
    void setValue(int arg);
signals:
    void valueChanged(int arg);
};








#endif //AMB_VEHICLE_SPEED_H
