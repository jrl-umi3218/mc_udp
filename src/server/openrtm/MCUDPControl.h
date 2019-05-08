#pragma once

#include <mc_udp/server/Server.h>

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

#include <chrono>
#include <memory>

class MCUDPControl  : public RTC::DataFlowComponentBase
{
public:
  MCUDPControl(RTC::Manager* manager);
  ~MCUDPControl();

  virtual RTC::ReturnCode_t onInitialize();

  virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

protected:
  // Configuration/Run
  bool m_enabled;
  int port;

  // Outputs
  RTC::TimedDoubleSeq m_qOut;
  RTC::OutPort<RTC::TimedDoubleSeq> m_qOutOut;

private:
  /* Measure execution time */
  std::chrono::time_point<std::chrono::system_clock> compute_start;
  std::chrono::time_point<std::chrono::system_clock> compute_end;
  std::chrono::duration<double> compute_time;
  /** Data server */
  mc_udp::Server server_;

  bool got_control_;
  bool control_lost_;
  uint64_t control_lost_iter_;
};


extern "C"
{
  DLL_EXPORT void MCUDPControlInit(RTC::Manager* manager);
}
