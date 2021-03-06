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

class ConfigTester : public ArConfig
{
public:
  ConfigTester();
  virtual ~ConfigTester();
  virtual bool processFile(void);
  int myInt;
  double myDouble;
  bool myBool;
  ArPose myPose;
  char myString[512];

  // stuff for testing the functor ones
  std::list<std::string> myList;
  std::list<ArArgumentBuilder *> myArgList;
  bool listAdder(ArArgumentBuilder *builder) 
    { myList.push_front(builder->getFullString()); 
    printf("Added %s\n", builder->getFullString()); return true; }
  const std::list<ArArgumentBuilder *> *getList(void) 
    {
      std::list<ArArgumentBuilder *>::iterator argIt;
      std::list<std::string>::iterator listIt;
      ArArgumentBuilder *builder;

      if (myArgList.size() != 0)
      {
	while ((argIt = myArgList.begin()) != myArgList.end())
	{
	  delete (*argIt);
	  myArgList.pop_front();
	}
      }
      for (listIt = myList.begin(); listIt != myList.end(); listIt++)
      {
	builder = new ArArgumentBuilder;
	builder->add((*listIt).c_str());
	myArgList.push_front(builder);
      }
      return &myArgList;
    }
  ArRetFunctor1C<bool, ConfigTester, ArArgumentBuilder *> mySetFunctor;
  ArRetFunctorC<const std::list<ArArgumentBuilder *> *, ConfigTester> myGetFunctor;

};

ConfigTester::ConfigTester() : 
  mySetFunctor(this, &ConfigTester::listAdder),
  myGetFunctor(this, &ConfigTester::getList)
{
  myInt = 32;
  myDouble = 239.394;
  myBool = true;
  myPose.setPose(42, -42.3, 21.21);
  strcpy(myString, "happy fun string will begin to smoke");

  addParam(ArConfigArg("int", &myInt, "fun things!"), "fuah");//, 0, 300));
  addParam(ArConfigArg("double", &myDouble, "fun things double!"));//, 0, 2300));
  addParam(ArConfigArg("bool", &myBool, "fun things bool!"));
  addParam(ArConfigArg("string", myString, "fun things string!", sizeof(myString)));
  addParam(ArConfigArg("functor", &mySetFunctor, &myGetFunctor, "fun functor thing!"));
}

ConfigTester::~ConfigTester()
{
  std::list<ArArgumentBuilder *>::iterator argIt;
  if (myArgList.size() != 0)
  {
    while ((argIt = myArgList.begin()) != myArgList.end())
    {
      delete (*argIt);
      myArgList.pop_front();
    }
  }
}

bool ConfigTester::processFile(void)
{
  printf("Processed \n");
  return true;
}

bool func100(void)
{
  printf("100\n");
  return true;
}

bool func90a(void)
{
  printf("90a\n");
  return true;
}

bool func90b(void)
{
  printf("90b\n");
  return true;
}

bool func50(void)
{
  printf("50\n");
  return true;
}

int main(int argc, char **argv)
{
  Aria::init();
  ArLog::init(ArLog::StdOut, ArLog::Verbose);
  
  ArArgumentParser parser(&argc, argv);
  ConfigTester tester;
  bool ret;
  tester.writeFile("configBefore.txt");
  char errorBuffer[512];
  errorBuffer[0] = '\0';


  ArGlobalRetFunctor<bool> func100cb(&func100);
  ArGlobalRetFunctor<bool> func90acb(&func90a);
  ArGlobalRetFunctor<bool> func90bcb(&func90b);
  ArGlobalRetFunctor<bool> func50cb(&func50);

  func100cb.setName("100cb");
  func90bcb.setName("bcb");
  func50cb.setName("50cb");

  tester.addProcessFileCB(&func100cb, 100);
  tester.addProcessFileCB(&func90acb, 90);
  tester.addProcessFileCB(&func90bcb, 90);
  tester.addProcessFileCB(&func50cb, 50);

  tester.useArgumentParser(&parser);
  ret = tester.parseFile("configTest.txt", false, true, errorBuffer, 
			 sizeof(errorBuffer));
  if (ret)
  {
    printf("int %d double %g bool %s string '%s'\n", 
	   tester.myInt, tester.myDouble,
	   ArUtil::convertBool(tester.myBool),
	   tester.myString
	);
  }
  else
  {
    printf("\nFailed config test because '%s'\n\n", errorBuffer);
  }

  tester.writeFile("configAfter.txt");
  exit(0);
}



