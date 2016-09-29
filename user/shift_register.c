#define CHANNELS    16
#define LOW          0
#define HIGH         1
#define DATA_PIN    13
#define LATCH_PIN   12
#define CLOCK_PIN   11
#define OE_PIN      10


unsigned int secuencias[]={
   10832,
   10576,
   9808,
   9552,
   6736,
   6480,
   5712,
   5456,
   6728,
   6472,
   10800,
   10544,
   9776,
   9520,
   6704,
   6448,
   5680,
   5424,
   6696,
   6440,
   10800,
   5416
}


void writeData(unsigned int D){
	unsigned int statusD=0;
	unsigned int i=0;

	for(i=0;i<CHANNELS;i++){
		statusD=D%2;
		D=D/2;
		Serial.write(statusD+'0');

		if(statusD==0)
		{
			digitalWrite(DATA_PIN, LOW);
		}
		else if(statusD==1)
		{
			digitalWrite(DATA_PIN, HIGH);
		}
		digitalWrite(CLOCK_PIN, HIGH);
		os_delay_us(1);
		digitalWrite(CLOCK_PIN, LOW);
		os_delay_us(1);      
		}
		digitalWrite(LATCH_PIN, HIGH);
		os_delay_us(1);
		digitalWrite(LATCH_PIN, LOW);
		os_delay_us(1);
}
