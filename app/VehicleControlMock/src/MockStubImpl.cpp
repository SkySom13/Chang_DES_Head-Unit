#include "MockStubImpl.h"
#include <QDebug>
#include <QDateTime>

MockStubImpl::MockStubImpl(QObject *parent)
    : QObject(parent)
    , m_updateTimer(new QTimer(this))
    , m_currentGear("P")
    , m_currentSpeed(0)
    , m_currentBattery(100)
    , m_currentDistance(START_DISTANCE)
{
    connect(m_updateTimer, &QTimer::timeout, this, &MockStubImpl::updateSimulation);
    qDebug() << "[Mock] ✓ MockStubImpl created";
}

MockStubImpl::~MockStubImpl()
{
    stopSimulation();
    qDebug() << "[Mock] MockStubImpl destroyed";
}

void MockStubImpl::setGearPosition(const std::shared_ptr<CommonAPI::ClientId> _client,
                                   std::string _gear,
                                   setGearPositionReply_t _reply)
{
    Q_UNUSED(_client);
    QString gear = QString::fromStdString(_gear);
    
    qDebug() << "";
    qDebug() << "[Mock] 📞 RPC Call: setGearPosition(" << gear << ")";

    // Validate gear
    if (gear != "P" && gear != "R" && gear != "N" && gear != "D") {
        qWarning() << "[Mock] ❌ Invalid gear:" << gear;
        _reply(false);
        return;
    }

    QString oldGear = m_currentGear;
    m_currentGear = gear;

    // Broadcast gear change event
    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();
    fireGearDistanceChangedEvent(m_currentGear.toStdString(),
                                  oldGear.toStdString(),
                                  m_currentDistance,
                                  timestamp);

    qDebug() << "[Mock] ✓ Gear changed:" << oldGear << "→" << m_currentGear;
    qDebug() << "[Mock] 📡 Broadcast: gearDistanceChanged";

    // Start/stop simulation based on gear
    if (gear == "R" && !m_updateTimer->isActive()) {
        qDebug() << "[Mock] 🚗 Reverse gear engaged - starting PDC simulation";
        m_currentDistance = START_DISTANCE;
        m_currentBattery = 100;
        m_updateTimer->start(UPDATE_INTERVAL);
    } else if (oldGear == "R" && gear != "R") {
        qDebug() << "[Mock] 🛑 Reverse gear disengaged - stopping PDC simulation";
        m_updateTimer->stop();
        m_currentDistance = START_DISTANCE;
    }

    _reply(true);
}

void MockStubImpl::startSimulation()
{
    qDebug() << "";
    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "[Mock] 🚀 Starting Continuous Simulation";
    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "";
    qDebug() << "[Mock] Configuration:";
    qDebug() << "       • Gear: R (Reverse)";
    qDebug() << "       • Speed: 0 km/h";
    qDebug() << "       • Battery: 100% → 10%";
    qDebug() << "       • Distance: 60cm → 5cm";
    qDebug() << "       • Update interval: 500ms";
    qDebug() << "";

    // Initialize state
    m_currentGear = "R";
    m_currentSpeed = 0;
    m_currentBattery = 100;
    m_currentDistance = START_DISTANCE;

    // Broadcast initial state
    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();
    
    fireGearDistanceChangedEvent(m_currentGear.toStdString(),
                                  "P",  // old gear
                                  m_currentDistance,
                                  timestamp);

    fireVehicleStateChangedEvent(m_currentGear.toStdString(),
                                  m_currentSpeed,
                                  m_currentBattery,
                                  timestamp);

    qDebug() << "[Mock] 📡 Initial broadcast sent";
    qDebug() << "       Gear: R | Speed: 0 | Battery: 100% | Distance: 60cm";
    qDebug() << "";
    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "";

    // Start periodic updates
    m_updateTimer->start(UPDATE_INTERVAL);
}

void MockStubImpl::stopSimulation()
{
    if (m_updateTimer->isActive()) {
        m_updateTimer->stop();
        qDebug() << "[Mock] 🛑 Simulation stopped";
    }
}

void MockStubImpl::updateSimulation()
{
    // Decrease distance
    m_currentDistance -= DISTANCE_STEP;

    // Calculate battery level (decreases as distance decreases)
    // 100% at 60cm → 10% at 5cm
    int batteryRange = 90;  // 100% - 10%
    int distanceRange = START_DISTANCE - END_DISTANCE;  // 60 - 5 = 55cm
    m_currentBattery = 100 - ((START_DISTANCE - m_currentDistance) * batteryRange / distanceRange);
    
    // Clamp battery to valid range
    if (m_currentBattery > 100) m_currentBattery = 100;
    if (m_currentBattery < 10) m_currentBattery = 10;

    // Check if simulation complete
    if (m_currentDistance <= END_DISTANCE) {
        m_currentDistance = END_DISTANCE;
        m_currentBattery = 10;
        m_updateTimer->stop();

        qDebug() << "";
        qDebug() << "═══════════════════════════════════════════════════════";
        qDebug() << "[Mock] ✅ Simulation cycle complete!";
        qDebug() << "       Final: Distance = 5cm, Battery = 10%";
        qDebug() << "       🔄 Restarting in 3 seconds...";
        qDebug() << "═══════════════════════════════════════════════════════";
        qDebug() << "";

        // Restart after delay
        QTimer::singleShot(3000, this, [this]() {
            qDebug() << "[Mock] 🔄 Restarting simulation cycle";
            m_currentDistance = START_DISTANCE;
            m_currentBattery = 100;
            m_updateTimer->start(UPDATE_INTERVAL);
        });
        return;
    }

    // Determine distance zone
    QString zone;
    if (m_currentDistance > 50) {
        zone = "🟢 SAFE";
    } else if (m_currentDistance > 30) {
        zone = "🟡 CAUTION";
    } else if (m_currentDistance > 15) {
        zone = "🟠 WARNING";
    } else {
        zone = "🔴 DANGER";
    }

    // Get timestamp
    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();

    // Broadcast events
    fireGearDistanceChangedEvent(m_currentGear.toStdString(),
                                  m_currentGear.toStdString(),
                                  m_currentDistance,
                                  timestamp);

    fireVehicleStateChangedEvent(m_currentGear.toStdString(),
                                  m_currentSpeed,
                                  m_currentBattery,
                                  timestamp);

    // Log update
    qDebug() << QString("[Mock] 📡 Distance: %1cm %2 | Battery: %3% | Gear: %4 | Speed: %5")
                .arg(m_currentDistance, 2)
                .arg(zone)
                .arg(m_currentBattery, 3)
                .arg(m_currentGear)
                .arg(m_currentSpeed);
}
