import kivy
kivy.require("1.9.0")

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.core.window import Window
from kivy.uix.popup import Popup
from kivy.uix.gridlayout import GridLayout
from digi.xbee.devices import XBeeDevice
from digi.xbee.devices import RemoteXBeeDevice
from digi.xbee.models.address import XBee64BitAddress


class value_comm:
    device = XBeeDevice("/dev/tty.usbserial-A50285BI", 9600)
    remote_xbee = RemoteXBeeDevice(device, XBee64BitAddress.from_hex_string("0013A2004189A911"))
    def __init__(self,switch,speed,altitude,direction):
        self.switch = switch
        self.speed = speed
        self.altitude = altitude
        self.direction = direction
    def open_conn(self):
        self.device.open()
    
    def close_conn(self):
        self.device.close()
    def update_switch(self,value):
        self.switch = value
    def update_speed(self,value):
        self.speed = value
    def update_altitude(self,value):
        self.altitude = value
    def update_direction(self,value):
        self.direction = value 
    def get_values(self):
        return self.switch,self.speed,self.altitude,self.direction
    def xbee_send(self):
        self.device.send_data(self.remote_xbee,str(self.get_values()))

class CustomPopup(Popup):
    pass
 
class SampBoxLayout(BoxLayout):
    a = value_comm(0,0,0,0)
    def disp(self,flag):
        if flag == 1:
            self.display.text = str(self.a.get_values())
        elif flag == 2:
            self.display.text = "Connection Started"
        elif flag == 3:
            self.display.text = "Connection Closed"
        elif flag == 4:
            self.display.text = "Data Sent"
    def switch_on(self, instance, value):
        if value == True:
            self.a.update_switch(1)
            self.a.open_conn()
            self.disp(2)
        else:
            self.a.update_switch(0)
            self.a.close_conn()
            self.disp(3)
    def slider_val(self,instance,value):
        self.a.update_speed(value)

    def slider_direction(self,instance,value):
        self.a.update_direction(value)
        
    def slider_altitude(self,instance,value):
        self.a.update_altitude(value)

    def send_data(self):
        self.a.xbee_send()
        self.disp(4)
        
class codeApp(App):
    def build(self):
        Window.clearcolor = (1, 1, 1, 1)
        return SampBoxLayout()


app = codeApp()
app.run()
