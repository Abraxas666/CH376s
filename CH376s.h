#include "Arduino.h"
#include <SoftwareSerial.h>

#ifndef __CH376s_h__
#define __CH376s_h__
extern int const RX_pin;
extern int const TX_pin;
#endif

class CH376s
{
  private:

  	void CMD_Reset_All();
	void CMD_Check_Exist(); 
	void CMD_Set_Usb(); 
	void CMD_Read_Usb_Data();
	void CMD_Get_File_Size();
	void CMD_Write_Data();
	void CMD_Set_File_Name(); 
	void CMD_Disk_Connect();
	void CMD_Disk_Mount();
	void CMD_Open();
	void CMD_Set_File_Create();
	void CMD_Set_File_Erase();
	void CMD_Close();
	void CMD_Set_Pointer();
	void CMD_Byte_Read();
	void CMD_Byte_Cont_Read();
	void CMD_Byte_Write();
	void CMD_Byte_Cont_Write();
	void CMD_Dir_Create();
	void CMD_Set_Begin();
	void CMD_Set_End();
	void CMD_End_Byte();

	void USBdiskMount();					
	void set_USB_Mode ();					
    void resetALL();						
    void begin(long speed);
    void diskConnectionStatus();											

	void fileWrite(String);
	void fileRead();
	void fileOpen();
	void tryOpen();
	void fileClose();
	void fileUpdate();
	void filePointer(boolean);
	void setFileName(String);
	void writeFolder(String);
	void folderUpdate();
	void folderOpen(String,String);
	void setFolderName(String); 


	boolean fileCreate();
	boolean continueRead();
	boolean waitForResponse(String);
	byte 	getResponseFromUSB();
	boolean setByteRead(byte);
	boolean folderCreate();
	int  	getFileSize();				
	
  public:

    CH376s(uint8_t defRX,uint8_t defTX);	// Define Class
    
    void checkConnection();					// Check connection with the CH376s IC
    void checkUnexpectMsg();				// Check if unexpected messages was sent by the CH376s IC
    void init();							// Init the CH376s connection and check the USB status
	
	void readFile(String,String);			// Read an existing file
	void writeFile(String,String,String);	// Overwrite an existing file. If the file does not exist create a new file.
	void deleteFile(String,String);			// Delete an existing file
	void appendFile(String,String,String);	// Append data to an existing file or create a new one
	
	void openDir(String);					// Create or open a directory
	void deleteDir(String,String);			// Delete an existing directory
	     
};
