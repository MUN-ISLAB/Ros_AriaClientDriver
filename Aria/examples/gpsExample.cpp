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

/** @example gpsExample.cpp Example program to connect to a GPS and read and
 * display data on the terminal.
 *  Also tries to connect to a TCM compass through the computer serial port, and
 *  use that to set the ArGPS compass data.
 */

#include "Aria.h"
#include "ArGPS.h"
#include "ArGPSConnector.h"
#include "ArTrimbleGPS.h"
#include "ArTCMCompassDirect.h"
#include <iostream>

int main(int argc, char** argv)
{
  Aria::init();
  ArLog::init(ArLog::StdErr, ArLog::Normal);

  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();
  ArSimpleConnector connector(&argParser);
  ArGPSConnector gpsConnector(&argParser);

  if(!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    ArLog::log(ArLog::Terse, "gpsExample options:\n  -printTable   Print data to standard output in regular columns rather than a refreshing terminal display, and print more digits of precision");
    return 1;
  }

  // Try connecting to robot 
  ArRobot robot;
  if(!connector.connectRobot(&robot))
  {
    ArLog::log(ArLog::Terse, "gpsExample: Warning: Could not connect to robot.  Will not be able to switch GPS power on, or load GPS options from this robot's parameter file.");
  }
  else
  {
    ArLog::log(ArLog::Normal, "gpsExample: Connected to robot.");
    robot.runAsync(true);
  }

  // check command line arguments for -printTable
  bool printTable = argParser.checkArgument("printTable");

  // On the Seekur, power to the GPS receiver is switched on by this command.
  // (A third argument of 0 would turn it off). On other robots this command is
  // ignored.
  robot.com2Bytes(116, 6, 1);

  // Try connecting to a GPS. We pass the robot pointetr to the connector so it
  // can check the robot parameters for this robot type for default values for
  // GPS device connection information (receiver type, serial port, etc.)
  ArLog::log(ArLog::Normal, "gpsExample: Connecting to GPS, it may take a few seconds...");
  ArGPS *gps = gpsConnector.createGPS(&robot);
  if(!gps || !gps->connect())
  {
    ArLog::log(ArLog::Terse, "gpsExample: Error connecting to GPS device.  Try -gpsType, -gpsPort, and/or -gpsBaud command-line arguments. Use -help for help.");
    return -1;
  }


  ArLog::log(ArLog::Normal, "gpsExample: Reading data...");
  ArTime lastReadTime;
  if(printTable)
    gps->printDataLabelsHeader();
  while(true)
  {
    int r = gps->read();
    if(r & ArGPS::ReadError)
    {
      ArLog::log(ArLog::Terse, "gpsExample: Warning: error reading GPS data.");
      ArUtil::sleep(1000);
      continue;
    }


    if(r & ArGPS::ReadUpdated)
    {
      if(printTable)
      {
        gps->printData(false);
        printf("\n");
      }
      else
      {
        gps->printData();
        printf("\r");
      }
      fflush(stdout);
      ArUtil::sleep(500);
      lastReadTime.setToNow();
      continue;
    } else {
      if(lastReadTime.secSince() >= 5) {
        ArLog::log(ArLog::Terse, "gpsExample: Warning: haven't recieved any data from GPS for more than 5 seconds!");
      }
      ArUtil::sleep(1000);
      continue;
    }

  }
  return 0;
}
