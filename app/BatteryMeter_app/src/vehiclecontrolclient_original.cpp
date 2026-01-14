#include "vehiclecontrolclient.h"
#include <QDebug>

VehicleControlClient::VehicleControlClient(QObject *parent)
    : QObject(parent), 
      gearState_(0), 
      speed_(0), 
      batteryLevel_(0),
      isConnected_(false)
{
    setupProxy();

    // Setup update timer (poll every 100ms)
    connect(&updateTimer_, &QTimer::timeout, this, &VehicleControlClient::updateValues);
    updateTimer_.start(100);
}

VehicleControlClient::~VehicleControlClient()
{
    updateTimer_.stop();
}

void VehicleControlClient::setupProxy()
{
    try {
        auto runtime = CommonAPI::Runtime::get();
        proxy_ = runtime->buildProxy<VehicleControlProxy>("local", "vehiclecontrol");
        
        if (!proxy_) {
            qWarning() << "Failed to build VehicleControlProxy";
            onConnectionFailed();
            return;
        }
        
        proxy_->getProxyStatusEvent().subscribe([this](const CommonAPI::AvailabilityStatus& status) {
            if (status == CommonAPI::AvailabilityStatus::AVAILABLE) {
                qDebug() << "VehicleControl service is AVAILABLE";
                isConnected_ = true;
                emit connectionStatusChanged(true);
                onConnectionEstablished();
            } else {
                qDebug() << "VehicleControl service is NOT AVAILABLE";
                isConnected_ = false;
                emit connectionStatusChanged(false);
                onConnectionFailed();
            }
        });
        
    } catch (const std::exception& e) {
        qWarning() << "Exception in setupProxy:" << e.what();
        onConnectionFailed();
    }
}

void VehicleControlClient::onConnectionEstablished()
{
    qDebug() << "VehicleControl connection established";
}

void VehicleControlClient::onConnectionFailed()
{
    qDebug() << "VehicleControl connection failed";
}

void VehicleControlClient::updateValues()
{
    if (!proxy_ || !isConnected_) {
        return;
    }

    try {
        // Example: Fetch gear state, speed, and battery level from the service
        // This is a placeholder - adapt to your actual CommonAPI service methods
        
        // Call remote methods (adapt these to your actual service interface)
        CommonAPI::CallStatus callStatus;
        
        // Pseudo-code - replace with actual service methods:
        // uint32_t gear = proxy_->getGearState(callStatus);
        // uint32_t spd = proxy_->getSpeed(callStatus);
        // uint32_t battery = proxy_->getBatteryLevel(callStatus);
        
    } catch (const std::exception& e) {
        qWarning() << "Exception in updateValues:" << e.what();
    }
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
