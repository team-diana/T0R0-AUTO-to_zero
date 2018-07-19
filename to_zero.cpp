#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
#include <stdio.h>    
#include <math.h> 
#include <tf/tf.h>


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
         ROS_INFO("subodom-> x: [%f], y: [%f], z: [%f]\n", x,y,z);
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
  int i=0;
  float a,b,c;
  while(a!=0.00 && b!=0.00 && c!=0.00)
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

//ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]\n", x,y,z);
//return msg->pose.pose.position.x;  

}

class planclass
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


uint16_t FloatToUint(float n)
{
   return (uint16_t)(*(uint16_t*)&n);
}



void turn (float b, float a)
{
	uint16_t left;
	uint16_t right;
	if (a == 0)  
{  
        a+=5;
}  	
	float leftfloat=a;
	float rightfloat=-a;
	FloatToUint( leftfloat);
	FloatToUint( rightfloat);
	
        printf("%hu\n", left);
	printf("%hu\n", right);

}


void go (float a)
{
	uint16_t left;
	uint16_t right;
	float leftfloat=a;
	float rightfloat=a;
	FloatToUint( leftfloat);
	FloatToUint( rightfloat);
        printf("%hu\n", left);
	printf("%hu\n", right);

}

float distance(float xo,float yo,float xp,float yp )
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
  ros::Subscriber subodom = n.subscribe<nav_msgs::Odometry>("/rtabmap/odom", 1, &odom::callback, &odominfo);
  path pathinfo;
  ros::Subscriber subpath = n.subscribe<nav_msgs::Path>("/rtabmap/local_path", 1, &path::callback, &pathinfo);
  planclass plan;
  plan.nextpoint(0);
  tf::Quaternion odomq(1, 0, 0, 0);
  tf::Vector3 vector(0, 1, 0);
  tf::Vector3 odomvector;
  float x,y,xdir,ydir,xod,yod;
  
  while (ros::ok()) 
    {
        ros::spinOnce();
	ROS_INFO("storeodompoint-> x: [%f]\n", odominfo.x);
	//ROS_INFO("storepathpoint-> x: [%f]\n", pathinfo.x);
	//ROS_INFO("storepathpoint-> y: [%f]\n", pathinfo.y);
	
	int sequence;
	sequence=plan.nowpoint();
	if (distance(odominfo.x,odominfo.y,pathinfo.x[sequence],pathinfo.y[sequence])<=0.5)
{
	plan.nextpoint(sequence+1); 
	continue;
}
	odomq[0]=odominfo.xo;
	odomq[1]=odominfo.yo;
	odomq[2]=odominfo.zo;
	odomq[3]=odominfo.wo;
	odomvector = tf::quatRotate(odomq, vector);
	x=pathinfo.x[sequence]-odominfo.x;
	y=pathinfo.y[sequence]-odominfo.y;
	xdir=x/(x*x+y*y);
	ydir=y/(x*x+y*y);
	xod=odomvector[0]/(odomvector[0]*odomvector[0]+odomvector[1]*odomvector[1]);
	yod=odomvector[1]/(odomvector[0]*odomvector[0]+odomvector[1]*odomvector[1]);
	if (((xdir-xod)*(xdir-xod)+(ydir-yod)*(ydir-yod))>=5)
{
	turn (xdir-xod,ydir-yod);
	continue;

}
	else
{
	go(10);
	continue;

}

        loop_rate.sleep();
    }
}


