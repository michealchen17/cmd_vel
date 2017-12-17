/**     2017-12-17
 *      michealchen     michealchen17@163.com
 *
 * this node can subsrcibe topic teleop published from kb_teleop node.
 * and it can publish geometry_msgs::twistStamped msg as topic cmd_vel.
 * this is velocity command publish node used to drive a mobile robot to move!
 * only for x linear velocity and z angular velocity
 * goodluck!
 * */
#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>
#include <sensor_msgs/Joy.h>
//system configure. this will be set as param,so it can be asinged through launch file
const double vel_l_max = 1;                 //maxmum linear velocity    m/s
const double vel_a_max = 1;                 //maxmum angular velocity   rad/s
const double vel_l_acc = 0.01;              //linear velocity acceleration
const double vel_a_acc = 0.01;              //angular velocity acceleration

ros::Publisher pub;
geometry_msgs::TwistStamped lastVel;
unsigned int msg_seq = 0;

void teleopCallback(const sensor_msgs::Joy &msg)
{
    if( (msg.buttons[0]==1) && (lastVel.twist.linear.x<vel_l_max))
    {
        lastVel.twist.linear.x += vel_l_acc;
    }
    if( (msg.buttons[1]==1) && (lastVel.twist.linear.x>-vel_l_max))
    {
        lastVel.twist.linear.x -= vel_l_acc;
    }
    if( (msg.buttons[2]==1) && (lastVel.twist.angular.z<vel_a_max))
    {
        lastVel.twist.angular.z += vel_a_acc;
    }
    if( (msg.buttons[3]==1) && (lastVel.twist.angular.z>-vel_a_max))
    {
        lastVel.twist.angular.z -= vel_a_acc;
    }

    pub.publish(lastVel);
    msg_seq++;
}




int main(int argc, char** argv)
{
    ros::init(argc, argv, "cmd_vel");
    ros::NodeHandle nh;

    pub = nh.advertise<geometry_msgs::TwistStamped>("/cmd_vel",1);
    lastVel.header.frame_id = "base_link";
    lastVel.header.seq = msg_seq;
    lastVel.header.stamp = ros::Time::now();
    lastVel.twist.linear.x = 0;
    lastVel.twist.linear.y = 0;
    lastVel.twist.linear.z = 0;
    lastVel.twist.angular.x = 0;
    lastVel.twist.angular.y = 0;
    lastVel.twist.angular.z = 0;

    ros::Subscriber sub = nh.subscribe("kb_teleop", 1, teleopCallback);
    ros::spin();
    return 0;
}

