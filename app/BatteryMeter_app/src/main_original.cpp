#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QTimer>
#include <QWindow>
#include "vehiclecontrolclient.h"

int main(int argc, char *argv[])
{
    // Set app_id BEFORE creating QGuiApplication
    // This ensures Qt Wayland uses it from the start
    qputenv("QT_WAYLAND_SHELL_INTEGRATION", "xdg-shell");

    QGuiApplication app(argc, argv);

    // ═══════════════════════════════════════════════════════
    // KIOSK SHELL: Set application name for display routing
    // ═══════════════════════════════════════════════════════
    // This name must match the app-ids in weston.ini
    app.setApplicationName("appBatteryMeter");  // ← Routes to appropriate Weston output
    app.setApplicationDisplayName("Battery Meter");
    app.setDesktopFileName("appBatteryMeter");  // Critical for Wayland app_id

    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "BatteryMeter_app Starting (Kiosk Shell - Weston)";
    qDebug() << "App ID: appBatteryMeter";
    qDebug() << "═══════════════════════════════════════════════════════";
    
    QQmlApplicationEngine engine;

    // Connect to QML engine warnings/errors
    QObject::connect(&engine, &QQmlApplicationEngine::warnings, [](const QList<QQmlError> &warnings) {
        for (const QQmlError &warning : warnings) {
            Q_UNUSED(warning);
        }
    });

    engine.addImportPath("qrc:/");

    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/Design/Constants.qml")),
                             "Design", 1, 0, "Constants");

    // ═══════════════════════════════════════════════════════
    // Register C++ objects to QML context
    // ═══════════════════════════════════════════════════════
    // VehicleControlClient (vsomeip - Battery Level)
    VehicleControlClient vehicleClient;
    engine.rootContext()->setContextProperty("vehicleClient", &vehicleClient);

    const QUrl url(QStringLiteral("qrc:/DesignContent/App.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
