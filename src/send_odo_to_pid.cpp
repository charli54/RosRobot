#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

double R_spd, L_spd;
ros::Publisher right_wheel_pub;

void publish_spd(const ros::TimerEvent &event){
//	right_wheel_pub.publish(R_spd);
}

void listen_to_odo(const geometry_msgs::Twist::ConstPtr& msg){
    R_spd = (const double)msg->linear.x;
    //L_spd = msg.linear.y;
	ROS_INFO("I heard: [%s]", msg->linear.x);

    publish_spd();
    //right_wheel_pub.publish(R_spd);
}

int main(int argc, char** argv){

	ros::init(argc, argv, "send_odo_to_pid");

	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("ard_odo", 1000, listen_to_odo);
	right_wheel_pub = n.advertise<std_msgs::Float64>("right_wheel_spd", 50);
	//ros::Publisher left_wheel_pub = n.advertise<std_msgs::Float64>("left_wheel_spd", 50);
  	
    ros::spin(); 

	return 0;
}