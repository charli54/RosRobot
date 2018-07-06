#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <Encoder.h>

#define ENCODER_A 2
#define ENCODER_B 3
#define motorPin 11

float wheelDistance = 0.2;
float wheelDiameter = 0.06;
int CPR = 1456;
int Imax = 300;
int accParam = 3;

int odoWait = 3;
int odoCount = 0;

Encoder lEncoder(ENCODER_A, ENCODER_B);

long encoderVal[2]= {0,0};
double dDis[2] = {0,0};
long lastTime[2] = {0,0};

ros::NodeHandle nh;
geometry_msgs::Twist odoMsg;
ros::Publisher pub("ard_odo", &odoMsg);

void setup() {
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
  lastTime[0] = millis();
  nh.initNode();
  nh.advertise(pub);
}

void loop() {
  double pubSpeed = getSpeed();
  nh.spinOnce();
  if(odoCount > odoWait){
    odoMsg.linear.x = pubSpeed;
    odoMsg.linear.y = pubSpeed;

    pub.publish(&odoMsg);
  }
  else{odoCount++;}
  analogWrite(motorPin, 100);
  Serial.print("Speed: ");
  Serial.println(getSpeed());
  delay(100);
}

double getSpeed(){
  
  encoderVal[0] = lEncoder.read();
  lEncoder.write(0);

  long T = millis();
  int DTime = T - lastTime[0];
  lastTime[0] = T;

  //Vitesse angulaire
  double dAngle = encoderVal[0]*1.00/CPR;   //en RPM
  double omega = (dAngle/DTime)*1000*60;

  //Vitesse tangentielle
  dDis[0] = encoderVal[0]*3.14*wheelDiameter/CPR; 
  double eVel =  (dDis[0]/DTime)*1000;      //en m/s
  
  //On sauve la vitesse a publier ici

  
  return eVel;
}
