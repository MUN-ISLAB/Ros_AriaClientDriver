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
#ifndef ARLASERCONNECTOR_H
#define ARLASERCONNECTOR_H

#include "ariaTypedefs.h"
#include "ArSerialConnection.h"
#include "ArTcpConnection.h"
#include "ArArgumentBuilder.h"
#include "ArArgumentParser.h"
#include "ariaUtil.h"
#include "ArRobotConnector.h"

class ArLaser;
class ArRobot;



/// Connect to robot and laser based on run-time availablitily and command-line arguments
/**

   ArLaserConnector makes a laser connection either through a serial port 
   connection, or through a TCP
   port (for the simulator or for robots with Ethernet-serial bridge
   devices instead of onboard computers).
   Normally, it first attempts a TCP connection on 
   @a localhost port 8101, to use a simulator if running. If the simulator
   is not running, then it normally then connects using the serial port
   Various connection
   parameters are configurable through command-line arguments or in the robot
   parameter file. (Though the internal interface used by ARIA to do this is also
   available if you need to use it: See addLaser(); otherwise don't use
   addLaser(), setupLaser(), etc.).
  
   When you create your ArLaserConnector, pass it command line parameters via
   either the argc and argv variables from main(), or pass it an
   ArArgumentBuilder or ArArgumentParser object. (ArArgumentBuilder
   is able to obtain command line parameters from a Windows program
   that uses WinMain() instead of main()).
   ArLaserConnector registers a callback with the global Aria class. Use
   Aria::parseArgs() to parse all command line parameters to the program, and
   Aria::logOptions() to print out information about all registered command-line parameters.

   The following command-line arguments are checked:
   @verbinclude ArLaserConnector_options

   To connect to any lasers that were set up in the robot parameter file or
   via command line arguments, call connectLasers().  If successful, 
   connectLasers() will return true and add an entry for each laser connected
   in the ArRobot object's list of lasers.  These ArLaser objects can be
   accessed from your ArRobot object via ArRobot::findLaser() or ArRobot::getLaserMap(). 
   

   @since 2.7.0

 **/
class ArLaserConnector
{
public:
  /// Constructor that takes argument parser
  AREXPORT ArLaserConnector(ArArgumentParser *parser, 
			    ArRobot *robot, ArRobotConnector *robotConnector,
			    bool autoParseArgs = true,
			    ArLog::LogLevel infoLogLevel = ArLog::Verbose);
  /// Destructor
  AREXPORT ~ArLaserConnector(void);
  /// Connects all the lasers the robot has that should be auto connected
  AREXPORT bool connectLasers(bool continueOnFailedConnect = false,
			      bool addConnectedLasersToRobot = true,
			      bool addAllLasersToRobot = false,
			      bool turnOnLasers = true,
			      bool powerCycleLaserOnFailedConnect = true);
  /// Sets up a laser to be connected
  AREXPORT bool setupLaser(ArLaser *laser, 
			   int laserNumber = 1);
  /// Connects the laser synchronously (will take up to a minute)
  AREXPORT bool connectLaser(ArLaser *laser,
			     int laserNumber = 1,
			     bool forceConnection = true);
  /// Adds a laser so parsing will get it
  AREXPORT bool addLaser(ArLaser *laser,
			 int laserNumber = 1);
  /// Adds a laser for parsing but where connectLaser will be called later
  AREXPORT bool addPlaceholderLaser(ArLaser *placeholderLaser,
				    int laserNumber = 1,
				    bool takeOwnershipOfPlaceholder = false);
  /// Function to parse the arguments given in the constructor
  AREXPORT bool parseArgs(void);
  /// Function to parse the arguments given in an arbitrary parser
  AREXPORT bool parseArgs(ArArgumentParser *parser);
  /// Log the options the simple connector has
  AREXPORT void logOptions(void) const;
  /// Internal function to get the laser (only useful between parseArgs and connectLasers)
  AREXPORT ArLaser *getLaser(int laserNumber);

  /// Internal function to replace the laser (only useful between parseArgs and connectLasers) but not the laser data
  AREXPORT bool replaceLaser(ArLaser *laser, int laserNumber);
  
protected:
  /// Class that holds information about the laser data
  class LaserData
  {
  public:
    LaserData(int number, ArLaser *laser, 
	      bool laserIsPlaceholder = false, bool ownPlaceholder = false)
      { 
	myNumber = number; 
	myLaser = laser; 
	myConn = NULL;
	myLaserIsPlaceholder = laserIsPlaceholder;
	myOwnPlaceholder = ownPlaceholder;
	myConnect = false; myConnectReallySet = false;
	myPort = NULL; 
	myPortType = NULL;
	myRemoteTcpPort = 0; myRemoteTcpPortReallySet = false;
	myFlipped = false; myFlippedReallySet = false; 
	myDegreesStart = HUGE_VAL; myDegreesStartReallySet = false; 
	myDegreesEnd = -HUGE_VAL; myDegreesEndReallySet = false; 
	myDegrees = NULL; 
	myIncrementByDegrees = -HUGE_VAL; myIncrementByDegreesReallySet = false; 
	myIncrement = NULL; 
	myUnits = NULL; 
	myReflectorBits = NULL;
	myPowerControlled = true; myPowerControlledReallySet = false; 
	myStartingBaud = NULL;
	myAutoBaud = NULL;
	myMaxRange = INT_MAX; myMaxRangeReallySet = false; 
	myAdditionalIgnoreReadings = NULL;
      }
    virtual ~LaserData() {}
    /// The number of this laser
    int myNumber;
    /// The actual pointer to this laser
    ArLaser *myLaser;
    // our connection
    ArDeviceConnection *myConn;
    /// If the laser is a placeholder for parsing
    bool myLaserIsPlaceholder;
    /// If we own the placeholder laser
    bool myOwnPlaceholder;
    // if we want to connect the laser
    bool myConnect;
    // if myConnect was really set
    bool myConnectReallySet;
    // the port we want to connect the laser on
    const char *myPort;
    // the type of port we want to connect to the laser on
    const char *myPortType;
    // laser tcp port if we're doing a remote host
    int myRemoteTcpPort;  
    // if our remote laser tcp port was really set
    bool myRemoteTcpPortReallySet;
    // if we have the laser flipped
    bool myFlipped;
    // if our flipped was really set
    bool myFlippedReallySet;
    // what degrees to start at 
    double myDegreesStart;
    // if our start degrees was really set
    bool myDegreesStartReallySet;
    // what degrees to end at 
    double myDegreesEnd;
    // if our end degrees was really set
    bool myDegreesEndReallySet;
    // the degrees we want wto use
    const char *myDegrees;
    // what increment to use
    double myIncrementByDegrees;
    // if our end degrees was really set
    bool myIncrementByDegreesReallySet;
    // the increment we want to use
    const char *myIncrement;
    /// the units we want to use 
    const char *myUnits;
    /// the reflector bits we want to use 
    const char *myReflectorBits;
    // if we are controlling the laser power
    bool myPowerControlled;
    // if our flipped was really set
    bool myPowerControlledReallySet;
    /// the starting baud we want to use
    const char *myStartingBaud;
    /// the auto baud we want to use
    const char *myAutoBaud;
    // if we set a new max range from the command line
    int myMaxRange;
    // if our new max range was really set
    bool myMaxRangeReallySet;
    /// the additional laser ignore readings
    const char *myAdditionalIgnoreReadings;
  };
  std::map<int, LaserData *> myLasers;
  
  /// Parses the laser arguments
  AREXPORT bool parseLaserArgs(ArArgumentParser *parser, 
			       LaserData *laserData);
  /// Logs the laser command line option help text. 
  AREXPORT void logLaserOptions(LaserData *laserdata, bool header = true, bool metaOpts = true) const;
  // Sets the laser parameters
  bool internalConfigureLaser(LaserData *laserData);

  std::string myLaserTypes;

  // our parser
  ArArgumentParser *myParser;
  bool myOwnParser;
  // if we should autoparse args or toss errors 
  bool myAutoParseArgs;
  bool myParsedArgs;

  ArRobot *myRobot;
  ArRobotConnector *myRobotConnector;

  ArLog::LogLevel myInfoLogLevel;

  ArRetFunctorC<bool, ArLaserConnector> myParseArgsCB;
  ArConstFunctorC<ArLaserConnector> myLogOptionsCB;
};

#endif // ARLASERCONNECTOR_H
