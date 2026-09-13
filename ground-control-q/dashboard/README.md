# Ground-control dashboard

The touchscreen UI will run on the UNO Q and consume the local ground-control API / WebSocket service.

Planned views:

- node/device registry and health
- radar target display
- LiDAR visualization
- GNSS position/status
- IMU/attitude view
- transport/link status
- logging/replay controls

The dashboard should render from Purik capabilities and schemas rather than directly polling sensor-vendor endpoints.
