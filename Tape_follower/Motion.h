#ifndef MOTION_H
#define MOTION_H

class Motion{
	private:
	  int kp, kd;

	public:
  	Motion(int pr,int dv);
  	void followTape();
    bool cliff();
    void dropBridge1();
    void dropBridge2();
    
};

#endif
