/**
 * @file command_server.cpp
 * @brief  Brief description of file.
 *
 */
#include <ros/ros.h>
#include "sonic/Command.h"

// SonicAES sound library
#include "lib.h"
#include <jack/jack.h>

bool runfile(sonic::Command::Request &req,
        sonic::Command::Response &res)
{
    ROS_INFO("runfile: %s\n",req.cmd.c_str());
    try{
        sonicAESSynth::runFile(req.cmd.c_str());
        res.result=1;
    } catch (sonicAESSynth::Exception &e) {
        ROS_INFO("error: %s\n",e.what());
        res.error = e.whats();
        res.result=0;
    }
    return true;
}

bool cmd(sonic::Command::Request &req,
        sonic::Command::Response &res)
{
    ROS_INFO("cmd: %s\n",req.cmd.c_str());
    try{
        sonicAESSynth::cmd(req.cmd.c_str());
        res.result=1;
    } catch (sonicAESSynth::Exception &e) {
        ROS_INFO("error: %s\n",e.what());
        res.error = e.whats();
        res.result=0;
    }
    return true;
}


int main(int argc,char *argv[]){
    // ugly hack to make sure the node pulls in Jack so the
    // static library can link to it. Why this doesn't happen
    // automatically I don't know.
    jack_port_name_size();
    
    sonicAESSynth::init(NULL);
    sonicAESSynth::initAudio();
    
    ros::init(argc,argv,"sonic_server");
    ros::NodeHandle n;
    ros::ServiceServer service1 = n.advertiseService("cmd",cmd);
    ros::ServiceServer service2 = n.advertiseService("runfile",runfile);
    ROS_INFO("Ready for commands");
    ros::spin();
    return 0;
}
