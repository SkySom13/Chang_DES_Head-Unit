#ifndef VEHICLECONTROLCLIENT_H
#define VEHICLECONTROLCLIENT_H

#include <QObject>
#include <QTimer>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/vehiclecontrol/VehicleControlProxy.hpp>

using namespace v1::vehiclecontrol;

class VehicleControlClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int gearState READ gearState NOTIFY gearStateChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)

public:
    explicit VehicleControlClient(QObject *parent = nullptr);
    ~VehicleControlClient();

    // Property getters
    int gearState() const;
    int speed() const;
    int batteryLevel() const;

signals:
    void gearStateChanged();
    void speedChanged();
    void batteryLevelChanged();
    void connectionStatusChanged(bool connected);

private slots:
    void onConnectionEstablished();
    void onConnectionFailed();
    void updateValues();

private:
    std::shared_ptr<VehicleControlProxyDefault> proxy_;
    QTimer updateTimer_;
    
    int gearState_;
    int speed_;
    int batteryLevel_;
    bool isConnected_;

    void setupProxy();
};

#endif // VEHICLECONTROLCLIENT_H
