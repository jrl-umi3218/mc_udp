#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpedantic"
#ifdef __clang__
#  pragma GCC diagnostic ignored "-Wdelete-incomplete"
#  pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#endif

#include "MCUDPSensors.h"
#include <iostream>
#include <rtm/Manager.h>
#include <signal.h>
#include <stdlib.h>
#include <string>

void MyModuleInit(RTC::Manager * manager)
{
  MCUDPSensorsInit(manager);
  RTC::RtcBase * comp;

  // Create a component
  comp = manager->createComponent("MCUDPSensors");
  std::cout << comp << std::endl;
}

int main(int argc, char ** argv)
{
  RTC::Manager * manager;
  manager = RTC::Manager::init(argc, argv);

  // Initialize manager
  manager->init(argc, argv);

  // Set module initialization proceduer
  // This procedure will be invoked in activateManager() function.
  manager->setModuleInitProc(MyModuleInit);

  // Activate manager and register to naming service
  manager->activateManager();

  // run the manager in blocking mode
  // runManager(false) is the default.
  manager->runManager();
  return 0;
}

#pragma GCC diagnostic pop
