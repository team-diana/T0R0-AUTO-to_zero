/*
  This algorithm could be improved by following this tips about "Local Planner":

  http://wiki.ros.org/navigation/Tutorials/Navigation%20Tuning%20Guide
  http://wiki.ros.org/navigation/Tutorials/RobotSetup
  http://wiki.ros.org/base_local_planner

*/

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
#include <stdio.h>
#include <math.h>
#include <tf/tf.h>


class odom     // class to read data from the rostopic
{
  public:
    float x;
    float y;
    float z;  //since we are developing a 2-D path, this coordinate could be useless
    float xo;
    float yo;
    float zo;
    float wo;

    void callback(const nav_msgs::Odometry::ConstPtr& msg);
};


void odom::callback(const nav_msgs::Odometry::ConstPtr& msg)  // function to read data
{
  //ROS_INFO("Seq: [%d]", msg->header.seq);

	 x=msg->pose.pose.position.x;
	 y=msg->pose.pose.position.y;
	 z=msg->pose.pose.position.z; //same as above
	 xo=msg->pose.pose.orientation.x;
	 yo=msg->pose.pose.orientation.y;
	 zo=msg->pose.pose.orientation.z;
	 wo=msg->pose.pose.orientation.w;

   ROS_INFO("subodom-> x: [%f], y: [%f], z: [%f]\n", x,y,z);
   //return msg->pose.pose.position.x;
return ;
}

static void toEulerAngle(const Quaterniond& q, double& yaw) //TO CHECK Quaternion declaration: in "main" it's declared as tf::Quaternion
{

	// yaw (z-axis rotation), in 2-D that should be the only angle needed
	double siny = +2.0 * (q.w() * q.z() + q.x() * q.y()); //q.w, q.x, q.y, q.z, values should be changed according to Quaternion declaration
	double cosy = +1.0 - 2.0 * (q.y() * q.y() + q.z() * q.z());
	yaw = atan2(siny, cosy);

  //source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

}

class path
{
  public:
    float x[];
    float y[];
    float z[]; //since we are developing a 2-D path, these coordinates could be useless

    void callback(const nav_msgs::Path::ConstPtr& msg);
};


void path::callback(const nav_msgs::Path::ConstPtr& msg)  // here MAYBE WRONG,modify the while loop to get better
{

  ROS_INFO("Seq: [%d]", msg->header.seq);
  int i=0;
  float a,b,c;
  while(a!=0.00 && b!=0.00 && c!=0.00) // we should look for a regulation over "i" rather than wait to read a target point (0,0,0) that could never come
{
	 x[i]=msg->poses[i].pose.position.x;
	 a=x[i]*10;
	ROS_INFO("subpathpoint%d-> x: [%f]\n", i,x[i]);
	 y[i]=msg->poses[i].pose.position.y;
	b=y[i]*10;
	 z[i]=msg->poses[i].pose.position.z;
	c=z[i]*10;
	 i++;
};

}

class planclass             //store the next target
{
     private:
     int point;
     public:
     void nextpoint(int input);
     int nowpoint();

};


void planclass::nextpoint(int input)
{

   point =input;

}

int planclass::nowpoint()
{
 return point;
}


uint16_t FloatToUint(float n)              // convert the data to uint16
{
   return (uint16_t)(*(uint16_t*)&n);
}



void turn (float a)   // motor
{
	uint16_t left;
	uint16_t right;

  if (a>=0)
{
	float leftfloat=-a;
	float rightfloat=a;
	left = FloatToUint( leftfloat);
	right = FloatToUint( rightfloat);

  //it would be better to calculate an angular velocity value

  printf("%hu\n", left);
	printf("%hu\n", right);
}
else if (a<0)
{
  float leftfloat=a;
	float rightfloat=-a;
	left = FloatToUint( leftfloat);
	right = FloatToUint( rightfloat);

  //it would be better to calculate an angular velocity value

  printf("%hu\n", left);
	printf("%hu\n", right);
}

  /*
    distinction between counter and clockwise rotation accordingly to "a" sign
    is necessary to let the value reach "0" during rotation
  */

  //it is neccesary to add something to send the output values to motors
  // "geometry_msgs/Twist" and "cmd_vel" topics hold velocity values, CHANGES NEEDED

  /*
  this algorithm continously sends different values to motors, as they constantly reduces.
  it may be TOO MUCH STRESSFUL for Rover
  */

}


void go (float a)    //motor
{
	uint16_t left;
	uint16_t right;
	float leftfloat=a;
	float rightfloat=a;
	left = FloatToUint( leftfloat);
	rigth = FloatToUint( rightfloat);
  printf("%hu\n", left);
	printf("%hu\n", right);

  //it is neccesary to add something to send the output values to motors
  // "geometry_msgs/Twist" and "cmd_vel" topics hold velocity values, CHANGES NEEDED

  /*
  this algorithm continously sends different values to motors, as they constantly reduces.
  it may be TOO MUCH STRESSFULL for Rover
  */

}

float distance(float xo,float yo,float xp,float yp )       // caculate the distance
{

 float distance=sqrt((xp-xo)*(xp-xo)+(yp-yo)*(yp-yo));
 return distance;

}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "to_zero");
  ros::NodeHandle n;
  ros::Rate loop_rate(30);
  odom odominfo;
  ros::Subscriber subodom = n.subscribe<nav_msgs::Odometry>("/rtabmap/odom", 1, &odom::callback, &odominfo);  // the template to sub from ros topic
  path pathinfo;
  ros::Subscriber subpath = n.subscribe<nav_msgs::Path>("/rtabmap/local_path", 1, &path::callback, &pathinfo);
  planclass plan;
  plan.nextpoint(0);
  tf::Quaternion odomq;
  double yaw_rov, yaw_target;
  float x,y,xdir,ydir,xod,yod;
  float dist;

  while (ros::ok())
    {
        ros::spinOnce();
	//ROS_INFO("storeodompoint-> x: [%f]\n", odominfo.x);
	//ROS_INFO("storepathpoint-> y: [%f]\n", pathinfo.y);

  dist = distance(odominfo.x,odominfo.y,pathinfo.x[sequence],pathinfo.y[sequence])

  int sequence;
	sequence=plan.nowpoint();
	if (dist>=0.5)               // distance_from_target tolerance TBD
{
  go(dist)
  continue;
}
  else
{
	plan.nextpoint(sequence+1);
  go(0)                         // "stop command"
	continue;
}

  odomq[0]=odominfo.xo;
  odomq[1]=odominfo.yo;
  odomq[2]=odominfo.zo;
  odomq[3]=odominfo.wo;
  toEulerAngle(odomq, yaw_rov);
  yaw_target = atan2((pathinfo.y[sequence]-odominfo.y),(pathinfo.x[sequence]-odominfo.x));

  if ((yaw_target-yaw_rov)>=0.2)      // orientation tolerance TBD
{
	turn (static_cast <float> (yaw_target-yaw_rov));
	continue;

}
	else
{
	turn(0);              // "stop command"
	continue;

}
  /*
  FUNDAMENTAL: the above lines about orientation and rotations work ONLY AND
  ONLY IF the "yaw_rov" value calculated by "toEulerAngle" function is calculated
  with respect to the same frame of reference of position coordinates.
  THIS NEEDS TESTING
  */

        loop_rate.sleep();
    }
}


//if map update continously, the path may also change, then you can pub the goal in a loop, and just subscribe the first 3-5 data,
//reference :http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29
