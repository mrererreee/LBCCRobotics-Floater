**hc05-comms-check**

1. install putty (for secondary computer to communicate through serial Bluetooth port) https://putty.org (can pull up any youtube tutorial)
2. install esp32 boards library 

   1. tools -> boards manager -> esp32 (by espressif) 
   2. connect Arduino ide to proper dev board, use board manager to choose board and proper COM port
3. make connections \[insert Bluetooth module connections] with HC05 and esp32
4. in serial computer (computer you're using to communicate to the putty computer), input code attached in this folder
5. upload to ESP32 (assuming attachments are correct)
6. pair putty computer to Bluetooth port (for non-"LBCC-SIM2" Bluetooth modules, name is "HC05" and pw is "1234". otherwise pw is 1305)
7. open putty, make sure you click serial connection, and change the COM(number) to whatever your device manager says your Bluetooth device is on

   1. click forced on for echo and feedback(?)
8. change serial computer baud rate to 115200 and (NL + CR)
9. type characters back and forth from either computers, it should show on your terminals

