#ifndef VEHICLECONTROLCLIENT_H
#define VEHICLECONTROLCLIENT_H

#include <QObject>
#include <QTimer>
#include <QString>

// Temporary mock version until VehicleControlECU is ready
class VehicleControlClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(bool serviceAvailable READ serviceAvailable NOTIFY serviceAvailableChanged)

public:
    explicit VehicleControlClient(QObject *parent = nullptr);
    ~VehicleControlClient();

    int batteryLevel() const { return m_batteryLevel; }
    bool serviceAvailable() const { return m_serviceAvailable; }

signals:
    void batteryLevelChanged(int level);
    void serviceAvailableChanged(bool available);

private slots:
    void simulateBatteryUpdate();

private:
    int m_batteryLevel;
    bool m_serviceAvailable;
    QTimer m_mockTimer;
};

#endif // VEHICLECONTROLCLIENT_H
