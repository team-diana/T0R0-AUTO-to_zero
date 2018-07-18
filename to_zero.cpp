#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>


class odom 
{
  public:
    float x;
    float y;
    float z;
    float xo;
    float yo;
    float zo;
    float wo;

    void callback(const nav_msgs::Odometry::ConstPtr& msg);
};


void odom::callback(const nav_msgs::Odometry::ConstPtr& msg)
{
  //ROS_INFO("Seq: [%d]", msg->header.seq);

	 x=msg->pose.pose.position.x;
	 y=msg->pose.pose.position.y;
	 z=msg->pose.pose.position.z;
	 xo=msg->pose.pose.orientation.x;
	 yo=msg->pose.pose.orientation.y;
	 zo=msg->pose.pose.orientation.z;
	 wo=msg->pose.pose.orientation.w;  
//ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]\n", x,y,z);
//return msg->pose.pose.position.x;  
return ;
}


class path
{
  public:
    float x[];
    float y[];
    float z[];

    void callback(const nav_msgs::Path::ConstPtr& msg);
};


void path::callback(const nav_msgs::Path::ConstPtr& msg)
{
  ROS_INFO("Seq: [%d]", msg->header.seq);
  int i;
  float a,b,c;
  while(a!=0.00 && b!=0.00 && c!=0.00)
{        printf("12\n");
	 x[i]=msg->poses[i].pose.position.x;
	 a=x[i]*10;
	ROS_INFO("Position-> x: [%f]\n", x[i]);
	 y[i]=msg->poses[i].pose.position.y;
	b=y[i]*10;
	 z[i]=msg->poses[i].pose.position.z;
	c=z[i]*10;
	 i++;
};

//ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]\n", x,y,z);
//return msg->pose.pose.position.x;  

}




int main(int argc, char **argv)
{

  ros::init(argc, argv, "to_zero");

  ros::NodeHandle n;
  ros::Rate loop_rate(30); 
  odom odominfo;

  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/rtabmap/odom", 1, &odom::callback, &odominfo);
  path pathinfo;
  sub = n.subscribe<nav_msgs::Path>("/rtabmap/local_path", 1, &path::callback, &pathinfo);

  while (ros::ok()) 
    {
        ros::spinOnce();
        //ROS_INFO("x: %f", odominfo.x);
        //ROS_INFO("y: %f", odominfo.y);
        //ROS_INFO("z: %f", odominfo.z);
        loop_rate.sleep();
    }
}


