/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#include <mc_udp/server/Server.h>

// clang-format off
// Clang-format was disabled on purpose here to prevent it from re-ordering the includes
// These must come first
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>

#include <rtm/CorbaPort.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/Manager.h>
#include <chrono>
#include <memory>
// clang-format off

class MCUDPSensors : public RTC::DataFlowComponentBase
{
public:
  MCUDPSensors(RTC::Manager * manager);
  ~MCUDPSensors();

  virtual RTC::ReturnCode_t onInitialize();

  virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

protected:
  // Configuration/Run
  bool m_enabled;
  int port;
  std::string robot_;

  // Inputs
  RTC::TimedDoubleSeq m_qIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_qInIn;
  // Floating base input
  RTC::InPort<RTC::TimedPoint3D> m_pInIn;
  RTC::InPort<RTC::TimedPose3D> m_basePoseInIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_baseVelInIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_baseAccInIn;
#ifdef MC_UDP_OPENRTM_LEGACY
  RTC::TimedDoubleSeq m_rpyIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_rpyInIn;
  RTC::TimedDoubleSeq m_rateIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_rateInIn;
  RTC::TimedDoubleSeq m_accIn;
  RTC::InPort<RTC::TimedDoubleSeq> m_accInIn;
#else
  RTC::TimedOrientation3D m_rpyIn;
  RTC::InPort<RTC::TimedOrientation3D> m_rpyInIn;
  RTC::TimedAngularVelocity3D m_rateIn;
  RTC::InPort<RTC::TimedAngularVelocity3D> m_rateInIn;
  RTC::TimedAcceleration3D m_accIn;
  RTC::InPort<RTC::TimedAcceleration3D> m_accInIn;
#endif
  RTC::TimedPoint3D m_pIn;
  RTC::TimedPose3D m_basePoseIn;
  RTC::TimedDoubleSeq m_baseVelIn;
  RTC::TimedDoubleSeq m_baseAccIn;
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

private:
  /* Measure execution time */
  std::chrono::time_point<std::chrono::system_clock> compute_start;
  std::chrono::time_point<std::chrono::system_clock> compute_end;
  std::chrono::duration<double> compute_time;
  /** Data server */
  mc_udp::Server server_;
};

extern "C"
{
  DLL_EXPORT void MCUDPSensorsInit(RTC::Manager * manager);
}
