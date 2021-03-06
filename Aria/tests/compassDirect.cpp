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
#include "ArTCMCompassDirect.h"
#include <assert.h>

void compassCB(double heading)
{
  fprintf(stderr, "compass callback got heading %f\n", heading);
  assert(heading >= -180 && heading <= 180);
}

int main(int argc, char **argv)
{
  Aria::init();
  ArTCMCompassDirect compass("/dev/ttyS0");
  if(!compass.blockingConnect()) 
  {
    puts("Error connecting!");
    return -1;
  }
  compass.addHeadingDataCallback(new ArGlobalFunctor1<double>(&compassCB));
  ArTime printTime;
  printTime.setToNow();
  while(true)
  {
    if(compass.read() < 0) 
    {
      puts("Error reading!");
      return -1;
    }
    if(printTime.secSince() >= 5)
    {
      printf("%3.6f\n", compass.getHeading());
      fflush(stdout);
      printTime.setToNow();
    }
    assert(compass.getHeading() >= -180 && compass.getHeading() <= 180);
    ArUtil::sleep(200);
  }
}
