#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QFont>
#include <QDebug>
#include "vehiclecontrolclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qDebug() << "═══════════════════════════════════════════════════════";
    qDebug() << "BatteryMeter_app Starting (Qt Widgets - MOCK TEST)";
    qDebug() << "═══════════════════════════════════════════════════════";

    VehicleControlClient vehicleClient;

    // Create main widget
    QWidget window;
    window.setWindowTitle("Battery Meter - Test Build");
    window.resize(480, 800);
    
    // Set dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(10, 14, 39));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(31, 31, 31));
    darkPalette.setColor(QPalette::Text, Qt::white);
    window.setPalette(darkPalette);

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.setSpacing(20);

    // Title
    QLabel title("BATTERY METER");
    QFont titleFont;
    titleFont.setPixelSize(24);
    titleFont.setBold(true);
    title.setFont(titleFont);
    title.setAlignment(Qt::AlignCenter);
    title.setStyleSheet("color: #FFD700;");
    layout.addWidget(&title);

    // Battery percentage label
    QLabel batteryLabel;
    batteryLabel.setText("100%");
    batteryLabel.setAlignment(Qt::AlignCenter);
    QFont labelFont;
    labelFont.setPixelSize(20);
    batteryLabel.setFont(labelFont);
    batteryLabel.setStyleSheet("color: white;");
    layout.addWidget(&batteryLabel);

    // Battery progress bar
    QProgressBar batteryBar;
    batteryBar.setRange(0, 100);
    batteryBar.setValue(100);
    batteryBar.setMinimumHeight(60);
    batteryBar.setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #FFD700;"
        "   border-radius: 8px;"
        "   background-color: #1f1f1f;"
        "   text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #FFD700;"
        "   border-radius: 6px;"
        "}"
    );
    layout.addWidget(&batteryBar);

    // Speed label
    QLabel speedLabel;
    speedLabel.setText("Speed: 0 km/h");
    speedLabel.setAlignment(Qt::AlignCenter);
    speedLabel.setFont(labelFont);
    speedLabel.setStyleSheet("color: white;");
    layout.addWidget(&speedLabel);

    // Gear label
    QLabel gearLabel;
    gearLabel.setText("Gear: P");
    gearLabel.setAlignment(Qt::AlignCenter);
    gearLabel.setFont(labelFont);
    gearLabel.setStyleSheet("color: #FFD700;");
    layout.addWidget(&gearLabel);

    // Info label
    QLabel infoLabel;
    infoLabel.setText("(Running in MOCK mode - test build)");
    infoLabel.setAlignment(Qt::AlignCenter);
    infoLabel.setStyleSheet("color: #999; font-size: 10px;");
    layout.addWidget(&infoLabel);

    window.setLayout(&layout);
    window.show();

    // Connect signals to update UI
    QObject::connect(&vehicleClient, &VehicleControlClient::batteryLevelChanged, [&]() {
        int level = vehicleClient.batteryLevel();
        batteryBar.setValue(level);
        batteryLabel.setText(QString::number(level) + "%");
    });

    QObject::connect(&vehicleClient, &VehicleControlClient::speedChanged, [&]() {
        int speed = vehicleClient.speed();
        speedLabel.setText("Speed: " + QString::number(speed) + " km/h");
    });

    QObject::connect(&vehicleClient, &VehicleControlClient::gearStateChanged, [&]() {
        int gear = vehicleClient.gearState();
        QString gearStr;
        switch(gear) {
            case 0: gearStr = "P"; break;
            case 1: gearStr = "R"; break;
            case 2: gearStr = "N"; break;
            case 3: gearStr = "D"; break;
            case 4: gearStr = "S"; break;
            default: gearStr = "?";
        }
        gearLabel.setText("Gear: " + gearStr);
    });

    return app.exec();
}
