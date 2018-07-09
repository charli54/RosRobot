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

double velTarget[2] = {0,0};

//Publisher used to publish odometry from encoder
ros::NodeHandle nh;
geometry_msgs::Twist odoMsg;
ros::Publisher pub("ard_odo", &odoMsg);

geometry_msgs::Twist spdMsg;
ros::Publisher info("ard_spd", &spdMsg);

//suscriber used to sucribe to cmd_vel comming from the navigation stack
//suscriber callback function
void getVelocity(const geometry_msgs::Twist& CVel){
    double vel_x = CVel.linear.x;
    double vel_th = CVel.angular.z;

    velTarget[0] = vel_x;
    velTarget[1] = vel_th;   
}
//the suscriber
ros::Subscriber<geometry_msgs::Twist> Sub("r2d2_diff_drive_controller/cmd_vel", &getVelocity );

void setup() {
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
  lastTime[0] = millis();
  nh.initNode();
  nh.advertise(pub);
  nh.advertise(info);
  nh.subscribe(Sub);
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

  spdMsg.linear.x = velTarget[0];
  spdMsg.linear.y = velTarget[1];
  
  info.publish(&spdMsg);
  
  analogWrite(motorPin, int(velTarget[0] * 100));
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
