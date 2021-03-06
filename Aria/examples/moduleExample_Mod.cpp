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


/** @example moduleExample_Mod.cpp 
 * @brief Example demonstrating how to implement a module with ArModule
 *
  This is a simple example of how to define a module of code that Aria's ArModuleLoader class can dynamically load at runtime.
  On Windows, loadable modules are DLL files. On Linux, they are shared object (.so)
  files (ARIA's Makefiles can compile any source file ending in "Mod.cpp" into
  a .so, so to build this module, run "make moduleExample_Mod.so").
  ArModuleLoader knows about these platform conventions, so only the base name
  (without the .dll or .so suffix) is used to load it.
  To implement a loadable module, you derive a class from ArModule,
  instantiate the pure virtual functions init() and exit(), and create an
  instance of that class. The two functions simply print out the equivalent
  of Hello World. An important part is the global instance of the class
  and the call to the macro ARDEF_MODULE(). Without that macro invocation,
  Aria will never be able to invoke those two functions.

  The program moduleExample.cpp is designed to load this module and check the
  error return.

  @sa moduleExample.cpp.
  @sa ArModule in the reference manual.
*/


class SimpleMod : public ArModule
{
public:

  bool init(ArRobot *robot, void *argument = NULL);
  bool exit();
};

SimpleMod aModule;
ARDEF_MODULE(aModule);

bool SimpleMod::init(ArRobot *robot, void *argument)
{
  ArLog::log(ArLog::Terse, "module: init(%p) called in the loaded module!", robot);
  if (argument != NULL)
    ArLog::log(ArLog::Terse, "module: Argument given to ArModuleLoader::load was the string '%s'.", 
	   (char *)argument);
  else
    ArLog::log(ArLog::Terse, "module: No argument was given to ArModuleLoader (this is OK).");
    
  // Do stuff here...
    
  return(true);
}

bool SimpleMod::exit()
{
  ArLog::log(ArLog::Terse, "module: exit() called.");

  // Do stuff here...

  return(true);
}
