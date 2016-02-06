#include "RobotBase.h"
#include <fcntl.h>
#include <stdio.h>
#include "Utility.h"
#include "PRU.h"

using namespace Utility;

RobotBase::RobotBase()
{
	setupDeviceTreeOverlay();
	initPRU();
}

void RobotBase::setupDeviceTreeOverlay()
{
	std::string capeMgrDir = searchDirectory("/sys/devices/", "bone_capemgr.") + "/slots";
	std::string dtoName = "BB-UNIVERSAL";
	int fd = open(capeMgrDir, O_WRONLY);
	write(fd, dtoName.c_str(), dtoName.length());
	close(fd);
}

void RobotBase::initPRU()
{
	PRU::Init();
	PRU::Execute(0, "./encoder.bin");
	PRU::Execute(1, "./encoder.bin");
}