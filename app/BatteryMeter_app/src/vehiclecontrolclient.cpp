#include "vehiclecontrolclient.h"
#include <QDebug>

VehicleControlClient::VehicleControlClient(QObject *parent)
    : QObject(parent)
    , m_batteryLevel(75)
    , m_serviceAvailable(false)
{
    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "VehicleControlClient (BatteryMeter) - TEMPORARY MOCK MODE";
    qDebug() << "TODO: Replace with vsomeip when VehicleControlECU is ready";
    qDebug() << "═══════════════════════════════════════════════════════";
    
    // Temporary mock timer - simulates battery updates
    connect(&m_mockTimer, &QTimer::timeout, this, &VehicleControlClient::simulateBatteryUpdate);
    m_mockTimer.start(2000);  // Update every 2 seconds
    
    // Simulate service available after 1 second
    QTimer::singleShot(1000, this, [this]() {
        m_serviceAvailable = true;
        emit serviceAvailableChanged(true);
        qDebug() << "✅ Mock service now 'available'";
    });
}

VehicleControlClient::~VehicleControlClient()
{
    m_mockTimer.stop();
    qDebug() << "VehicleControlClient destroyed";
}

void VehicleControlClient::simulateBatteryUpdate()
{
    // Simulate battery drain
    m_batteryLevel--;
    if (m_batteryLevel < 0) {
        m_batteryLevel = 100;  // Reset
    }
    
    emit batteryLevelChanged(m_batteryLevel);
    
    // Log every 5 updates
    static int count = 0;
    if (++count % 5 == 0) {
        qDebug() << "📡 [MOCK Battery] Level:" << m_batteryLevel << "%";
    }
}
