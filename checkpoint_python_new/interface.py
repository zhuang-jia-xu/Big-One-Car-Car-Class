import bluetooth as BT

class interface:
    def __init__(self):
        print("")
        print("Arduino Bluetooth Connect Program.")
        print("")
        self.ser = BT.bluetooth()
        port = "com17"
        while(not self.ser.do_connect(port)):
            if(port == "quit"):
                self.ser.disconnect()
                quit()
            port = "com17"
        input("Press enter to start.")
        self.ser.write('s')
    def send_dir(self,dir):
        self.ser.write(dir)
    def get_UID(self):
        return self.ser.SerialReadByte()
    def get_i(self):
        while True:
            msg=self.ser.readByte()
            print(msg)
            if msg=='i':#RFID start
                break

    def end_process(self):
        self.ser.write('e')
        self.ser.disconnect()