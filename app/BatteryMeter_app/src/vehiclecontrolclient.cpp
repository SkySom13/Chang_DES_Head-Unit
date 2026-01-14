#include "vehiclecontrolclient.h"
#include <QDebug>
#include <QRandomGenerator>

VehicleControlClient::VehicleControlClient(QObject *parent)
    : QObject(parent), 
      gearState_(0), 
      speed_(0), 
      batteryLevel_(100),
      isConnected_(true)
{
    qDebug() << "VehicleControlClient initialized (MOCK MODE)";
    
    // Setup update timer (simulate values every 500ms)
    connect(&updateTimer_, &QTimer::timeout, this, &VehicleControlClient::updateValues);
    updateTimer_.start(500);
    
    emit connectionStatusChanged(true);
}

VehicleControlClient::~VehicleControlClient()
{
    updateTimer_.stop();
}

void VehicleControlClient::updateValues()
{
    // Simulate battery level (slowly draining)
    if (batteryLevel_ > 0) {
        batteryLevel_ -= 1;
    }
    
    // Simulate speed (random between 0-100)
    speed_ = QRandomGenerator::global()->bounded(0, 101);
    
    // Simulate gear state (random between 0-4: P,R,N,D,S)
    gearState_ = QRandomGenerator::global()->bounded(0, 5);
    
    emit batteryLevelChanged();
    emit speedChanged();
    emit gearStateChanged();
}

int VehicleControlClient::gearState() const
{
    return gearState_;
}

int VehicleControlClient::speed() const
{
    return speed_;
}

int VehicleControlClient::batteryLevel() const
{
    return batteryLevel_;
}
