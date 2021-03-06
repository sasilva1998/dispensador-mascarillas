from serial import Serial


class SerialCom:
    def __init__(
        self,
        arduino_port="/dev/ttyACM0",
        atmega_port="/dev/ttyACM1",
        baud_arduino=9600,
        baud_atmega=9600,
    ):

        # serial communication
        self.ser_arduino = Serial(arduino_port, baud_arduino, timeout=1)
        self.ser_atmega = Serial(atmega_port, baud_atmega, timeout=1)
        self.header = [0xFF, 0xFF]

    def com(self, device_id, inst, params=[]):
        packet = bytearray(self.make_packet(device_id, inst, params, len(params) + 2))
        if device_id == 1:
            print("*" * 15)
            print("comunicandose con el arduino")
            self.ser_arduino.write(packet)
        else:
            print("*" * 15)
            print("comunicacion de accion atmega")
            packet[0] = device_id
            packet[1] = inst
            self.ser_atmega.write(packet)

    def listen(self, device_id):

        if device_id == 1:
            uart = self.ser_arduino
        else:
            uart = self.ser_atmega

        if uart.in_waiting:
            temp = uart.read(2)
            # print(list(temp))
            if list(temp) == self.header:
                id_length = uart.read(2)
                device_id = list(id_length)[0]
                length = list(id_length)[1]
                data = [device_id, length]
                data += list(uart.read(length))
                print("recibiendo datos de los end device")
                # print(data)
                if device_id == 2:
                    return data
                if check_checksum(data[-1], data):
                    return data

            # print("Error in communication.")
            return False
        return False

    def make_packet(self, device_id, inst, params, length):
        packet = self.header + [device_id, length, inst]
        if len(params):
            packet += params
        # for i in params:
        #     if i > 255:
        #         packet += [i]
        #     else:
        #         packet += le(i)
        packet += [define_checksum(packet[2:])]
        return packet


def check_checksum(value, packet):
    checksum_val = 0
    for i in packet[:-1]:
        # print(i)
        checksum_val += i
    checksum_val = le(~checksum_val)[0]
    # print(checksum_val)
    if checksum_val == value:
        return True
    return False


def le(h):
    h &= 0xFFFF
    return [h & 0xFF, h >> 8]


def word(l, h):
    return (h << 8) + l


def define_checksum(packet):
    checksum = 0
    for i in packet:
        checksum += i
    checksum = le(~(checksum))[0]
    return checksum
