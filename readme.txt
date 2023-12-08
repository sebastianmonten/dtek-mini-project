To compile project:

make

How to upload to ChipKIT uC32:

make install

If this does not work, try:

make install TTYDEV=/dev/ttyUSB0

where "/dev/ttyUSB0" is the name of the USB serial port connected to the ChipKIT uC32