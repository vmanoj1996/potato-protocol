# potato-protocol
A quick recipe for transferring structured data from Arduino to host PC. It is similar to MAVLink.

1. Upload this code into teensy 4.1 or any other arduino board.
2. Check if the arduino board prints gibberish on serial monitor. Close the serial monitor.
3. Set the Arduino serial port in main.cpp. Compile and run the host receiver code.
```
bash compile.bash && ./main.out
```

![image](https://github.com/user-attachments/assets/8fd26d11-4f6a-47a7-bedd-7aaa65d08f9e)


**Credits**:

 [Joao Alves CRC library](https://github.com/jpralves/crc16/tree/492dbcf2329147ed404eba1fc523af84430d5ec0) is used in this repo!
