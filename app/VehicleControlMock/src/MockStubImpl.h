#ifndef MOCKSTUBIMPL_H
#define MOCKSTUBIMPL_H

#include <QObject>
#include <QTimer>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/vehiclecontrol/VehicleControlStubDefault.hpp>

using namespace v1::vehiclecontrol;

/**
 * @brief Mock VehicleControl service for testing all IVI apps
 * 
 * Simulates:
 * - Battery level: 100% → 10% (decreases with distance)
 * - Gear: R (Reverse) for PDC testing
 * - Speed: 0 km/h (stationary)
 * - Distance: 60cm → 5cm (cyclic for PDC)
 * 
 * Broadcasts:
 * - vehicleStateChanged(gear, speed, battery, timestamp)
 * - gearDistanceChanged(newGear, oldGear, distance, timestamp)
 */
class MockStubImpl : public QObject, public VehicleControlStubDefault
{
    Q_OBJECT

public:
    explicit MockStubImpl(QObject *parent = nullptr);
    virtual ~MockStubImpl();

    // Override RPC method from FIDL
    virtual void setGearPosition(const std::shared_ptr<CommonAPI::ClientId> _client,
                                 std::string _gear,
                                 setGearPositionReply_t _reply) override;

    // Start/stop simulation
    void startSimulation();
    void stopSimulation();

private slots:
    void updateSimulation();

private:
    QTimer* m_updateTimer;
    
    // Current state
    QString m_currentGear;
    uint16_t m_currentSpeed;
    uint8_t m_currentBattery;
    uint16_t m_currentDistance;
    
    // Simulation parameters
    static const int START_DISTANCE = 60;   // Start at 60cm
    static const int END_DISTANCE = 5;      // End at 5cm
    static const int UPDATE_INTERVAL = 500; // Update every 500ms
    static const int DISTANCE_STEP = 2;     // Decrease 2cm per update
};

#endif // MOCKSTUBIMPL_H
