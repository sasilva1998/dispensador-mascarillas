
import threading
from cloud_control import CloudControl


def main():
    raspi_control = CloudControl()
    process_one = threading.Thread(target=raspi_control.serial_listener)
    process_one.setDaemon(True)
    process_one.start()


if __name__ == "__main__":
    main()
