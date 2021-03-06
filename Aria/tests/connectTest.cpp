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
#include <time.h>

int main(int argc, char **argv) 
{
  std::string str;
  int ret;
  int successes = 0, failures = 0;
  int action;
  bool exitOnFailure = true;
  
  ArSerialConnection con;
  ArRobot robot;

  srand(time(NULL));
// if (!exitOnFailure)
//    ArLog::init(ArLog::None, ArLog::Terse);
  //else
  //ArLog::init(ArLog::None);
  while (1)
  {
    if (con.getStatus() != ArDeviceConnection::STATUS_OPEN &&
	(ret = con.open(ArUtil::COM2)) != 0)
    {
      str = con.getOpenMessage(ret);
      printf("Open failed: %s\n", str.c_str());
      ++failures;
      if (exitOnFailure)
      {
	printf("Failed\n");
	exit(0);
      }
      else
      {
	ArUtil::sleep(200);
	continue;
      }
    }
    robot.setDeviceConnection(&con);
    ArUtil::sleep((rand() % 5) * 100);
    if (robot.blockingConnect())
    {
      robot.comInt(ArCommands::SONAR, 0);
      robot.comInt(ArCommands::SOUNDTOG, 0);
      robot.comInt(ArCommands::PLAYLIST, 0);
      robot.comInt(ArCommands::ENCODER, 1);
      ArUtil::sleep((rand() % 20) * 100);
      ++successes;
      // okay, now try to leave it in a messed up state
      action = rand() % 8;
      switch (action) {
      case 0:
	printf("Discon  0 ");
	robot.disconnect();
	ArUtil::sleep(100);
	robot.com(0);
	break;
      case 1:
	printf("Discon  1 ");
	robot.disconnect();
	ArUtil::sleep(100);
	robot.com(0);
	ArUtil::sleep(100);
	robot.com(1);
	break;
      case 2:
	printf("Discon  2 ");
	robot.disconnect();
	ArUtil::sleep(100);
	robot.com(0);
	ArUtil::sleep(100);
	robot.com(1);
	ArUtil::sleep(100);
	robot.com(2);
	break;
      case 3:
	printf("Discon 10 ");
	robot.disconnect();
	ArUtil::sleep(100);
	robot.com(10);
	break;
      case 4:
	printf("Discon    ");
	robot.disconnect();
	break;
      default:
	printf("Leave     ");
	break;
      }
    }
    else
    {
      if (exitOnFailure)
      {
	printf("Failed after %d tries.\n", successes);
	exit(0);
      }
      printf("Failed ");
      ++failures;
    }
    if ((rand() % 2) == 0)
    {
      printf(" ! RadioDisconnect ! ");
      con.write("|||\15", strlen("!!!\15"));
      
      ArUtil::sleep(100);
      con.write("WMD\15", strlen("WMD\15"));
      ArUtil::sleep(200);
    }
    if ((rand() % 2) == 0)
    {
      printf(" ! ClosePort !\n");
      con.close();
    }
    else
      printf("\n");
    printf("#### %d successes %d failures, %% %.2f success\n", successes, failures,
	   (float)successes/(float)(successes+failures)*100);


    ArUtil::sleep((rand() % 2)* 1000);
  }
  return 0; 
}

