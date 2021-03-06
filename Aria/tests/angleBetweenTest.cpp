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

/* See also angleFixTest, angleTest, mathTests. */

int main(void)
{
  bool error = false;

  if (!ArMath::angleBetween(60, 30, 90))
  {
    error = true;
    printf("Failed 60 reported not between 30, 90\n");
  }
  if (ArMath::angleBetween(120, 30, 90))
  {
    error = true;
    printf("Failed 120 reported between 30, 90\n");
  }

  if (!ArMath::angleBetween(-60, -90, -30))
  {
    error = true;
    printf("Failed -60 reported not between -30, -90\n");
  }
  if (ArMath::angleBetween(-120, -90, -30))
  {
    error = true;
    printf("Failed -120 reported between -30, -90\n");
  }

  if (!ArMath::angleBetween(-120, 90, -90))
  {
    error = true;
    printf("Failed -120 reported not between 90, -90\n");
  }
  if (!ArMath::angleBetween(120, 90, -90))
  {
    error = true;
    printf("Failed 120 reported not between 90, -90\n");
  }
  if (ArMath::angleBetween(0, 90, -90))
  {
    error = true;
    printf("Failed 0 reported between 90, -90\n");
  }

  if (!ArMath::angleBetween(-30, -90, 90))
  {
    error = true;
    printf("Failed -30 reported not between -90, 90\n");
  }
  if (!ArMath::angleBetween(30, -90, 90))
  {
    error = true;
    printf("Failed 30 reported not between -90, 90\n");
  }
  
  if (ArMath::angleBetween(180, -90, 90))
  {
    error = true;
    printf("Failed 180 reported between -90, 90\n");
  }

  if (!error)
    printf("\nTest ran successfully!\n");
  else
    printf("\nTest failed!\n");
}
