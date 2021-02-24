#include <ros/ros.h>

#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionFeedback.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <string.h>
#include <map>
#include <nav_msgs/Odometry.h>
#include <math.h>

struct Pose
{
  double x, y, theta;
  std::string frame;
  
};

class goalNav
{
private:
  void callActionServer(move_base_msgs::MoveBaseGoal goal);
  void getGoals();
  void setGoals(Pose final_pose);
  void run(int status);
  
  int goal_count;
  bool goal_reached ,goal_sended, operation_started;

  
  double real_start_time, real_end_time;
  double new_pose_x , new_pose_y , old_pose_x , old_pose_y, dist;

  ros::NodeHandle nh_;
  ros::Subscriber sub;
  ros::Subscriber sub2;
  ros::Subscriber odom_sub;
  move_base_msgs::MoveBaseGoal goal;
  move_base_msgs::MoveBaseGoal goal2;
  
  int check_status(int status);
  double get_start_time(double start_time);
  double get_end_time(double end_time);
  double calculate_distance(double curr_pos, double last_pos);
  int goal_status;
  

public:
  void resultCallback(const actionlib_msgs::GoalStatusArray::ConstPtr &msg);
  void odomCallback(const nav_msgs::Odometry::Ptr &msg);
  goalNav(ros::NodeHandle nh);
  ~goalNav();
};

goalNav::goalNav(ros::NodeHandle nh)
{
  getGoals();
  odom_sub = nh.subscribe("/husky_velocity_controller/odom",1,&goalNav::odomCallback,this);
  sub = nh.subscribe("/move_base/status",1,&goalNav::resultCallback,this);
  goal_count = 0;
  goal_status = 0;
  operation_started = false;
  old_pose_x = 0.0; old_pose_y = 0.0;
  dist = 0;
}

goalNav::~goalNav()
{
}

void goalNav::run(int status)
{
  
  if (status == 3 && goal_count == 0) { // this is the first stage 
    
    goal_reached = false;
    callActionServer(goal);
    goal_count = goal_count + 2;
    ROS_INFO("Moving to goal");
    operation_started = false;
  } 
  if (status == 3){
      ros::shutdown();
  }
  else
  {
    //do nothing
  }
  
  
}

int goalNav::check_status(int status)
{
  goal_status = status;
  if (goal_status == 1) {
    goal_sended = true;
    goal_reached = false;
  }
  if (goal_status == 3)
  {
    goal_reached = true;
    goal_sended = false;
  }
  run(goal_status);
  return goal_status;

}
double goalNav::get_start_time(double start_time)
{
  
  if (start_time > 0) {
    real_start_time = start_time;
    operation_started = true;
    std::cout << "real start time is :" << real_start_time << std::endl;
    return real_start_time;
    
  }
  
}
double goalNav::get_end_time(double end_time){

  if (end_time > 0) {
    real_end_time = end_time;
    std::cout << "real end time is :" << real_end_time << std::endl;
    return real_end_time;
    
  }

}

void goalNav::odomCallback(const nav_msgs::Odometry::Ptr & msg)
{

  if (operation_started) {
  new_pose_x = msg->pose.pose.position.x;
  new_pose_y = msg->pose.pose.position.y;
  double diff_x , diff_y;

  diff_x = new_pose_x - old_pose_x;
  diff_y = new_pose_y - old_pose_y;
  dist = calculate_distance(diff_x,diff_y) + dist;
  old_pose_x = new_pose_x; old_pose_y = new_pose_y;
  
  }
}

double goalNav::calculate_distance(double diff_x, double diff_y)
{
  // calculate the distance 
  double result = hypot (diff_x, diff_y);
  return result;
}

void goalNav::resultCallback(const actionlib_msgs::GoalStatusArray::ConstPtr &msg){
// check if goal is reached 

int goal_stat;
if (msg->status_list.empty()) {
  goal_stat = 3;
}
else
{
  goal_stat = msg->status_list[0].status;
}

check_status(goal_stat);
double start_time , finish_time;

if (goal_count == 1)
		{
			// status is not clear, no goal is sended yet!
			start_time = msg->header.stamp.toSec();
      get_start_time(start_time);
		}
if ( goal_count == 2)
    {
      finish_time = msg->header.stamp.toSec(); 
      get_end_time(finish_time);
      double total_time = abs(real_start_time - real_end_time);
      ROS_INFO("total time = %f",total_time);
      goal_count = goal_count + 1;
    }
    
}


void goalNav::getGoals()
{
    Pose final_pose;
    final_pose.x = 5.0;
    final_pose.y = 2.0;
    final_pose.theta = 1.0;
    final_pose.frame = "map";
    setGoals(final_pose);
  
}

void goalNav::setGoals(Pose final_pose)
{
  geometry_msgs::PoseStamped target_pose;

  //we'll send a goal to the robot 
  goal.target_pose.header.frame_id = final_pose.frame;
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = final_pose.x;
  goal.target_pose.pose.orientation.w = final_pose.theta;
  std::cout << "first goal is :  " << final_pose.x << ", " << final_pose.y << ", " << final_pose.theta << ", " << final_pose.frame << std::endl;

}

void goalNav::callActionServer(move_base_msgs::MoveBaseGoal goal)
{
  typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
   MoveBaseClient ac("/move_base", true);

  while(!ac.waitForServer(ros::Duration(5.0))){
  ROS_INFO("Waiting for the move_base action server to come up");
    
  }
  //we'll send a goal to the robot the goal we get from previous function
  ac.sendGoal(goal);
  
  ROS_INFO("Sending goal");
  
  
}

int main(int argc, char** argv){
    ros::init(argc,argv,"multi_goal_driver");
    ros::NodeHandle nh;
    goalNav goalNav(nh);  
    ros::Rate rate(5);
    ros::spin();

    return 0;

}



