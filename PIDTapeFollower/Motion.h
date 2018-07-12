#ifndef MOTION_H
#define MOTION_H

class Motion{
	private:
	  int kp, kd;

	public:
  	Motion(int pr,int dv);
  	void followTape();
    bool crossGap1();
    bool crossGap2();
    
};

#endif
