import sys
from platformio.device.list.util import list_serial_ports

device_list = []

print("\n#################################\n#### List of available ports ####\n#################################\n")

for device in list_serial_ports():
    if (device["hwid"] != 'n/a'):
        print("Port: ", device["port"])
        print("Description: ", device["description"])
        print("hwid", device["hwid"])
        print("\n")

        device_list.append(device["port"])

    