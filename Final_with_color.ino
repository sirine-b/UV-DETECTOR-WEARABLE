
//call the libraries required to run the code
#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include "Adafruit_SI1145.h"

Adafruit_SI1145 uv = Adafruit_SI1145();

//Initialise the variables in the same order they were used throughout the code 
float result;
float Clouds = 1.00066;
float Elevation = 0.89;
float lambda;
float c=3E8;
//h corresponds to Planck's constant which is 6.626E-34, however the smallest number that can be computed on ARDUINO IDE 
//is to the power of -26, any number smaller than this would be rounded to 0 and thus falsify the result of the calculation in which h is used 
//(i.e. it would be always equal to zero) 
float h=6.626E-26; 
float energy;
float ENERGY;
float totalenergy;
float MAXENERGY = 0.5;

//const int speaker = 5;
//const int rightButton = 19;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  
  Serial.println("Adafruit SI1145 test");
  
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }

  Serial.println("OK!");

//  //define the speaker on pin 5 as the output
//  pinMode(5, OUTPUT);
//
//  //define the button on pin 19 (right button) as the input
//  pinMode(19, INPUT_PULLUP);
}

void loop() {
  
//store the value of the UV index measured by the sensor in the variable 'UVindex'
  float UVindex = uv.readUV();

//the measured index is automatically multiplied by 100 so  divide by 100 in order to get the integer index
  UVindex /= 100.0;  
//  Serial.print("UV: ");  Serial.println(UVindex);

// Calculate the  result based on the obtained value of UV index as well as on the clouds and elevation parameter set by the user
  result= (UVindex*25)/(Clouds*Elevation);
//  Serial.print("result: "); Serial.println(result);
  
  delay(1000);
//Determines the wavelength corresponding to the measured UV index based on the range to which the obtained result belongs
  if (result>0 && result<10){
  lambda = 0;

// Serial.print("lambda: "); Serial.println(lambda);
}
  else if (result>=10 && result<75) {
    lambda = 400;
//    Serial.print("lambda: "); Serial.println(lambda);
  }
  else if (result>=75 && result<150){
    lambda = 290;
//    Serial.print("lambda: "); Serial.println(lambda);
}
  else {
  lambda = 320;
//  Serial.print("lambda: "); Serial.println(lambda);
}

//Calculate the energy of the measured UV index using the obtained corresponding lambda value
 
  //Energy was set to 0 in case lambda = 0, 
  //=> if lamda=0 there is no light and thus no energy
  //=> also doing this prevents a maths error from arising in the used equation due to a division by 0
  if (lambda==0){ 
    totalenergy= totalenergy+ 0; //if 
    Serial.print("totalenergy: "); Serial.println(totalenergy);
//    Serial.print("Condition1");
  }
  else {
    energy =(h*c)/lambda*pow(10,-9);
    //the calculated value of energy is in the range of E-11. It being small, the code rounds it to 0 which would interfere with calculation
    //and lead to an energy value that is always equal to 0
    //=> thus, the calculated energy is multiplied by E11.
    ENERGY = energy*pow(10,11);
    Serial.print("ENERGY=");Serial.println(energy);
    delay(1000);

    // increments (sums up) the calculated energy values at each loop so as to obtain the value of the total energy the user was exposed 
    //to throughout the day not just value of energy exposure at one specific moment of time
    
    totalenergy=totalenergy+energy;
    Serial.print("totalenergy: "); Serial.println(totalenergy);
//    Serial.print("Condition2");
  }



// Once the maximum threshold has been reached the circuit playground beeps to remind user that their maximum has been reached

 if (totalenergy>=MAXENERGY){
    CircuitPlayground.playTone(200,1000); 
    
}

//Calculate the percentage of energy to which the user has been exposed to based on the maximum energy

int totalpercentage=(totalenergy/MAXENERGY)*100;
Serial.print(" the total percentage is: ");
Serial.println(totalpercentage);

//Compares the current percentage every 10 percent and once that value is reached 
// a neo pixel illuminates (e.g. once the user has been exposed to 10% of their max threshold, a green light shines at neopixel 0
// note : the choice of colors was studied and based on the colors used to represent the UV indexes

if (totalpercentage<=10){
  
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(0,   0, 255,   0);
 
}
else if (totalpercentage<=20){
  CircuitPlayground.setPixelColor(1,   0, 255,   0);
 
}
else if (totalpercentage<=30){
  CircuitPlayground.setPixelColor(2,250,227,0);
 
}
else if (totalpercentage<=40){
  CircuitPlayground.setPixelColor(3,250,227,0);
 
}
else if (totalpercentage<=50){
  CircuitPlayground.setPixelColor(4,250,100,0);
 
}
else if (totalpercentage<=60){
  CircuitPlayground.setPixelColor(5,250,100,0);
 
}
else if (totalpercentage<=70){
  CircuitPlayground.setPixelColor(6, 255,   0,   0);
 
}
else if (totalpercentage<=80){
  CircuitPlayground.setPixelColor(7, 255,   0,   0);
 
}
else if (totalpercentage<=90){
  CircuitPlayground.setPixelColor(8,162,0,191);
 
}
else if (totalpercentage<=100){
  CircuitPlayground.setPixelColor(9,162,0,191);
 
}


}
