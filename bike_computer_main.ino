//Bike computer project

#define WHEEL_CIR_IN_MM 2155.0
#define NUM_OF_TIME_SAMPLES 4

short led = 13;
short mag_switch = 2;
short switch_made;
short inc_count = true;
unsigned long mm = 0;
unsigned long meters = 0;
unsigned int kmeters = 0;
float miles = 0.0;
float dd_dt = 0.0;
float mph = 0.0;
unsigned int time_sample[]={0,0,0,0};
unsigned long last_time = 0;
unsigned long this_time = 0;
unsigned int avg_time = 0;
unsigned int numerator = 0;

short imperial = false;
//short imperial = true;
//short sim_pulse = false;
short sim_pulse = true;
int sim_delay = 500;

void setup(){
  pinMode(led, OUTPUT);
  pinMode(mag_switch, INPUT);
  Serial.begin(9600);
}

void loop(){
  this_time = millis();
  if((this_time - last_time) > 10000)
  {
    dd_dt = 0.0;
    for(int i = 0;i < NUM_OF_TIME_SAMPLES; i++)
      {
        time_sample[i] = 0;
      }
  }
  numerator = 0;
  switch_made = digitalRead(mag_switch);
  digitalWrite(led, switch_made);
  if(switch_made || sim_pulse)
  {
    if(inc_count)
    {
      mm = mm+WHEEL_CIR_IN_MM;
      inc_count = false;
      
      //create and use a moving average to use for
      //instantanious speed
      for(int i = 0;i < NUM_OF_TIME_SAMPLES -1;i++)
      {
        time_sample[i] = time_sample[i+1];
      }
      time_sample[NUM_OF_TIME_SAMPLES-1] = this_time-last_time;

      for(int i = 0;i < NUM_OF_TIME_SAMPLES; i++)
      {
        numerator = numerator+time_sample[i];
      }
      avg_time = numerator/NUM_OF_TIME_SAMPLES;
      dd_dt = WHEEL_CIR_IN_MM/avg_time;//speed in mm/ms (m/s)
      dd_dt = dd_dt*3.6;// speed in km/hr

      last_time = this_time;
    }
  }
  else
    inc_count = true;
 
  while(mm > 1000)
  {
    mm = mm - 1000;
    meters++;
  }
  
  if(meters > 1000)
  {
    kmeters++;
    meters = meters - 1000;
  }
  if(sim_pulse)
  {
    delay(sim_delay);
    if(millis() >= 60000)
    {  
      inc_count = false;
      sim_pulse = false;
    }
    else
      inc_count = true;
  }
  if(imperial)
  {
    miles = (.0006*meters + (kmeters*1000));
    mph = .6214 * dd_dt;
    Serial.print("Miles: = ");
    Serial.println(miles);
    Serial.print("MPH: = ");
    Serial.println(mph);
  }
  else
  {
    Serial.print("kmeters = ");
    Serial.print(kmeters);
    Serial.print(", Meters = ");
    Serial.print(meters);
    Serial.print(", mm = ");
    Serial.println(mm);
    Serial.print("km/hr = ");
    Serial.println(dd_dt);
  }
  if (Serial.available() > 0 ) 
  {
    // read the incoming byte:
    Serial.read();
    imperial = !imperial;    
   }
}  
