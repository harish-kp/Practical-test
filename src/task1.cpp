#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

static void daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE); }
int main(int argc, char** argv)
{
  //Daemon function call
  daemon();
  //init the ROS node
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;

  //init publisher
  ros::Publisher cmd_vel_pub_;
  cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    
  //init direction that husky should go
  geometry_msgs::Twist base_cmd;
  geometry_msgs::Twist base_cmd_turn_right; //Clockwise rotation msg definition

  base_cmd.linear.x = 0;
  base_cmd.linear.y = 0;
  base_cmd.angular.z = 0;
  base_cmd_turn_right.linear.x = 0; 
  base_cmd_turn_right.linear.y = 0;
  base_cmd_turn_right.angular.z = 0;

  //base_cmd_turn_right will be used to turn husky 90 degrees
  base_cmd_turn_right.linear.x = 0; //m/s
  base_cmd_turn_right.angular.z = -1.57/2; 

  ros::Rate rate(5); // 5Hz
  while(nh.ok()) { 
    //"publish" sends the command to husky to keep going
    for(int n=50; n>0; n--) {
      cmd_vel_pub_.publish(base_cmd_turn_right);
      rate.sleep();
    }
    break;
  }


  base_cmd.linear.x = 0;
  base_cmd.linear.y = 0;
  base_cmd.angular.z = 0;
  cmd_vel_pub_.publish(base_cmd);

  return 0;
}