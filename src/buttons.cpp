#include "ros/ros.h"
#include "turtlebot3_extension/ButtonValues.h"
#include "turtlebot3_msgs/SensorState.h"
#include <fstream>
using namespace ros;

turtlebot3_msgs::SensorState sensor_state_values;

void sensor_stateCallback(const turtlebot3_msgs::SensorState::ConstPtr &msg)
{
    sensor_state_values = *msg;
}

int main(int argc, char **argv)
{
    init(argc, argv, "buttons");
    NodeHandle n;

    turtlebot3_extension::ButtonValues buttons;

    Publisher pub = n.advertise<turtlebot3_extension::ButtonValues>("buttons", 1);
    Subscriber sub = n.subscribe("sensor_state", 1, sensor_stateCallback);

    ros::Rate loop_rate(10);
    int c[2] = {0, 0};

    while (ros::ok())
    {
        if (sensor_state_values.button == 1)
        {
            c[0] += 1;
            buttons.front = true;
        }
        else if (sensor_state_values.button == 2)
        {
            c[1] += 1;
            buttons.rear = true;
        }
        else
        {
            c[0] = 0;
            c[1] = 0;
            buttons.front = false;
            buttons.rear = false;
        }

        if (c[0] > 4)
        {
            buttons.front_toggle = not buttons.front_toggle;
            c[0] = 0;
        }
        if (c[1] > 4)
        {
            buttons.rear_toggle = not buttons.rear_toggle;
            c[1] = 0;
        }

        pub.publish(buttons);
        spinOnce();
        loop_rate.sleep();
    }
    exit(0);
}