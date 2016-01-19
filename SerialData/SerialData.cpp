// SerialData.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "oldaapi.h" //For Keithley
#using <System.dll>
#include <vcclr.h>
#include <atlstr.h>
#include <stdio.h>
#using <mscorlib.dll>
#include <string>
#include <msclr\marshal.h>
#include <iostream>
#include <math.h>
using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;
using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System;
using namespace msclr::interop;
string gps;
string compass;
//string lat_r;
//string lng_r;
//string lat_f;
//string lng_f;

char lat_r[2] = { 0 };
char lng_r[3] = { 0 };


char lat_f[7] = { 0 };
char lng_f[7] = { 0 };
string k;

double latitude;
double longitude;
//double direction;
double heading;

double lat_d= 12.837811;
double lng_d= 80.137932;
double bearing;

double x;
double y;


int NumberOfUSB1s = 0;
unsigned char ModuleAddress;
bool blnResult;
DBL volt = 2.5, gain, min0, max0, gain1, min1, max1;
HDEV hDev = NULL;
HDASS hDA = NULL, hDA1 = NULL;
LNG iValue;

char lpszDriverName1[100], lpszDriverName2[100];
UINT resolution, encoding, resolution1, encoding1, channel[2] = { 0,1 };
HDEV hDevice[2];
char Bname[2][100];
using namespace std;
 void EnumBrdProc(LPSTR lpszBrdName, LPSTR lpszDriverName, LPARAM lParam)
{
	// Make sure we can Init Board
	//MessageBox::Show("Hollo");
	(olDaInitialize((PTSTR)lpszBrdName, (LPHDEV)lParam));

	// Make sure Board has an A/D Subsystem 
	UINT uiCap = 0;
	olDaGetDevCaps(*((LPHDEV)lParam), OLDC_DAELEMENTS, &uiCap);

	//printf("Called%d\n",cnt);

}
/*Intitialisation of Keithley board*/
 void init(void) {
	olDaEnumBoards((DABRDPROC)EnumBrdProc, (LPARAM)&hDevice[0]);
	(olDaGetDeviceName(hDevice[0], (PTSTR)lpszDriverName1, 100));
	olDaGetDASS(hDevice[0], OLSS_DA, 0, &hDA);
	olDaSetDataFlow(hDA, OL_DF_SINGLEVALUE);
	olDaSetChannelListEntry(hDA, 0, channel[0]);
	olDaSetChannelListEntry(hDA, 0, channel[1]);
	olDaConfig(hDA);
	olDaGetResolution(hDA, &resolution);
	olDaGetEncoding(hDA, &encoding);
	olDaGetGainListEntry(hDA, 0, &gain);
	olDaGetRange(hDA, &max0, &min0);
	olDaVoltsToCode(min0, max0, gain, resolution, encoding, volt, &iValue);
	olDaPutSingleValue(hDA, iValue, channel[0], gain);
	olDaPutSingleValue(hDA, iValue, channel[1], gain);

}



public ref class PortChat
{
private:
	static bool _continue;
	static SerialPort^ _serialPort;
	static SerialPort^ comp;
	
	

public:
	static void Main()
	{
		
		String^ name;
		String^ message;
		//StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;
		Thread^ readThread = gcnew Thread(gcnew ThreadStart(PortChat::Read));
		Thread^ readThreadcomp = gcnew Thread(gcnew ThreadStart(PortChat::ReadComp));
		Thread^ disp = gcnew Thread(gcnew ThreadStart(PortChat::display));

		// Create a new SerialPort object with default settings.
		_serialPort = gcnew SerialPort();
		comp = gcnew SerialPort();;
		// Allow the user to set the appropriate properties.
		_serialPort->PortName = "COM8";
		_serialPort->BaudRate = 4800;
		comp->PortName = "COM14";
		comp->BaudRate = 4800;
	//	_serialPort->Parity = SetPortParity(_serialPort->Parity);
	//	_serialPort->DataBits = SetPortDataBits(_serialPort->DataBits);
	//	_serialPort->StopBits = SetPortStopBits(_serialPort->StopBits);
	//	_serialPort->Handshake = SetPortHandshake(_serialPort->Handshake);

		// Set the read/write timeouts
		_serialPort->ReadTimeout = 500;
		_serialPort->WriteTimeout = 500;
		comp->ReadTimeout = 500;
		comp->WriteTimeout = 500;

		_serialPort->Open();
		comp->Open();
		_continue = true;
		readThread->Start();
		readThreadcomp->Start();
		disp->Start();

	//	Console::Write("Name: ");
	//	name = Console::ReadLine();

	//	Console::WriteLine("Type QUIT to exit");

		//while (_continue)
		//{
		//	message = Console::ReadLine();

		//	if (stringComparer->Equals("quit", message))
		//	{
		//		_continue = false;
		//	}
		//	else
		//	{
		//		_serialPort->WriteLine(
		//			String::Format("<{0}>: {1}", name, message));
		//	}
		//}

		readThread->Join();
		readThreadcomp->Join();
		disp->Join();
		_serialPort->Close();
		comp->Close();
	}
	static void display() {
		

		while (1) {
			if (gps.compare(0, 6, "$GPRMC") == 0)
			{
				//std::cout << gps<<"\n";
				//std::cout << compass<<"\n";

				lat_r[0] = gps[20];
				lat_r[1] = gps[21];
				lat_r[2] = '\0';

				lat_f[0] = gps[22];
				lat_f[1] = gps[23];
				lat_f[2] = gps[24];
				lat_f[3] = gps[25];
				lat_f[4] = gps[26];
				lat_f[5] = gps[27];
				lat_f[6] = gps[28];
				lat_f[7] = '\0';

				lng_r[0] = gps[32];
				lng_r[1] = gps[33];
				lng_r[2] = gps[34];
				lng_r[3] = '\0';

				lng_f[0] = gps[35];
				lng_f[1] = gps[36];
				lng_f[2] = gps[37];
				lng_f[3] = gps[38];
				lng_f[4] = gps[39];
				lng_f[5] = gps[40];
				lng_f[6] = gps[41];
				lng_f[7] = '\0';



				/*std::cout << lat_r <<"-"<< lat_f <<"\t";
				std::cout << lng_r << "-" << lng_f << "\n";*/
				//double temp = strtod(lat_r, NULL);
				latitude = strtod(lat_r, NULL) + (strtod(lat_f, NULL) / 60);
				longitude = strtod(lng_r, NULL) + (strtod(lng_f, NULL) / 60.0);
				heading = strtod(compass.c_str(), 0);
				
				//std::cout << latitude << "\n";
				//printf("latutde=%f\t", latitude); printf("Heading=%f\n", heading);
				//printf("longitude=%f\n", longitude);

				x = cos(lat_d)*sin(lng_d - longitude);
				y = cos(latitude)*sin(lat_d) - sin(latitude)*cos(lat_d)*cos(lng_d - longitude);
				bearing = atan2(x , y)* (180 / Math::PI);

				if (bearing < 0) {
					bearing += 360;
				}

				if (lat_d == latitude && lng_d == longitude) {
					stopmotor();
				}

				if ((int)heading>=(int)bearing-5 || (int)heading<(int)bearing+5) {

					startmotor_L(4);
					startmotor_R(4);

				}


				if ((int)bearing > (int)heading+8) {//right
					startmotor_L(3.6);
					startmotor_R(1.4);

				}

				if ((int)bearing < (int) heading-8) {  
					//left

					startmotor_L(1.4);
					startmotor_R(3.6);
				}


				printf("heading=%f\t bearing=%f\n",heading,bearing);
				





				
			}
			
					
		}
	}

	static void Read()
	{
		while (_continue)
		{
			try
			{
				//String^ message = _serialPort->ReadLine();
				gps = (char*)(void*)Marshal::StringToHGlobalAnsi(_serialPort->ReadLine());

				//gps=marshal_as<const char*>(_serialPort->ReadLine());
				
				//Console::WriteLine(message);
			}
			catch (TimeoutException ^) {}
		}
	}
	static void ReadComp()
	{
		while (_continue)
		{
			try
			{

				compass = (char*)(void*)Marshal::StringToHGlobalAnsi(comp->ReadLine());
				//Console::WriteLine(message);
				//cout << compass << "\n";
			}
			catch (TimeoutException ^) {}
		}
	}

	


/* Function to stop motor*/
static void stopmotor()
{
	(olDaVoltsToCode(min0, max0, gain, resolution, encoding, 2.5, &iValue));  //To Stop Motor
	(olDaPutSingleValue(hDA, iValue, channel[0], gain));  
	(olDaPutSingleValue(hDA, iValue, channel[1], gain));
}

/*Function to start motor*/
static void startmotor_L(DBL v)
{
	(olDaVoltsToCode(min0, max0, gain, resolution, encoding, v, &iValue));  //To Stop Motor
	(olDaPutSingleValue(hDA, iValue, channel[0], gain));
	//(olDaPutSingleValue(hDA, iValue, channel[1], gain));
}

static void startmotor_R(DBL v)
{
	(olDaVoltsToCode(min0, max0, gain, resolution, encoding, v, &iValue));  //To Stop Motor
	//(olDaPutSingleValue(hDA, iValue, channel[0], gain));
	(olDaPutSingleValue(hDA, iValue, channel[1], gain));
}
  



};

int main()
{
	init();
	PortChat::Main();
}

