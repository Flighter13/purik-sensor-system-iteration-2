"""Runtime registry for multiple Purik sensor nodes.

The ground station keys nodes by node_id, so two or more R4 nodes can operate
simultaneously without the UI or analysis layer caring which sensors are on
which board.
"""

from dataclasses import dataclass, field
from time import monotonic
from typing import Dict, List


@dataclass
class SensorRecord:
    sensor_id: int
    sensor_class: str
    schema: str
    online: bool = True
    last_seen_s: float = field(default_factory=monotonic)


@dataclass
class NodeRecord:
    node_id: int
    name: str = "unknown"
    online: bool = True
    last_seen_s: float = field(default_factory=monotonic)
    sensors: Dict[int, SensorRecord] = field(default_factory=dict)


class NodeRegistry:
    def __init__(self, timeout_s: float = 3.0):
        self.timeout_s = timeout_s
        self.nodes: Dict[int, NodeRecord] = {}

    def touch_node(self, node_id: int, name: str | None = None) -> NodeRecord:
        node = self.nodes.get(node_id)
        if node is None:
            node = NodeRecord(node_id=node_id, name=name or f"node-{node_id:02d}")
            self.nodes[node_id] = node
        elif name:
            node.name = name

        node.online = True
        node.last_seen_s = monotonic()
        return node

    def register_sensor(
        self,
        node_id: int,
        sensor_id: int,
        sensor_class: str,
        schema: str,
    ) -> SensorRecord:
        node = self.touch_node(node_id)
        sensor = node.sensors.get(sensor_id)
        if sensor is None:
            sensor = SensorRecord(
                sensor_id=sensor_id,
                sensor_class=sensor_class,
                schema=schema,
            )
            node.sensors[sensor_id] = sensor
        else:
            sensor.sensor_class = sensor_class
            sensor.schema = schema
            sensor.online = True
            sensor.last_seen_s = monotonic()
        return sensor

    def mark_timeouts(self) -> None:
        now = monotonic()
        for node in self.nodes.values():
            node.online = (now - node.last_seen_s) <= self.timeout_s
            for sensor in node.sensors.values():
                sensor.online = node.online and ((now - sensor.last_seen_s) <= self.timeout_s)

    def online_nodes(self) -> List[NodeRecord]:
        self.mark_timeouts()
        return [node for node in self.nodes.values() if node.online]
