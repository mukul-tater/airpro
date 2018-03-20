#include <SoftwareSerial.h>
#include <MQ135.h>
SoftwareSerial gsm(2, 3);
MQ135 gasSensor = MQ135(A0);

unsigned long duration;
unsigned long starttime;
unsigned long endtime;
unsigned long responseTime = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
int pm2_5 = 8;

String gpsInfo[5];
String lat, lon, utc;

float aqi;
float sensor_volt;
float RS_gas;
float ratioMQ2;
int sensorValue;

void setup()
{
	Serial.begin(9600);
	gsm.begin(9600);
	if (gsm.available() > 0)
		Serial.println("Config GSM Module");
	Serial.flush();
	gsm.flush();
	delay(2000);
	// for GPS

  sendIt("AT\r", 1000, true);
	// for GPRS service
	sendIt("AT+CGATT?\r", 1000, true);
	// bearer settings
	sendIt("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r", 2000, true);
	// APN settings
	sendIt("AT+SAPBR=3,1,\"APN\",\"internet\"\r", 2000, true);
	// Test
	sendIt("AT+SAPBR=1,1\r", 2000, true);
	sendIt("AT+SAPBR=2,1\r", 2000, true);
	// Init http service
	sendIt("AT+HTTPINIT\r", 1000, true);
	sendIt("AT+HTTPPARA=\"CID\",1\r", 2000, true);
  sendIt("AT+CGNSPWR=1\r", 2000, true);
  sendIt("AT+CGPSINF=0\r", 2000, true);
	Serial.println("Done!...");

	// Pinmode set
	pinMode(pm2_5, INPUT);
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
    pinMode(A2, INPUT);
	starttime = millis();
}

void loop()
{

	duration = pulseIn(pm2_5, LOW);
	lowpulseoccupancy += duration;
	endtime = millis();

	if ((endtime - starttime) > responseTime)
	{
		ratio = (lowpulseoccupancy - endtime + starttime + responseTime) / (responseTime * 10.0);
		concentration = (1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62) / 10.0;

        sensorValue = analogRead(A1);

		float mq135 = gasSensor.getPPM();

		sensor_volt = (float)sensorValue / 1024 * 5.0;
		RS_gas = (5.0 - sensor_volt) / sensor_volt; // omit *RL
		ratioMQ2 = RS_gas / 0.50;

		float V_RL = analogRead(A2)* 3.3/4095;
		float ppm = 3.027*exp(1.0698*V_RL);

		aqi = (concentration + ratioMQ2 + ppm + mq135)/5;

		getGPSData();

		gsm.print("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update?api_key=IU1DPU7AA5F9XNHO&field1=");
		gsm.print(ratioMQ2);
		gsm.print("&field2=");
		gsm.print(mq135);
		gsm.print("&field3=");
		gsm.print(ppm);
		gsm.print("&field4=");
		gsm.print(concentration);
		gsm.print("&field5=");
		gsm.print(lat);
		gsm.print("&field6=");
		gsm.print(lon);
		gsm.print("&field7=");
		gsm.print(aqi);
		sendIt("\"\r", 200, true);
		// Get request
		sendIt("AT+HTTPACTION=0\r", 1000, true);
		Serial.println("\nData Sent ! ");

		lowpulseoccupancy = 0;
		starttime = millis();
	}
}

String sendIt(String command, const int timeout, boolean debug)
{
	String rd = "";
	gsm.print(command);
	long int time = millis();
	while ((time + timeout) > millis())
	{
		while (gsm.available())
		{
			char c = gsm.read();
			rd += c;
		}
	}
	if (debug)
		Serial.print(rd);
	return rd;
}

void getGPSData()
{
	const int timeout = 1000;
	gsm.println("AT+CGNSINF");
	long int time = millis();
	int i = 0;

	while ((time + timeout) > millis())
	{
		while (gsm.available())
		{
			char c = gsm.read();
			if (c != ',')
			{
				gpsInfo[i] += c;
				delay(50);
			}
			else
			{
				i++;
			}
			if (i == 4)
			{
				delay(10);
				goto exitL;
			}
		}
	}
exitL:
	utc = gpsInfo[2];
	lat = gpsInfo[3];
	lon = gpsInfo[4];
	gpsInfo[2] = "";
	gpsInfo[3] = "";
	gpsInfo[4] = "";
}
