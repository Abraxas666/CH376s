
// The serial terminal is used to monitor the commands executions and the reply received from the CH376s IC
// The example shows how to crete,manipulate and delete files and directories
// The example shows also how to log data to a csv file

#include <CH376s.h>

#define Tx  49
#define Rx  51

int const TX_pin = Tx;
int const RX_pin = Rx;

byte computerByte;           //used to store data coming from the computer

String wrData  = "What is the meaning of life?";    
String wrData2 = "The answer is 42.";  
String wrData3 = "\tTime\tTemperature\tPressure\tHumidity"; 

String root   = "root";                          
String path1  = "DIR1";
String path2  = "DIR1/DIR2/DIR3";
String path3  = "DIR1/DIR2/DIR3/DIR4";

String folderName = "DIR3";

String fileName1  = "FILETXT.TXT";
String fileName2  = "FILECSV.CSV";


CH376s CH376(Rx,Tx);                        // define class name
                                                 
//==============================================================================================================================================
void setup() {
  Serial.begin(9600);                       // Setup serial communication with the computer (using a baud rate of 9600 on serial monitor)
  CH376.init();                             // Init the connection with CH376 and check the USB status
}

void loop() {
  if(Serial.available()){
    computerByte = Serial.read();                    
      if(computerByte==49){                         // If number 1 from the serial monitor is received
        CH376.checkConnection();                    // Check for successful connection and communication with the CH376S module.
        }
        if(computerByte==50){                       // If number 2 from the serial monitor is received 
        CH376.writeFile(fileName1,wrData,root);     // Create a file. If file exists,overwrite it.
        }
      if(computerByte==51){                         // If number 3 from the serial monitor is received        
        CH376.appendFile(fileName1,wrData2,path1);  // Create a file inside a dir. If folder exist open it,file exists,append it.
        }
      if(computerByte==52){                         // If number 4 from the serial monitor is received  
        CH376.readFile(fileName1,root);             // Read the file called "FILETXT.TXT" in the main root
        }
       if(computerByte==53){                        // If number 5 from the serial monitor is received        
        CH376.appendFile(fileName1,wrData2,path3);  // Append a file called FILETXT.TXT on path /DIR1/DIR2/DIR3/DIR4 if folder and/or files does not exist it create it.
        }
      if(computerByte==54){                         // If number 6 from the serial monitor is received    
        CH376.readFile(fileName1,path3);            // Read the file called "FILETXT.TXT" on path /DIR1/DIR2/DIR3/DIR4 
        }
      if(computerByte==55){                         // If number 7 from the serial monitor is received 
        CH376.deleteFile(fileName1,root);           // Delete the file called "FILETXT.TXT" inside the main root
        }
       if(computerByte==56){                        // If number 8 from the serial monitor is received 
        CH376.deleteFile(fileName1,path3);          // Delete the file called "FILETXT.TXT" on path /DIR1/DIR2/DIR3/DIR4 
        }  
      if(computerByte==57){                         // If number 9 from the serial monitor is received 
         CH376.deleteDir(folderName,path2);         // Delete the entire directory called DIR3 on path /DIR1/DIR2/DIR3
        }    
      if(computerByte==48){                         // If number 0 from the serial monitor is received
        CH376.writeFile(fileName2,wrData3,root);    // Create a file called FILECSV.CSV and then Write the contents of wrData to this file
        int time =0;                                // CSV file can normally opened with excel using text to coluumns function, splittig the columns by tab
        float temp = 26;
        float press = 1013.25;
        float humidity = 60;
        String wrData4 = ""; 
        String wrData5 = "\t";
        String wrData6 = "\n";
        for ( int i = 0; i < 10; i++ ){             // put some data in the table, time,temp,press and humidity.
          time += i; 
          temp += i;
          press += i;
          humidity += i;
          wrData4 += wrData6;
          wrData4 += wrData5;
          wrData4 += time;
          wrData4 += wrData5;
          wrData4 += temp;
          wrData4 += wrData5;
          wrData4 += press;
          wrData4 += wrData5;
          wrData4 += humidity;
          CH376.appendFile(fileName2,wrData4,root);  // Append data to this file
          wrData4 = "";
          delay(250);
          }
        }          
  }
  CH376.checkUnexpectMsg();                         // Check for unexpected data transmitted by the CH376S module
}

