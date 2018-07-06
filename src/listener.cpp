#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <iostream>

using namespace std;

void printData(const geometry_msgs::Twist::ConstPtr& twist_msg){
    cout << "Speed :" << twist_msg->linear.x << "\n";
    ROS_INFO("I heard: [%s]", twist_msg->linear.x);
}

int main(int argc, char** argv){
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("ard_odo", 1000, printData);
    ros::spin();
    return 0;
}
