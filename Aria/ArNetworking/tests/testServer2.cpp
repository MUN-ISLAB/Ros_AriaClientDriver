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
#include "ArNetworking.h"

void testFunction(ArServerClient *client, ArNetPacket *packet)
{
  ArNetPacket sending;
  printf("responding to a packet of command %d\n", packet->getCommand());
  sending.strToBuf ("Laser data");
  client->sendPacketTcp(&sending);
}

void setVelFunction(ArServerClient *client, ArNetPacket *packet)
{
  double vel;
  printf("recieved command %d\n", packet->getCommand());
  vel=(double)packet->bufToByte2();
  printf("recieved data %lf\n", vel);
}

int main(int argc, char **argv)
{
  Aria::init();
  ArGlobalFunctor2<ArServerClient *, ArNetPacket *> testCB(&testFunction);
  ArGlobalFunctor2<ArServerClient *, ArNetPacket *> setVelCB(&setVelFunction);
  ArServerBase server;
  //ArLog::init(ArLog::StdOut, ArLog::Verbose);
  ArNetPacket packet;

  server.addData("test", "some wierd test", &testCB, "none", "none");
  server.addData("test2", "another wierd test", &testCB, "none", "none");
  server.addData("test3", "yet another wierd test", &testCB, "none", "none");
  server.addData("SetVelRequest", "yet another wierd test", &setVelCB, "none", "none");
  if (!server.open(7273))
  {
    printf("Could not open server port\n");
    exit(1);
  }
  server.runAsync();
  while (server.getRunningWithLock())
  {
    ArUtil::sleep(1000);
    server.broadcastPacketTcp(&packet, "test3");
  }
  Aria::shutdown();
  return 0;
}
