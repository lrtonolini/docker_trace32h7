# Welcome TRACE32_H7 Linux 

_TRACE32_H7 Linux_ is a package to start Trace 32 with arduino Portenta H7 in Linux. There are three different ways to debug : GBD, JTAG and Simulator. 


## Fisrt install

After download .zip, you can extract file to **HOME**.


## Start 

Open terminal and use `cd TRACE32_H7` to go in file. 
Use `./Arduino_linux.sh` to open Trace32_H7 in linux.

Now, you can start Simulator and JTAG mode. If you want to use GDB mode, please refer to the next part "Start GBD mode". 

__Warning__ : If you want to debug in JTAG mode, you need probe and licence. Please, refere to [Lauterbach website](https://www.lauterbach.com/).

## Start GDB mode

To debug in GDB mode, you need to procure arduino licence for Trace 32. It's a free license and will be valid for one year. 

To get one, follow these steps : 

1. Please install ArduinoIDE : [install ArduinoIDE](https://www.arduino.cc/en/software).

2. Connect the board and select "Arduino Portenta H7 (M7 core)" "COM** Serial Port (USB)" 

3. In Arduino IDE, select "Tools -> Board : -> Board Manager " and install "Arduino Mbed Os Portenta Board by Arduino" version 4.0.4

4. In Arduino IDE, select "Tools -> GET Board Info" and raise a serial number (SN). 

5. Go to Lauterbach registration page : [Lauterbach.com](https://repo.lauterbach.com/register_arduino.php)

6. Complete the form to get your new license 

7. Start demo in TRACE32_H7, select "Portenta H7 -> Debug GDB -> DEbug GDB -> T32THreadDebug". N.B : As soon as you launch the demo, you'll get a license error, that's normal.

8. In command line, write `License.state` and copy the license key. 

9. Clic to button `Save`

**Now, you can use TRACE32_H7 in a GDB mode.** 


## Documentation 
To begin debugging, please refer to file _pdf_ in the folder _TRACE32_H7\TRACE32_.

To start debugging with Lauterbach on the Portenta H7 , you can refer to this link [Arduino tuto](https://docs.arduino.cc/tutorials/nano-33-ble/lauterbach-debugger/).

## Help 

If you need to install drivers, please open terminal and use`cd TRACE32_H7` to go in file. Then use `./driver_linux.sh` to install drivers.

For more information, please conatact support_fr@lautebabach.com
