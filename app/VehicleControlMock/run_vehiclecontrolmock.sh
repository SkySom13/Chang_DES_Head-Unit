#!/bin/bash

# VehicleControlMock Startup Script
# Sets up environment variables and starts the VehicleControl mock service

echo "🚀 Starting VehicleControlMock..."

# Set environment variables
export VSOMEIP_CONFIGURATION="/home/seame/Documents/GitHub/Chang_DES_Head-Unit/app/VehicleControlMock/config/vsomeip_mock.json"
export VSOMEIP_APPLICATION_NAME="VehicleControlMock"
export COMMONAPI_CONFIG="/home/seame/Documents/GitHub/Chang_DES_Head-Unit/commonapi/commonapi.ini"

echo "Environment variables:"
echo "  VSOMEIP_CONFIGURATION: $VSOMEIP_CONFIGURATION"
echo "  VSOMEIP_APPLICATION_NAME: $VSOMEIP_APPLICATION_NAME"
echo "  COMMONAPI_CONFIG: $COMMONAPI_CONFIG"
echo ""

# Change to the VehicleControlMock directory
cd "/home/seame/Documents/GitHub/Chang_DES_Head-Unit/app/VehicleControlMock"

# Start the mock service
echo "🔧 Launching VehicleControlMock..."
./build/VehicleControlMock

echo "VehicleControlMock terminated."