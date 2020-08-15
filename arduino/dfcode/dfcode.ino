#include "OneButton.h"
#include "SoftwareSerial.h"
SoftwareSerial mySerial(9, 10);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

# define ACTIVATED LOW

int buttonPause = 6;
int volume = 15;
boolean isPlaying = false;

OneButton button7(7, true);
OneButton button5(5, true);



void setup () {

  button7.attachLongPressStop(ButtonNext);            // link the function to be called on a doubleclick event.
  button7.attachClick(VolumeUp);                  // link the function to be called on a singleclick event.
  
  button5.attachLongPressStop(ButtonBack);            // link the function to be called on a doubleclick event.
  button5.attachClick(VolumeDown);                  // link the function to be called on a singleclick event.  
      
pinMode(buttonPause, INPUT);
digitalWrite(buttonPause,HIGH);

mySerial.begin (9600);
delay(1000);
playFirst();
isPlaying = true;


}


void loop () {
  button7.tick();
  button5.tick();
  delay(10); 

 if (digitalRead(buttonPause) == ACTIVATED)
  {
    if(isPlaying)
    {
      pause();
      isPlaying = false;
    }else
    {
      isPlaying = true;
      play();
    }
  }
}

 

void ButtonNext(){
  if(isPlaying)
    {
      playNext();
    }
}

void VolumeUp(){
  volumeINC();
}

void ButtonBack(){
  if(isPlaying)
    {
      playPrevious();
    }
}

void VolumeDown(){
  volumeDEC();
}
void playFirst()
{
execute_CMD(0x3F, 0, 0);
delay(500);
execute_CMD(0x06, 0, volume);
delay(500);
execute_CMD(0x11,0,1);
delay(500);
}


void pause()
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1); 
  delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);
  delay(500);
}

void playPrevious()
{
  execute_CMD(0x02,0,1);
  delay(500);
}

void volumeINC()
{
volume = volume+1;
if(volume==31)
{
volume=30;
}
execute_CMD(0x06, 0, volume);
delay(500);
}
void volumeDEC()
{
volume = volume-1;
if(volume==-1)
{
volume=0;
}
execute_CMD(0x06, 0, volume);
delay(500);
}



void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}
