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
    qputenv("QT_WAYLAND_SHELL_INTEGRATION", "xdg-shell");

    QGuiApplication app(argc, argv);

    app.setApplicationName("appBatteryMeter");
    app.setApplicationDisplayName("Battery Meter (Test)");
    app.setDesktopFileName("appBatteryMeter");

    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "BatteryMeter_app Starting (MOCK TEST BUILD)";
    qDebug() << "═══════════════════════════════════════════════════════";
    
    QQmlApplicationEngine engine;

    // Connect to QML engine warnings/errors
    QObject::connect(&engine, &QQmlApplicationEngine::warnings, [](const QList<QQmlError> &warnings) {
        for (const QQmlError &warning : warnings) {
            qWarning() << "QML Warning:" << warning.toString();
        }
    });

    engine.addImportPath("qrc:/");

    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/Design/Constants.qml")),
                             "Design", 1, 0, "Constants");

    // Register C++ objects to QML context
    VehicleControlClient vehicleClient;
    engine.rootContext()->setContextProperty("vehicleClient", &vehicleClient);

    // Try resource first, then fallback to file system
    const QUrl url(QStringLiteral("qrc:/DesignContent/App.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load from resources, trying file system...";
        QString appPath = QString::fromLocal8Bit(argv[0]);
        QString basePath = appPath.left(appPath.lastIndexOf("/"));
        QString qmlPath = basePath + "/../qml/DesignContent/App.qml";
        qDebug() << "Attempting to load:" << QUrl::fromLocalFile(qmlPath);
        engine.load(QUrl::fromLocalFile(qmlPath));
        
        if (engine.rootObjects().isEmpty())
            return -1;
    }

    return app.exec();
}
