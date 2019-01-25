#pragma once

#include <mc_nng/server/Server.h>

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

#include <chrono>
#include <memory>

class MCNNGControl  : public RTC::DataFlowComponentBase
{
public:
  MCNNGControl(RTC::Manager* manager);
  ~MCNNGControl();

  virtual RTC::ReturnCode_t onInitialize();

  virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

protected:
  // Configuration/Run
  double m_timeStep;
  bool m_enabled;
  std::string uri;
  int timeout;

  bool was_enabled;

  // Inputs
  RTC::TimedDoubleSeq m_qIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_qInIn;
  RTC::TimedDoubleSeq m_rpyIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_rpyInIn;
  RTC::TimedDoubleSeq m_rateIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_rateInIn;
  RTC::TimedDoubleSeq m_accIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_accInIn;
  RTC::TimedDoubleSeq m_taucIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_taucInIn;
  RTC::TimedDoubleSeq rfsensor;
  RTC::InPort<RTC::TimedDoubleSeq> rfsensorIn;
  RTC::TimedDoubleSeq lfsensor;
  RTC::InPort<RTC::TimedDoubleSeq> lfsensorIn;
  RTC::TimedDoubleSeq rhsensor;
  RTC::InPort<RTC::TimedDoubleSeq> rhsensorIn;
  RTC::TimedDoubleSeq lhsensor;
  RTC::InPort<RTC::TimedDoubleSeq> lhsensorIn;

  // Outputs
  RTC::TimedDoubleSeq m_qOut;
  RTC::OutPort<RTC::TimedDoubleSeq> m_qOutOut;

private:
  /* Measure execution time */
  std::chrono::time_point<std::chrono::system_clock> compute_start;
  std::chrono::time_point<std::chrono::system_clock> compute_end;
  std::chrono::duration<double> compute_time;
  /** Data server */
  mc_nng::Server server_;

  bool got_control_;
  bool control_lost_;
  uint64_t control_lost_iter_;
};


extern "C"
{
  DLL_EXPORT void MCNNGControlInit(RTC::Manager* manager);
}
