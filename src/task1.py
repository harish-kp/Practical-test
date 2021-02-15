#!/usr/bin/env python

import rospy, tf
import geometry_msgs.msg, nav_msgs.msg
from math import *
from time import sleep
from createDaemon import createDaemon
def huskyOdomCallback(message,cargs):

    # Callback arguments 
    pub,msg,call = cargs
    default_v = 0 # default linear velocity
    default_w = 0 # default angluar velocity
    move_w = 0.5
    if call == 1 :
        print (" w = 0 ")
        msg.linear.x = default_v
        msg.angular.z = default_w
        pub.publish(msg)
    msg.linear.x = default_v
    msg.angular.z = move_w
    pub.publish(msg)
def callback(event):
    print("Rotated 10 sec using python")
    rospy.signal_shutdown("Rotated 10 secs")  
    
call = 0  
createDaemon()
rospy.init_node('navigation_husky',anonymous=True)
if call == 0:
    rospy.Timer(rospy.Duration(10), callback)
    cmdmsg = geometry_msgs.msg.Twist()
    cmdpub = rospy.Publisher('/cmd_vel',geometry_msgs.msg.Twist, queue_size=10)
    rospy.Subscriber('odometry/filtered',nav_msgs.msg.Odometry,huskyOdomCallback,(cmdpub,cmdmsg,call))
    rospy.spin()
else :
    cmdmsg = geometry_msgs.msg.Twist()
    cmdpub = rospy.Publisher('/cmd_vel',geometry_msgs.msg.Twist, queue_size=10)
    rospy.Subscriber('odometry/filtered',nav_msgs.msg.Odometry,huskyOdomCallback,(cmdpub,cmdmsg,call)) 
    rospy.signal_shutdown()