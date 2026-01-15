#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <CommonAPI/CommonAPI.hpp>
#include "MockStubImpl.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("VehicleControlMock");
    app.setApplicationVersion("1.0");

    // ═══════════════════════════════════════════════════════════
    // Environment Configuration
    // ═══════════════════════════════════════════════════════════
    if (qgetenv("VSOMEIP_APPLICATION_NAME").isEmpty()) {
        qputenv("VSOMEIP_APPLICATION_NAME", "VehicleControlMock");
    }
    
    if (qgetenv("VSOMEIP_CONFIGURATION").isEmpty()) {
        QString configPath = QCoreApplication::applicationDirPath() + "/../config/vsomeip_mock.json";
        qputenv("VSOMEIP_CONFIGURATION", configPath.toLocal8Bit());
    }
    
    if (qgetenv("COMMONAPI_CONFIG").isEmpty()) {
        QString commonApiConfig = QCoreApplication::applicationDirPath() + "/../../../commonapi/commonapi.ini";
        qputenv("COMMONAPI_CONFIG", commonApiConfig.toLocal8Bit());
    }

    qDebug() << "";
    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "  VehicleControlMock - vsomeip Service";
    qDebug() << "  Version: 1.0";
    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "";
    qDebug() << "Environment:";
    qDebug() << "  VSOMEIP_APPLICATION_NAME:" << qgetenv("VSOMEIP_APPLICATION_NAME");
    qDebug() << "  VSOMEIP_CONFIGURATION:" << qgetenv("VSOMEIP_CONFIGURATION");
    qDebug() << "  COMMONAPI_CONFIG:" << qgetenv("COMMONAPI_CONFIG");
    qDebug() << "";

    // ═══════════════════════════════════════════════════════════
    // CommonAPI Service Registration
    // ═══════════════════════════════════════════════════════════
    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
    if (!runtime) {
        qCritical() << "❌ Failed to get CommonAPI runtime!";
        return -1;
    }

    qDebug() << "✓ CommonAPI runtime initialized";

    // Create service implementation
    std::shared_ptr<MockStubImpl> mockStub = std::make_shared<MockStubImpl>();
    
    // Register service
    const std::string domain = "local";
    const std::string instance = "vehiclecontrol.VehicleControl";
    const std::string connection = "VehicleControlMock";
    
    bool registered = runtime->registerService(domain, instance, mockStub, connection);
    
    if (!registered) {
        qCritical() << "❌ Failed to register VehicleControl service!";
        return -1;
    }

    qDebug() << "✓ Service registered successfully";
    qDebug() << "  Domain:" << QString::fromStdString(domain);
    qDebug() << "  Instance:" << QString::fromStdString(instance);
    qDebug() << "";

    // ═══════════════════════════════════════════════════════════
    // Start Simulation
    // ═══════════════════════════════════════════════════════════
    qDebug() << "Press Ctrl+C to stop the service";
    qDebug() << "";

    // Start continuous simulation
    mockStub->startSimulation();

    return app.exec();
}
