# Practical-test
Status : In Development 
Completed : Task 1
In progress : Task 2
Last update date : 17 February 2021
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
Development of python code to move to a goal and avoid obstacles as it reaches it.
  
In progress:  

Fine tuning of controller to trace the path as Proportional controller is too aggressive as the goal reaching probability is about 60\%
Developing python and C++ code to read and write to csv file and make the robot to trace the path from the csv file.
