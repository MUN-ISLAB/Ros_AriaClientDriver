/*
MobileRobots Advanced Robotics Interface for Applications (ARIA)
Copyright (C) 2004, 2005 ActivMedia Robotics LLC
Copyright (C) 2006, 2007, 2008, 2009, 2010 MobileRobots Inc.
Copyright (C) 2011, 2012 Adept Technology

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact 
MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
MobileRobots Inc, 10 Columbia Drive, Amherst, NH 03031; 800-639-9481
*/
#include "Aria.h"

class ActionTest : public ArAction
{
public:
  ActionTest(double turnAmount, double speed);
  virtual ~ActionTest(void) {}
  virtual ArActionDesired *fire(ArActionDesired currentDesired);
  
protected:
  ArActionDesired myActionDesired;
  double myTurnAmount;
  double mySpeed;
};

ActionTest::ActionTest(double turnAmount, double speed) :
  ArAction("ActionTest")
{
  myTurnAmount = turnAmount;
  mySpeed = speed;
}

ArActionDesired *ActionTest::fire(ArActionDesired currentDesired)
{
  myActionDesired.reset();
  if (fabs(mySpeed) > 1)
    myActionDesired.setVel(mySpeed);
  if (fabs(myTurnAmount) > 1)
    myActionDesired.setDeltaHeading(myTurnAmount);
  return &myActionDesired;  
}

int main(int argc, char **argv)
{

  std::string str;
  int ret;
  ArTcpConnection con;
  ArRobot robot;

  ActionTest at1(-50, 333);
  ActionTest at2(25, 666);
  ActionTest at3(25, 0);
  ActionTest at4(0, -999);

  Aria::init();
  
  if ((ret = con.open()) != 0)
  {
    str = con.getOpenMessage(ret);
    printf("Open failed: %s\n", str.c_str());
    Aria::shutdown();
    return 1;
  }

  robot.setDeviceConnection(&con);
  if (!robot.blockingConnect())
  {
    printf("Could not connect to robot... exiting\n");
    Aria::shutdown();
    return 1;
  }

  robot.addAction(&at1, 100);
  robot.addAction(&at2, 100);
  robot.addAction(&at3, 100);
  robot.addAction(&at4, 100);

  robot.run(true);
  Aria::shutdown();
  return 0;



}
