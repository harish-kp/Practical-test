# Practical-test
Status : Developed / Looking for improvement opportunites   
Completed : Task 1, Task 2  
Last update date : 24 February 2021    
Requirements : Ubuntu 18.04, ROS Melodic  
Language : C++, Python
# Task 1

Task 1 aims to rotate the Husky UGV in clockwise and anticlockwise directions for 10 seconds. The second portion of the task aims to daemonize the task to be executed in the background without any interference from the terminal.
  
Completed portion: 
I have written python code to rotate the Husky UGV for 10 seconds in anticlockwise direction. In order to execute the code create a package named "practice_test" and clone the repository's contents into the package. "task1.py" file in the repository corresponds to this task. With the updated CMakeList and package file, creating a package in catkin_ws and `catkin_make` the process will compile the c++ and make it into an executable named `task`
  
To execute the program, 
In 3 different terminals,  
    Execute `roscore`  
    Execute `roslaunch husky_gazebo husky_empty_world.launch`  
    Execute `rosrun practice_test task1.py` from `~/catkin_ws`  to run the python file (anticlockwise rotation)  
    Execute `rosrun practice_test task` from `~/catkin_ws` to execute the c++ (clockwise rotation)  
    The file has already been daemonized, it'll right away exit to default terminal.  
To review that the process was executed in the background, kindly use `ps -axl`, where you can lookup that \verbose{roslaunch husky\_gazebo husky\_empty\_world.launch} being executed for about 10 seconds and 12 seconds repectively.  
Linux's "man daemon" came in handy to understand what and necessity of services in linux.    

# Task 2 

Task 2 aims to read goal from a csv file and write the poses traced by the robot back to the same csv file.  
Completed portion:  
Development of python code to move to a goal and avoid obstacles as it reaches it. With some digging into my earlier ROS project notes, I was able to recreate and modify it for Husky and made it to map and navigate to any given goal avoiding obstacles. Kindly ensure that the files in the repository are placed as is in the newly created package practice_test.  
To execute the program,   
In different terminals,  
Execute `roscore`  
Execute `roslaunch husky_gazebo husky_playpen.launch`  
Execute `roslaunch husky_navigation exploration_demo.launch`  
Execute `rosrun practice_test task2_mapping.py`  
Execute `rosrun practice_test task2_control.py`  
Execute `rostopic echo imu/data -b ~/Desktop/task2.bag -p > ~/Desktop/task2.csv`  
In order to view the robot in simulation environment with intricate details about the pose and location of the obstacles,  
Execute `roslaunch husky_viz view_robot.launch`  
Kindly open `task2_control.py` to change the navigation goal in line 523 in second and third arguments of the function control.goToPosition() representing x and y coordinates of the goal point respectively.  
The Husky can be allowed to freely explore the environment by uncommenting line 522 and then go to any goal point.  
The mapping of the environment invokes two services named `Centroid.srv` and `Trajectory.srv`. The launch file `exploration_demo.launch` invokes two important topics `base_link` and `map` which are necessary for mapping and keeping track of the robot relative to world frame.
ROSbag is convenient way to echo topics and replay them to move the robot in the same path along with CSV for data analytics.    
C++ code execution:  
In `task2.cpp`, one can alter the goal that is to be navigated in `goalNav::getGoals()` function. If the spawn location is changed from default values, please give in the spawn location in `goalNav::goalNav(ros::NodeHandle nh)` function. Upon executing `catkin_make`, executable named `tasktwo` is created and compiles the code.  
To execute the program,     
In different terminals,  
Execute `roslaunch husky_gazebo husky_playpen.launch`  
Execute `roslaunch husky_navigation exploration_demo.launch`  
Execute `rosrun practice_test tasktwo`  
Execute `rostopic echo imu/data -b ~/Desktop/tasktwo.bag -p > ~/Desktop/tasktwo.csv`  
In order to view the robot in simulation environment with intricate details about the pose and location of the obstacles,  
Execute `roslaunch husky_viz view_robot.launch`  
  
# References and Useful links  
[ROS Melodic Installation](http://wiki.ros.org/melodic/Installation)  
[ROS Tutorials](http://wiki.ros.org/ROS/Tutorials)  
[ROS Husky Tutorials](http://wiki.ros.org/Robots/Husky)  
[Clearpath Robotics Husky Datasheet](https://www.clearpathrobotics.com/wp-content/uploads/2013/02/HUSKY_A200_UGV_2013_TEASER_email.pdf)  
R Siegwart, I R Nourbaksh,Introduction to Autonomous Mobile Robots, MIT Press, 2004.  
R Hartley, A Zisserman, Multiple View Geometry in Computer Vision, Second ed. Cambridge University Press, 2004.  
R. Szeliski, Computer Vision: Algorithms and Applications. 2010.  