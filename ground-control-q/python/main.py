"""Purik Iteration 2 ground-control entry point.

Ground control is node-agnostic: it can host telemetry from multiple UNO R4
sensor nodes simultaneously. Sensor-vendor-specific code stays on the nodes.
"""

from services.node_registry import NodeRegistry


def main():
    print("Purik Ground Control - Iteration 2")
    print("Multi-node ground-control baseline starting...")

    registry = NodeRegistry(timeout_s=3.0)

    # Development placeholders proving the data model can hold two R4 nodes.
    # These will be replaced by live nRF24/Wi-Fi receiver callbacks.
    registry.touch_node(1, "sensor-node-01")
    registry.touch_node(2, "sensor-node-02")

    for node in registry.online_nodes():
        print(f"Node online: id={node.node_id} name={node.name}")

    print("Receiver, decoder, telemetry, API, and dashboard services pending.")


if __name__ == "__main__":
    main()
