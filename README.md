# CH376s library v1.0.0

This is an Arduino library for control the serial USB CH376s IC, it work with Aruino Uno/Mega ( other to be tested ) using the SoftwareSerial 

Original functions founded on 		  http://arduinobasics.blogspot.cz/2015/05/ch376s-usb-readwrite-module.html
and from the foxik, ScottC library    https://github.com/foxik0070/arduino_CH376s

Thanks for the efforts guys, you made a great basic struture of the software to control the CH376s IC.

What's more in the library:

- According with the chip datasheet, for every possible command accepted by the CH376 a dedicated funtion has been added.
  This to make the library more understandable and remove the existing exadecimal part of code present in the old projects.

- In the old projects to execute a single command it was necessary to execute more funcions consecutively in the the main code.
  For this reason some additional macro-functions were created to allow the final users to execute only a single funtion in the main code and obtain the expected result.

- The software serial was used and the Tx and Rx pins can be directly selected from the main program, this to give more flexibily as possible.

- The necessary code to create and delete directories has been added. It is possible to create up to ten levels of subdirectories.
  The code allows to create and manipulate files inside to the directories and subdirectories as well.

- The serial terminal is used to monitor the commands executions and the reply received from the CH376s IC






