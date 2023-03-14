/**
 * @file offb_node.cpp
 * @brief Offboard control example node, written with MAVROS version 0.19.x, PX4 Pro Flight
 * Stack and tested in Gazebo SITL
 * 
 * from https://docs.px4.io/master/en/ros/mavros_offboard.html
 */

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/PositionTarget.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/GlobalPositionTarget.h>

geometry_msgs::Point current_position;
// mavros_msgs::PositionTarget pose_vel;

void state_cb(const geometry_msgs::PoseStamped::ConstPtr& msg){
    current_position.x = (*msg).pose.position.x;
    current_position.y = (*msg).pose.position.y;
    current_position.z = (*msg).pose.position.z;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<geometry_msgs::PoseStamped>("mavros/local_position/pose", 10, state_cb);
    //ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
    //ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    //ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
    ros::Publisher local_pos_pub_mavros = nh.advertise<mavros_msgs::PositionTarget>("mavros/setpoint_raw/local", 5);

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    //while(ros::ok() && !current_state.connected){
    //    ros::spinOnce();
    //    rate.sleep();
    //}
    mavros_msgs::PositionTarget pose;
    //geometry_msgs::PoseStamped pose;
    pose.position.x = 0;
    pose.position.y = 0;
    pose.position.z = 1;
    pose.type_mask = pose.IGNORE_VX | pose.IGNORE_VY | pose.IGNORE_VZ | pose.IGNORE_AFZ | pose.IGNORE_AFY | pose.IGNORE_AFX;
    pose.coordinate_frame = 1;
    pose.yaw = 3.141592/2;

    //send a few setpoints before starting
    //for(int i = 200; ros::ok() && i > 0; --i){
    //    local_pos_pub.publish(pose);
    //    ros::spinOnce();
    //    rate.sleep();
    //}


   
    //pose_vel.coordinate_frame = pose_vel.FRAME_LOCAL_NED;
    //pose_vel.type_mask =  pose_vel.IGNORE_AFX | pose_vel.IGNORE_AFY | pose_vel.IGNORE_AFZ | pose_vel.FORCE | pose_vel.IGNORE_YAW | pose_vel.IGNORE_PX | pose_vel.IGNORE_PY | pose_vel.IGNORE_PZ;


    //mavros_msgs::SetMode offb_set_mode;
    //offb_set_mode.request.custom_mode = "OFFBOARD";

    //mavros_msgs::CommandBool arm_cmd;
    //arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();

    int count = 0;
    geometry_msgs::Point t_point_1;
    t_point_1.x=0;
    t_point_1.y=0;
    t_point_1.z=10;
    geometry_msgs::Point t_point_2;
    t_point_2.x=10;
    t_point_2.y=0;
    t_point_2.z=10;
    geometry_msgs::Point t_point_3;
    t_point_3.x=10;
    t_point_3.y=0;
    t_point_3.z=25;
    geometry_msgs::Point t_point_4;
    t_point_4.x=10;
    t_point_4.y=5;
    t_point_4.z=25;
    
    pose.position=t_point_1;
    

    while(ros::ok()){
        //if( current_state.mode != "OFFBOARD" &&
        //    (ros::Time::now() - last_request > ros::Duration(5.0))){
        //    if( set_mode_client.call(offb_set_mode) &&
        //        offb_set_mode.response.mode_sent){
        //        ROS_INFO("Offboard enabled");
        //    }
        //    last_request = ros::Time::now();
        //} else {
        //    if( !current_state.armed &&
        //        (ros::Time::now() - last_request > ros::Duration(5.0))){
        //        if( arming_client.call(arm_cmd) &&
        //            arm_cmd.response.success){
        //            ROS_INFO("Vehicle armed");
        //        }
        //       last_request = ros::Time::now();
        //    }
        //}
        if( (ros::Time::now() - last_request > ros::Duration(3.0)) &&
        sqrt(pow(current_position.x-t_point_1.x,2)+pow(current_position.y-t_point_1.y,2)+pow(current_position.z-t_point_1.z,2))<0.02){
            pose.position=t_point_2;
            last_request = ros::Time::now();


        }
        else if((ros::Time::now() - last_request > ros::Duration(3.0)) &&
        sqrt(pow(current_position.x-t_point_2.x,2)+pow(current_position.y-t_point_2.y,2)+pow(current_position.z-t_point_2.z,2))<0.02){
            pose.position=t_point_3;
            last_request = ros::Time::now();
        }
        else if((ros::Time::now() - last_request > ros::Duration(3.0))&& sqrt(pow(current_position.x-t_point_3.x,2)+pow(current_position.y-t_point_3.y,2)+pow(current_position.z-t_point_3.z,2))<0.02){
            pose.position=t_point_4;
            last_request = ros::Time::now();
        }
        else if((ros::Time::now() - last_request > ros::Duration(3.0)) && sqrt(pow(current_position.x-t_point_4.x,2)+pow(current_position.y-t_point_4.y,2)+pow(current_position.z-t_point_4.z,2))<0.02){
            pose.position=t_point_1;
            last_request = ros::Time::now();
        }

    //     if (count<100){
    //        pose.position.x = -2;
    //        pose.position.y = 0;
    //        pose.position.z = 3;
    //     }
    //     else if (count<200){
    //        pose.position.x = 2;
    //        pose.position.y = 0;
    //        pose.position.z = 3;
    //     }
	// else{
	// count=0;
    //     }
      
        //float phase = ((float)count/40);
        //pose_vel.header.stamp = ros::Time::now();
        //pose_vel.yaw_rate = 0;
        //pose_vel.velocity.x = 1.5*sin(phase);
        //pose_vel.velocity.y = 0; 
        //pose_vel.velocity.z = 0;
    
        //local_pos_pub_mavros.publish(pose_vel);

        local_pos_pub_mavros.publish(pose);

        ros::spinOnce();
        rate.sleep();
        // count++;
	// std::cout << count <<"     "<<current_position.x << std::endl;

    }

    return 0;
}
