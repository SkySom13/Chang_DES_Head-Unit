#!/bin/bash

# ═══════════════════════════════════════════════════════
# IC System Startup Script
# ═══════════════════════════════════════════════════════

# Qt environment
export LD_LIBRARY_PATH="${HOME}/Qt/5.15.2/gcc_64/lib:${LD_LIBRARY_PATH}"
export XDG_RUNTIME_DIR="/tmp/runtime-${USER}"

# Create and secure runtime directory
mkdir -p ${XDG_RUNTIME_DIR}
chmod 0700 ${XDG_RUNTIME_DIR}

# Clean up any existing sockets
rm -f ${XDG_RUNTIME_DIR}/wayland-*

# Application paths
BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMPOSITOR_PATH="${BASE_DIR}/IC_Compositor/build/IC_Compositor"
GEARSTATE_PATH="${BASE_DIR}/GearState_app/build/GearState_app"
SPEEDOMETER_PATH="${BASE_DIR}/Speedometer_app/build/Speedometer_app"
BATTERYMETER_PATH="${BASE_DIR}/BatteryMeter_app/build/BatteryMeter_app"

# Kill any existing instances
echo "Cleaning up existing processes..."
pkill -9 -f IC_Compositor 2>/dev/null
pkill -9 -f GearState_app 2>/dev/null
pkill -9 -f Speedometer_app 2>/dev/null
pkill -9 -f BatteryMeter_app 2>/dev/null
sleep 1

echo "═══════════════════════════════════════════════════════"
echo "Starting IC System (XDG Shell)"
echo "Base Dir: ${BASE_DIR}"
echo "Runtime Dir: ${XDG_RUNTIME_DIR}"
echo "═══════════════════════════════════════════════════════"

# ═══════════════════════════════════════════════════════
# Check if compositor exists
# ═══════════════════════════════════════════════════════
if [ ! -f "${COMPOSITOR_PATH}" ]; then
    echo ""
    echo "ERROR: Compositor not found at ${COMPOSITOR_PATH}"
    echo ""
    echo "Please build it first:"
    echo "  cd IC_Compositor/build"
    echo "  cmake --build . -j\$(nproc)"
    echo ""
    exit 1
fi

# ═══════════════════════════════════════════════════════
# Start Compositor (runs on X11)
# ═══════════════════════════════════════════════════════
echo ""
echo "Starting IC Compositor on X11..."

QT_QPA_PLATFORM=xcb \
QT_LOGGING_RULES="qt.qpa.*=false" \
${COMPOSITOR_PATH} > /tmp/ic_compositor.log 2>&1 &

COMPOSITOR_PID=$!
echo "  Compositor PID: ${COMPOSITOR_PID}"
sleep 3

# Check if compositor is running
if ! ps -p ${COMPOSITOR_PID} > /dev/null; then
    echo ""
    echo "ERROR: Compositor failed to start!"
    echo ""
    echo "═══════════════════════════════════════════════════════"
    echo "Compositor Log:"
    echo "═══════════════════════════════════════════════════════"
    cat /tmp/ic_compositor.log
    echo "═══════════════════════════════════════════════════════"
    exit 1
fi

# ═══════════════════════════════════════════════════════
# Wait for Wayland socket
# ═══════════════════════════════════════════════════════
WAYLAND_DISPLAY="wayland-0"
SOCKET_PATH="${XDG_RUNTIME_DIR}/${WAYLAND_DISPLAY}"

echo "  Waiting for Wayland socket at ${SOCKET_PATH}..."
timeout=15
while [ ! -S "${SOCKET_PATH}" ] && [ $timeout -gt 0 ]; do
    sleep 1
    timeout=$((timeout-1))
    if [ $((timeout % 3)) -eq 0 ]; then
        echo "    Still waiting... (${timeout}s remaining)"
    fi
done

if [ ! -S "${SOCKET_PATH}" ]; then
    echo ""
    echo "ERROR: Wayland socket not created after 15 seconds!"
    echo ""
    echo "═══════════════════════════════════════════════════════"
    echo "Compositor Log:"
    echo "═══════════════════════════════════════════════════════"
    cat /tmp/ic_compositor.log
    echo "═══════════════════════════════════════════════════════"
    kill ${COMPOSITOR_PID} 2>/dev/null
    exit 1
fi

echo "  ✓ Compositor ready (socket: ${SOCKET_PATH})"

# ═══════════════════════════════════════════════════════
# Start Client Applications (Wayland clients)
# ═══════════════════════════════════════════════════════
echo ""
echo "Starting client applications..."
echo ""

# GearState App (App ID: appGearState)
if [ -f "${GEARSTATE_PATH}" ]; then
    echo "  → GearState (App ID: appGearState)..."
    
    QT_QPA_PLATFORM=wayland \
    WAYLAND_DISPLAY=${WAYLAND_DISPLAY} \
    QT_WAYLAND_SHELL_INTEGRATION=xdg-shell \
    QT_WAYLAND_DISABLE_WINDOWDECORATION=1 \
    ${GEARSTATE_PATH} > /tmp/ic_gearstate.log 2>&1 &
    
    GEARSTATE_PID=$!
    echo "     PID: ${GEARSTATE_PID}"
    sleep 1.5
else
    echo "  ✗ GearState app not found"
    echo "     Expected: ${GEARSTATE_PATH}"
fi

# Speedometer App (App ID: appSpeedometer)
if [ -f "${SPEEDOMETER_PATH}" ]; then
    echo "  → Speedometer (App ID: appSpeedometer)..."
    
    QT_QPA_PLATFORM=wayland \
    WAYLAND_DISPLAY=${WAYLAND_DISPLAY} \
    QT_WAYLAND_SHELL_INTEGRATION=xdg-shell \
    QT_WAYLAND_DISABLE_WINDOWDECORATION=1 \
    ${SPEEDOMETER_PATH} > /tmp/ic_speedometer.log 2>&1 &
    
    SPEEDOMETER_PID=$!
    echo "     PID: ${SPEEDOMETER_PID}"
    sleep 1.5
else
    echo "  ✗ Speedometer app not found"
    echo "     Expected: ${SPEEDOMETER_PATH}"
fi

# BatteryMeter App (App ID: appBatteryMeter)
if [ -f "${BATTERYMETER_PATH}" ]; then
    echo "  → BatteryMeter (App ID: appBatteryMeter)..."
    
    QT_QPA_PLATFORM=wayland \
    WAYLAND_DISPLAY=${WAYLAND_DISPLAY} \
    QT_WAYLAND_SHELL_INTEGRATION=xdg-shell \
    QT_WAYLAND_DISABLE_WINDOWDECORATION=1 \
    ${BATTERYMETER_PATH} > /tmp/ic_batterymeter.log 2>&1 &
    
    BATTERYMETER_PID=$!
    echo "     PID: ${BATTERYMETER_PID}"
    sleep 1.5
else
    echo "  ✗ BatteryMeter app not found"
    echo "     Expected: ${BATTERYMETER_PATH}"
fi

# ═══════════════════════════════════════════════════════
# System Started
# ═══════════════════════════════════════════════════════
echo ""
echo "═══════════════════════════════════════════════════════"
echo "✓ IC System Started Successfully"
echo "═══════════════════════════════════════════════════════"
echo ""
echo "Environment:"
echo "  QT Path:         ${HOME}/Qt/5.15.2/gcc_64"
echo "  Runtime Dir:     ${XDG_RUNTIME_DIR}"
echo "  Wayland Display: ${WAYLAND_DISPLAY}"
echo ""
echo "Process IDs:"
echo "  Compositor:  ${COMPOSITOR_PID}"
[ -n "${GEARSTATE_PID}" ] && echo "  GearState:   ${GEARSTATE_PID}"
[ -n "${SPEEDOMETER_PID}" ] && echo "  Speedometer: ${SPEEDOMETER_PID}"
[ -n "${BATTERYMETER_PID}" ] && echo "  Battery:     ${BATTERYMETER_PID}"
echo ""
echo "Log files:"
echo "  Compositor:  tail -f /tmp/ic_compositor.log"
[ -f /tmp/ic_gearstate.log ] && echo "  GearState:   tail -f /tmp/ic_gearstate.log"
[ -f /tmp/ic_speedometer.log ] && echo "  Speedometer: tail -f /tmp/ic_speedometer.log"
[ -f /tmp/ic_batterymeter.log ] && echo "  Battery:     tail -f /tmp/ic_batterymeter.log"
echo ""
echo "Press Ctrl+C to stop all applications"
echo "═══════════════════════════════════════════════════════"

# ═══════════════════════════════════════════════════════
# Cleanup Function
# ═══════════════════════════════════════════════════════
cleanup() {
    echo ""
    echo "═══════════════════════════════════════════════════════"
    echo "Stopping IC System..."
    echo "═══════════════════════════════════════════════════════"
    
    # Stop client applications first (graceful)
    [ -n "${BATTERYMETER_PID}" ] && kill ${BATTERYMETER_PID} 2>/dev/null && echo "  ✓ BatteryMeter stopped"
    [ -n "${SPEEDOMETER_PID}" ] && kill ${SPEEDOMETER_PID} 2>/dev/null && echo "  ✓ Speedometer stopped"
    [ -n "${GEARSTATE_PID}" ] && kill ${GEARSTATE_PID} 2>/dev/null && echo "  ✓ GearState stopped"
    
    sleep 1
    
    # Stop compositor last
    kill ${COMPOSITOR_PID} 2>/dev/null && echo "  ✓ Compositor stopped"
    
    sleep 1
    
    # Force kill any remaining processes
    pkill -9 -f GearState_app 2>/dev/null
    pkill -9 -f Speedometer_app 2>/dev/null
    pkill -9 -f BatteryMeter_app 2>/dev/null
    pkill -9 -f IC_Compositor 2>/dev/null
    
    echo "═══════════════════════════════════════════════════════"
    echo "IC System stopped"
    echo "═══════════════════════════════════════════════════════"
    exit 0
}

# Trap Ctrl+C and termination signals
trap cleanup SIGINT SIGTERM

# Wait for compositor to exit
wait ${COMPOSITOR_PID}

# If compositor exits, cleanup
cleanup
