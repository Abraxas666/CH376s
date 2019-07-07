#include "CH376s.h"

SoftwareSerial  USB(RX_pin,TX_pin);

#define NULL_CHAR           0x00
#define USB_INT_SUCCESS     0x14
#define USB_INT_DISK_READ   0x1D
#define USB_INT_DISK_WRITE  0x1E
#define ERR_OPEN_DIR        0x41
#define FIRST_BYTE          0x00                   
#define NUM_BYTES           0x40
#define CLOSE               0x00
#define UPDATE              0x01
#define CHECK               0x01
#define CHECK_OK            0xFE
#define USB_SET_MODE        0x06
#define CMD_RET_SUCCESS     0x51
#define USB_INT_CONNECT     0x15


//////////////////////////////////////////////   CH376S COMMANDS  /////////////////////////////////////////////////////

void CH376s::CMD_Reset_All() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x05);  
  }
void CH376s::CMD_Check_Exist() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x06);  
  }
void CH376s::CMD_Set_Usb() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x15);  
  }
void CH376s::CMD_Read_Usb_Data() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x27);  
  }
void CH376s::CMD_Get_File_Size() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x0C);
  USB.write(0x68);  
  }
void CH376s::CMD_Write_Data() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x2D);  
  }
void CH376s::CMD_Set_File_Name() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x2F);  
  }
void CH376s::CMD_Disk_Connect() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x30);  
  }
void CH376s::CMD_Disk_Mount() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x31);  
  }
void CH376s::CMD_Open() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x32);  
  }
void CH376s::CMD_Set_File_Create() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x34);  
  }
void CH376s::CMD_Set_File_Erase() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x35);  
  }
void CH376s::CMD_Close() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x36);  
  }
void CH376s::CMD_Set_Pointer() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x39);  
  }
void CH376s::CMD_Byte_Read() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x3A);  
  }
void CH376s::CMD_Byte_Cont_Read() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x3B);  
  }
void CH376s::CMD_Byte_Write() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x3C);  
  }
void CH376s::CMD_Byte_Cont_Write() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x3D);  
  }
void CH376s::CMD_Dir_Create() {
  USB.write(0x57);
  USB.write(0xAB);
  USB.write(0x40);  
  }
void CH376s::CMD_Set_Begin() {
  USB.write((byte)0x00);             // Beginning of file
  USB.write((byte)0x00);
  USB.write((byte)0x00);
  USB.write((byte)0x00);  
  }
void CH376s::CMD_Set_End() {
  USB.write((byte)0xFF);             // End of file
  USB.write((byte)0xFF);
  USB.write((byte)0xFF);
  USB.write((byte)0xFF);  
  }
void CH376s::CMD_End_Byte() {
  USB.write((byte)0x00);             // End of file name  
  }

////////////////////////////////////////////////////////////// PRIVATE FUNCTIONS /////////////////////////////////////////////////////////////////////

void CH376s::USBdiskMount(){                        // Initialise the USB disk and check that it is ready.This process is required if you want to find the manufacturing information of the USB disk
  Serial.println("Mounting USB disk");
  CMD_Disk_Mount();

  if(waitForResponse("Connecting to USB disk")){    
    if(getResponseFromUSB()== USB_INT_SUCCESS){     
       Serial.println("Disk mounted - OK");
    } else {
      Serial.println(getResponseFromUSB());
      Serial.println("Failed to mount USB disk.");
    }
  }
}

void CH376s::set_USB_Mode (){                 // Make sure that the USB is inserted
    CMD_Set_Usb();
    USB.write(USB_SET_MODE);
    delay(20);
  
    if(USB.available()){
      byte USB_Byte;                                
      USB_Byte = USB.read();

      if(USB_Byte == CMD_RET_SUCCESS){              
        Serial.println("Set_USB_Mode command acknowledged"); 
        USB_Byte = USB.read();              
        if(USB_Byte == USB_INT_CONNECT){            
          Serial.println("USB is present");         
          }                                 
        else {
          Serial.print("USB Not present. Error code:"); 
          Serial.print(USB_Byte, HEX);
          Serial.println("H");
          }
      } 
      else {
        Serial.print("CH3765 error!   Error code:");
        Serial.print(USB_Byte, HEX);
        Serial.println("H");
     }   
   }
   delay(20);
}

void CH376s::resetALL(){                  // Perform a hardware reset of the CH376S module
    CMD_Reset_All();
    Serial.println("The CH376S module has been reset !");
    delay(200);
}

void CH376s::begin(long speed)			// Initizialize the connection
{
  USB.begin(speed); 
}

void CH376s::diskConnectionStatus(){                // Check the disk connection status
  Serial.println("Checking USB disk connection status");
  CMD_Disk_Connect();

  if(waitForResponse("Connecting to USB disk")){    
    if(getResponseFromUSB()== USB_INT_SUCCESS){    
       Serial.println("Connection to USB OK");
    } else {
      Serial.print("Connection to USB - FAILED.");
    }
  }
}

void CH376s::fileWrite(String data){              	// Used for write into a file
  Serial.print("Writing to file:");
  byte dataLength = (byte) data.length();         	
  Serial.println(data);
  Serial.print("Data Length:");
  Serial.println(dataLength);
  delay(100);
  CMD_Byte_Write(); 
  USB.write((byte) dataLength);                 	
  CMD_End_Byte();
  if(waitForResponse("setting data Length")){      	
    if(getResponseFromUSB()== USB_INT_DISK_WRITE){  
      CMD_Write_Data();
      USB.print(data);                             	
  
      if(waitForResponse("writing data to file")){  
      }
      Serial.print("Write code (normally FF and 14): ");
      Serial.print(USB.read(),HEX);                	
      Serial.print(",");
      CMD_Byte_Cont_Write();                        
      if(waitForResponse("updating file size")){   
      }
      Serial.println(USB.read(),HEX);               
    }
  }
}

void CH376s::fileRead(){                        	// Used to read the contents of the file
  Serial.println("Reading file:");
  byte firstByte = FIRST_BYTE;                  	
  byte numBytes = NUM_BYTES;                    
  while(setByteRead(numBytes)){              		
    CMD_Read_Usb_Data();                        	
    if(waitForResponse("reading data")){      		
        firstByte=USB.read();                 		
        while(USB.available()){
          Serial.write(USB.read());           		
          delay(1);                           		
        }
    }
    if(!continueRead()){                        	
      break;                                    	
    }
  }
  Serial.println();
  Serial.println("NO MORE DATA");
}

void CH376s::fileOpen(){                          	// Used the file for reading or writing
  Serial.println("Opening file.");
  CMD_Open(); 
  if(waitForResponse("file Open")){                 
    if(getResponseFromUSB()== USB_INT_SUCCESS){     
       Serial.println("File opened successfully.");
    } else {
      Serial.println("Failed to open file.");
      fileCreate();
    }
  }
}

void CH376s::tryOpen(){                            // Used the file for reading or writing
  Serial.println("Opening file.");
  CMD_Open(); 
  if(waitForResponse("file Open")){                 
    if(getResponseFromUSB()== USB_INT_SUCCESS){     
       Serial.println("File opened successfully.");
    } else {
      Serial.println("Failed to open file.");
    }
  }
}

void CH376s::fileClose(){                           // Used to close the files
  Serial.print("Closing file:");
  CMD_Close();
  USB.write((byte)CLOSE);                           

  if(waitForResponse("closing file")){              
     byte resp = getResponseFromUSB();
     if(resp == USB_INT_SUCCESS){                   
       Serial.println(" File closed successfully.");
     } else {
       Serial.print(" Failed to close file. Error code:");
       Serial.println(resp, HEX);
     }  
  }
}

void CH376s::fileUpdate(){                          // Used to close and update the files
  Serial.print("Closing file:");
  CMD_Close();
  USB.write((byte)UPDATE);                               

  if(waitForResponse("Closing file")){              
     byte resp = getResponseFromUSB();
     if(resp == USB_INT_SUCCESS){                   
       Serial.println("File updated successfully.");
     } else {
       Serial.print("Failed to update file. Error code:");
       Serial.println(resp, HEX);
     }  
  }
}

void CH376s::filePointer(boolean fileBeginning){    // FilePointer is used to set the file pointer position. True for beginning of file, false for the end of the file.
  CMD_Set_Pointer();
  if(fileBeginning){
    CMD_Set_Begin();} 
  else {CMD_Set_End();}
  if(waitForResponse("setting file pointer")){      
     if(getResponseFromUSB()== USB_INT_SUCCESS){    
       Serial.println("Pointer successfully applied");
     }
  }
}

void CH376s::setFileName(String fileName){          // Sets the name of the file to work with
  Serial.print("Setting filename to:");
  Serial.println(fileName);
  CMD_Set_File_Name();                            
  USB.print(fileName);                            
  CMD_End_Byte();
  delay(20);
}

void CH376s::writeFolder(String folderName){  	   // Used to create a new folder 

  setFolderName(folderName);          			   
  if(folderCreate()){               			  
    Serial.println("Folder created successfully");
    }
  else {Serial.println("Folder could not be created, or it already exists");
    }
  folderUpdate();
}

void CH376s::folderUpdate(){            		   // Used to update a folder
  Serial.print("Closing folder:");
  CMD_Close();
  USB.write((byte)UPDATE);                               

  if(waitForResponse("closing folder")){           
     byte resp = getResponseFromUSB();
     if(resp == USB_INT_SUCCESS){                  
       Serial.println(" Folder updated successfully.");
     } else {
       Serial.print(" Failed to update folder. Error code:");
       Serial.println(resp, HEX);
     }  
  }
}

void CH376s::folderOpen(String folderName, String path){      // FolderOpen is used to navigate inside a folder, if the folder does not exist, create the folder first
  Serial.println("--------------------- OPEN FOLDER ---------------------");
  Serial.println("Opening folder.");
  setFolderName(path);
  CMD_Open();                                    
  if(waitForResponse("file Open")){                 
    if(getResponseFromUSB()== ERR_OPEN_DIR){       
       Serial.println("Folder opened successfully.");
       } 
    else {
      int length = folderName.length();
      folderName = folderName.substring(1,length);
      Serial.print("foldername = ");
      Serial.println(folderName);
      writeFolder(folderName);
      Serial.println("Folder was not present,folder created!");      
      }
   }
  Serial.println("------------------ END OF OPEN FOLDER -----------------");
}

void CH376s::setFolderName(String folderName){			// Sets the name of the folder to work with
  Serial.print("Setting folder name to:");
  Serial.println(folderName);
  CMD_Set_File_Name();                          
  USB.print(folderName);                          
  CMD_End_Byte();
  delay(20);
}

boolean CH376s::fileCreate(){                 	   // Used to create a file
  boolean createdFile = false;
  CMD_Set_File_Create();
  if(waitForResponse("creating file")){            
     if(getResponseFromUSB()== USB_INT_SUCCESS){   
       createdFile=true;
     }
  }
  return(createdFile);
}

boolean CH376s::continueRead(){                    // Continue to read the file : I could not get this function to work as intended.
  boolean readAgain = false;
  CMD_Byte_Cont_Read();
  if(waitForResponse("continueRead")){             
     if(getResponseFromUSB()== USB_INT_SUCCESS){   
       readAgain=true;
     }
  }
  return(readAgain);
} 

boolean CH376s::waitForResponse(String errorMsg){  // WaitForResponse. Returns true when bytes become available, false if it times out.
  boolean bytesAvailable = true;
  int counter=0;
  int timeOut = 400;          					   

  while(!USB.available()){     					  
    delay(1);
    counter++;
    if(counter > timeOut){
      Serial.print("TimeOut waiting for response: Error while: ");
      Serial.println(errorMsg);
      bytesAvailable = false;
      break;
    }
  }
  delay(1);
  return(bytesAvailable);
}

byte CH376s::getResponseFromUSB(){  			   // ResponseFromUSB.Get any error codes or messages from the CH376S 
  byte response = NULL_CHAR;
  if (USB.available()){
    response = USB.read();
  }
  return(response);
}

boolean CH376s::setByteRead(byte numBytes){ 	   // This function is required if you want to read data from the file.
  boolean bytesToRead=false;
  int timeCounter = 0;
  CMD_Byte_Read();
  USB.write((byte)numBytes);   					   
  CMD_End_Byte();
  if(waitForResponse("setByteRead")){       	   
    if(getResponseFromUSB()== USB_INT_DISK_READ){ 
      bytesToRead=true;
    }
  }
  return(bytesToRead);
} 

boolean CH376s::folderCreate(){             	   // The command sequence to create a folder
  boolean createdFolder = false;
  CMD_Dir_Create();
  if(waitForResponse("creating file")){       	   
     if(getResponseFromUSB()== USB_INT_SUCCESS){   
       createdFolder=true;
     }
  }
  return(createdFolder);
}

int CH376s::getFileSize(){              // GetFileSize() is used to writes the file size to the serial Monitor.
  int fileSize=0;
  Serial.println("Getting File Size");
  CMD_Get_File_Size();
  delay(100);
  Serial.print("FileSize = ");
  if(USB.available()){
    fileSize = fileSize + USB.read();
  } 
  if(USB.available()){
    fileSize = fileSize + (USB.read()*255);
  } 
  if(USB.available()){
    fileSize = fileSize + (USB.read()*255*255);
  } 
  if(USB.available()){
    fileSize = fileSize + (USB.read()*255*255*255);
  }     
  Serial.println(fileSize);
  delay(10);
  return(fileSize);
}



//////////////////////////////////////////////////////// PUBLIC FUNCTIONS /////////////////////////////////////////////////////////////////////////////

CH376s::CH376s(uint8_t defRX,uint8_t defTX) {			// define class name
}

void CH376s::init(){									// initialize connection with CH376s and check the USB status
    begin(9600); 
    Serial.println("-------------------- INIT CH376 ---------------------");
    resetALL();                        
    set_USB_Mode();                    
    diskConnectionStatus();            
    USBdiskMount();                    
    Serial.println("---------------- END OF INTIALIZATION  ----------------");  
}

void CH376s::checkConnection(){ 				    // This function is used to check for successful communication with the CH376S module.
  CMD_Check_Exist();
  USB.write((byte)CHECK);
  Serial.println("------------------ CHECK CONNECTION -------------------");
  if(waitForResponse("checking connection")){       
    if(getResponseFromUSB()== CHECK_OK){
      Serial.println("Connection to CH376S was successful.");      
      }
    else {
      Serial.print("Connection to CH376S - FAILED.");
      }
  }
  Serial.println("------------------ CHECK COMPLETED  -------------------");  
}

void CH376s::checkUnexpectMsg(){
  if(USB.available()){                              	// If unexpected data transmitted by the CH376S module
    Serial.print("CH376S has just sent the message: ");
    Serial.println(USB.read(), HEX);
    }
 }

void CH376s::readFile(String fileName,String path){ 	// Used to read a file

  openDir(path);
  Serial.println("------------------ FILE READING -----------------------");
  setFileName(fileName);          					
  fileOpen();                     				    
  int fs = getFileSize();         					
  fileRead();                     					
  fileClose();                						
  Serial.println("-------------- FILE READING COMPLETED -----------------");
}

void CH376s::writeFile(String fileName, String data,String path){     // Used to write a file      
  openDir(path);
  setFileName(fileName);          					
  if(fileCreate()){               					
    fileWrite(data);              					
    } 
  else {
    Serial.println("File could not be created, or it already exists");
    }
  fileUpdate();
  Serial.println("------------------ FILE CREATED -----------------------");
}

void CH376s::deleteFile(String fileName,String path){     		// Used to delete a file
  openDir(path);
  Serial.println("----------------- FILE DELETING -----------------------");
  setFileName(fileName);
  tryOpen(); 
  delay(20);
  CMD_Set_File_Erase();
  if(waitForResponse("deleting file")){       		  
     if(getResponseFromUSB()== USB_INT_SUCCESS){    
       Serial.println("Successfully deleted file");
     }
     else { 
     	Serial.println("File could not be deleted, or it does not exists");
     	}
  }
  Serial.println("---------------- FILE DELETE END ----------------------");
}
void CH376s::appendFile(String fileName, String data, String path){  // AppendFile is used to write data to the end of the file, without erasing the contents of the file.
    openDir(path);
    Serial.println("-------------------- APPEND FILE ----------------------");
    setFileName(fileName);          				
    fileOpen();                     				
    filePointer(false);             				
    fileWrite(data);                				
    fileUpdate();                					
    Serial.println("------------------- END OF APPEND FILE ----------------");
}

void CH376s::deleteDir(String fileName,String path){         // Used to delete a file
  openDir(path);
  Serial.println("----------------- FOLDER DELETING -----------------------");
  setFileName(fileName);
  delay(20);
  CMD_Set_File_Erase();
  if(waitForResponse("deleting folder")){           
     if(getResponseFromUSB()== USB_INT_SUCCESS){    
       Serial.println("Successfully deleted folder");
     }
     else { 
      Serial.println("Folder could not be deleted, or it does not exists");
      }
  }
  Serial.println("---------------- FOLDER DELETE END ----------------------");
}

void CH376s::openDir(String dirNames){         // Used to open or create a directory

  int count =0;
  int index[] = {-1,0,0,0,0,0,0,0,0,0,0,0};
  String folderName;
  String path;
  String root = "/\0";
  
  for ( int i = 0; i < dirNames.length() +1; i++ ){
    if (dirNames[i] == '/'){
      count++;
      index[count]=i;
    }
  }
  index[count+1] = dirNames.length();
  folderOpen(root,root);
  
  for ( int i = 0; i < count+1; i++ ){
    String dirName = dirNames.substring(index[i]+1,index[i+1]);
    if (dirName.length() > 8) { Serial.println("ERROR name too long! Dir will not be created!");folderName = ""; path = "";break;}
    else if (dirName == "root" ) { folderName = ""; path = "";break;}
    else {
      if ( i == 0 ){
        folderName = "/";
        folderName+= dirName;
        folderName+= "\0";
        path = dirName;
        folderOpen(folderName,path);
        }
      else {
        folderName = " ";
        folderName+= dirName;
        folderName+= "\0";
        path = dirName;
        folderOpen(folderName,path);
        }
    }
    folderName = "";
  }
  path = "";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
