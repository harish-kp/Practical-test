#!/usr/bin/env python

import rospy, tf
import geometry_msgs.msg, nav_msgs.msg, sensor_msgs.msg
from math import *
from time import sleep

def huskyOdomCallback(message,cargs):
    # Implementation of proportional position control 

    # Callback arguments 
    
    pub,dist_obstacle,msg,goal = cargs

    # Tunable parameters
    wgain = 10.0 # Gain for the angular velocity [rad/s / rad]
    vconst = 2 # Linear velocity when far away [m/s]
    distThresh = 0.1 # Distance treshold [m]

    # Generate a simplified pose
    pos = message.pose.pose
    quat = pos.orientation
    # From quaternion to Euler
    angles = tf.transformations.euler_from_quaternion((quat.x,quat.y,
                                                       quat.z,quat.w))
    theta = angles[2]
    pose = [pos.position.x, pos.position.y, theta]  # X, Y, Theta 
    print (pose)
    # Proportional Controller
    v = 0 # default linear velocity
    w = 0 # default angluar velocity
    distance = sqrt((pose[0]-goal[0])**2+(pose[1]-goal[1])**2)
    if (distance > distThresh and dist_obstacle > 0.5):
        v = vconst
        desireYaw = atan2(goal[1]-pose[1],goal[0]-pose[0])
        u = desireYaw-theta
        bound = atan2(sin(u),cos(u))
        w = min(0.25 , max(-0.5, wgain*bound))
    elif (distance > distThresh and dist_obstacle < 0.5):
        v = -0.15
        desireYaw = atan2(goal[1]-pose[1],goal[0]-pose[0])
        u = desireYaw-theta
        bound = atan2(sin(u),cos(u))
        w = 0.25
    elif (distance > 15 and dist_obstacle > 0.5):
        v = vconst
        w = 0.0
    else:
        v=0
        w=0
        print ('Goal reached')  
    # Publish   
    msg.linear.x = v
    msg.angular.z = w
    pub.publish(msg)
    
    # Reporting
    print('huskyOdomCallback: x=%4.1f,y=%4.1f dist=%4.2f, cmd.v=%4.2f, cmd.w=%4.2f, dist_obst=%4.2f'%(pose[0],pose[1],distance,v,w,dist_obstacle))
def laserCallback(msg):

    dist_obstacle = msg.ranges[360]
    rospy.Subscriber('odometry/filtered',nav_msgs.msg.Odometry,huskyOdomCallback, 
                 (cmdpub,dist_obstacle,cmdmsg,goal))


# Initialize our node
rospy.init_node('navigation_husky',anonymous=True)
dist_obstacle = 0    
# Set waypoint for Husky to drive to
goal = [1,0]  # Goal

# Setup publisher
cmdmsg = geometry_msgs.msg.Twist()
cmdpub = rospy.Publisher('/cmd_vel',geometry_msgs.msg.Twist, queue_size=10)
rospy.Subscriber('/scan',sensor_msgs.msg.LaserScan, laserCallback)
rospy.spin()