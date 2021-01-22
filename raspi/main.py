import threading
from cloud_control import CloudControl


def main():
    raspi_control = CloudControl(arduino_port="/dev/ttyS2", atmega_port="/dev/ttyS1")
    print("empezando")
    process_one = threading.Thread(target=raspi_control.serial_listener)
    process_two = threading.Thread(target=raspi_control.mqtt_publisher)
    process_one.setDaemon(False)
    process_two.setDaemon(False)
    process_one.start()
    process_two.start()


if __name__ == "__main__":
    main()
