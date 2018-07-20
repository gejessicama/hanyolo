#ifndef MOTION_H
#define MOTION_H

class Motion{
	private:
	  int kp, kd;

	public:
  	Motion(int pr,int dv);
  	void followTape();
    bool cliff();
    void ResetServo();
    void dropBridge1();
    void dropBridge2();
    void stopMoving();
    long getEncoder0(bool,bool);
    long getEncoder1(bool,bool);
};

#endif
