#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QTimer>
#include <QWindow>
#include <QCoreApplication>
#include <QUrl>
#include <QDebug>
#include "vehiclecontrolclient.h"

int main(int argc, char *argv[])
{
    qDebug() << "BatteryMeter_app Starting...";
    QGuiApplication app(argc, argv);

    // ═══════════════════════════════════════════════════════
    // IVI SHELL: Set application name for display routing
    // ═══════════════════════════════════════════════════════
    // This name will be routed by ivi-shell
    app.setApplicationName("appBatteryMeter");  // ← IVI Shell routing
    app.setApplicationDisplayName("Battery Meter");
    app.setDesktopFileName("appBatteryMeter");

    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "BatteryMeter_app Starting (IVI Shell - Weston)";
    qDebug() << "App ID: appBatteryMeter";
    qDebug() << "Mock Mode: Using simulated vehicle data";
    qDebug() << "═══════════════════════════════════════════════════════";
    
    QQmlApplicationEngine engine;

    // Connect to QML engine warnings/errors
    QObject::connect(&engine, &QQmlApplicationEngine::warnings, [](const QList<QQmlError> &warnings) {
        for (const QQmlError &warning : warnings) {
            Q_UNUSED(warning);
        }
    });

    engine.addImportPath("qrc:/");
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/../qml");

    // ═══════════════════════════════════════════════════════
    // Register C++ objects to QML context
    // ═══════════════════════════════════════════════════════
    // VehicleControlClient (vsomeip - Battery Level)
    VehicleControlClient vehicleClient;
    engine.rootContext()->setContextProperty("vehicleClient", &vehicleClient);

    const QUrl url(QStringLiteral("qrc:/DesignContent/App.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML from resources, trying file system...";
        // Fallback to file system
        QString qmlPath = QCoreApplication::applicationDirPath() + "/../qml/DesignContent/App.qml";
        QUrl fileUrl = QUrl::fromLocalFile(qmlPath);
        qDebug() << "Attempting to load from:" << fileUrl;
        engine.load(fileUrl);
        
        if (engine.rootObjects().isEmpty()) {
            qCritical() << "Failed to load QML from both resources and file system!";
            return -1;
        }
    }

    return app.exec();
}
