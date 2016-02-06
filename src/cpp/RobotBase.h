#ifndef ROBOT_BASE_H_
#define ROBOT_BASE_H_

class RobotBase
{
public:
	RobotBase();
	~RobotBase();
	
private:
	void setupDeviceTreeOverlay();
	void initPRU();
};

#endif