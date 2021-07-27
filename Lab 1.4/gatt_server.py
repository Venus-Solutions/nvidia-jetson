import dbus
 
from advertisement import Advertisement
from service import Application, Service, Characteristic, Descriptor

import random

GATT_CHRC_IFACE = "org.bluez.GattCharacteristic1"
 
class NvidiaAdvertisement(Advertisement):
    def __init__(self, index):
        Advertisement.__init__(self, index, "peripheral")
        self.include_tx_power = True
 
class NvidiaService(Service):
    NVIDIA_SVC_UUID = "00000001-810e-4a5b-8d75-3e5b444bc3cf"
 
    def __init__(self, index):
        Service.__init__(self, index, self.NVIDIA_SVC_UUID, True)
        self.add_characteristic(NvidiaCharacteristic(self))
 
class NvidiaCharacteristic(Characteristic):
    NVIDIA_CHARACTERISTIC_UUID = "00000002-810e-4a5b-8d75-3e5b444bc3cf"
 
    def __init__(self, service):
        Characteristic.__init__(
                self, self.NVIDIA_CHARACTERISTIC_UUID,
                ["read"], service)
    
    def get_random_number(self):
        value = []

        num = random.randint(1, 9)

        strnum = str(num)

        for n in strnum:
            value.append(dbus.Byte(n.encode()))

        return value
 
    def ReadValue(self, options):
        value = self.get_random_number()
 
        return  value
 
app = Application()
app.add_service(NvidiaService(0))
app.register()
 
adv = NvidiaAdvertisement(0)
adv.register()
 
try:
    app.run()
except KeyboardInterrupt:
    app.quit()
    print("\nGATT application terminated")