// -*- C++ -*-
/*!
 * @file  MCUDPSensors.cpp * @brief Core component for MC control * $Date$
 *
 * $Id$
 */
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

#include <mc_udp/logging.h>

#include <fstream>
#include <iomanip>

// Module specification
// <rtc-template block="module_spec">
// clang-format off
static const char* mccontrol_spec[] =
  {
    "implementation_id", "MCUDPSensors",
    "type_name",         "MCUDPSensors",
    "description",       "Core component for MC control",
    "version",           "0.1",
    "vendor",            "CNRS",
    "category",          "Generic",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.is_enabled", "0",
    "conf.default.port", "4444",
    ""
  };
// </rtc-template>

MCUDPSensors::MCUDPSensors(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_enabled(false),
    port(4444),
    m_qInIn("qIn", m_qIn),
    m_rpyInIn("rpyIn", m_rpyIn),
    m_rateInIn("rateIn", m_rateIn),
    m_accInIn("accIn", m_accIn),
    m_taucInIn("taucIn", m_taucIn),
    rfsensorIn("rfsensor", rfsensor),
    lfsensorIn("lfsensor", lfsensor),
    rhsensorIn("rhsensor", rhsensor),
    lhsensorIn("lhsensor", lhsensor),
    m_pInIn("pIn", m_pIn),
    m_poseInIn("poseIn", m_poseIn),
    m_velInIn("velIn", m_velIn),
    server_()
    // </rtc-template>
{
}
// clang-format on

MCUDPSensors::~MCUDPSensors() {}

RTC::ReturnCode_t MCUDPSensors::onInitialize()
{
  MC_UDP_INFO("MCUDPSensors::onInitialize() starting")
  // Set InPort buffers
  addInPort("qIn", m_qInIn);
  addInPort("rpyIn", m_rpyInIn);
  addInPort("rateIn", m_rateInIn);
  addInPort("accIn", m_accInIn);
  addInPort("taucIn", m_taucInIn);
  addInPort("rfsensor", rfsensorIn);
  addInPort("lfsensor", lfsensorIn);
  addInPort("rhsensor", rhsensorIn);
  addInPort("lhsensor", lhsensorIn);
  addInPort("pIn", m_pInIn);
  // Floating base
  addInPort("poseIn", m_poseInIn);
  addInPort("velIn", m_velInIn);

  // Bind variables and configuration variable
  bindParameter("is_enabled", m_enabled, "0");
  bindParameter("port", port, "4444");

  MC_UDP_INFO("MCUDPSensors::onInitialize() finished")
  return RTC::RTC_OK;
}

RTC::ReturnCode_t MCUDPSensors::onActivated(RTC::UniqueId ec_id)
{
  MC_UDP_INFO("MCUDPSensors::onActivated")
  server_.restart(port);
  server_.sensors().id = 0;
  MC_UDP_SUCCESS("MCUDPSensors started on " << port)
  return RTC::RTC_OK;
}

RTC::ReturnCode_t MCUDPSensors::onDeactivated(RTC::UniqueId ec_id)
{
  MC_UDP_INFO("MCUDPSensors::onDeactivated")
  m_enabled = false;
  server_.stop();
  server_.sensors().id += 1;
  return RTC::RTC_OK;
}

namespace
{

void read_fsensor(const std::string & name,
                  RTC::InPort<RTC::TimedDoubleSeq> & port,
                  RTC::TimedDoubleSeq & data,
                  mc_udp::Server & server_)
{
  if(port.isNew())
  {
    port.read();
    if(data.data.length() == 6)
    {
      server_.sensors().fsensor(name, data.data.NP_data());
    }
  }
}

} // namespace

RTC::ReturnCode_t MCUDPSensors::onExecute(RTC::UniqueId ec_id)
{
  read_fsensor("rfsensor", rfsensorIn, rfsensor, server_);
  read_fsensor("lfsensor", lfsensorIn, lfsensor, server_);
  read_fsensor("rhsensor", rhsensorIn, rhsensor, server_);
  read_fsensor("lhsensor", lhsensorIn, lhsensor, server_);
  if(m_rpyInIn.isNew())
  {
    m_rpyInIn.read();
#ifdef MC_UDP_OPENRTM_LEGACY
    server_.sensors().orientation[0] = m_rpyIn.data[0];
    server_.sensors().orientation[1] = m_rpyIn.data[1];
    server_.sensors().orientation[2] = m_rpyIn.data[2];
#else
    server_.sensors().orientation[0] = m_rpyIn.data.r;
    server_.sensors().orientation[1] = m_rpyIn.data.p;
    server_.sensors().orientation[2] = m_rpyIn.data.y;
#endif
  }
  if(m_rateInIn.isNew())
  {
    m_rateInIn.read();
#ifdef MC_UDP_OPENRTM_LEGACY
    server_.sensors().angularVelocity[0] = m_rateIn.data[0];
    server_.sensors().angularVelocity[1] = m_rateIn.data[1];
    server_.sensors().angularVelocity[2] = m_rateIn.data[2];
#else
    server_.sensors().angularVelocity[0] = m_rateIn.data.avx;
    server_.sensors().angularVelocity[1] = m_rateIn.data.avy;
    server_.sensors().angularVelocity[2] = m_rateIn.data.avz;
#endif
  }
  if(m_accInIn.isNew())
  {
    m_accInIn.read();
#ifdef MC_UDP_OPENRTM_LEGACY
    server_.sensors().angularAcceleration[0] = m_accIn.data[0];
    server_.sensors().angularAcceleration[1] = m_accIn.data[1];
    server_.sensors().angularAcceleration[2] = m_accIn.data[2];
#else
    server_.sensors().angularAcceleration[0] = m_accIn.data.ax;
    server_.sensors().angularAcceleration[1] = m_accIn.data.ay;
    server_.sensors().angularAcceleration[2] = m_accIn.data.az;
#endif
  }
  if(m_taucInIn.isNew())
  {
    m_taucInIn.read();
    if(server_.sensors().torques.size() != m_taucIn.data.length())
    {
      server_.sensors().torques.resize(m_taucIn.data.length());
    }
    for(unsigned int i = 0; i < static_cast<unsigned int>(m_taucIn.data.length()); ++i)
    {
      server_.sensors().torques[i] = m_taucIn.data[i];
    }
  }
  if(m_pInIn.isNew())
  {
    m_pInIn.read();
    server_.sensors().position[0] = m_pIn.data.x;
    server_.sensors().position[1] = m_pIn.data.y;
    server_.sensors().position[2] = m_pIn.data.z;
  }
  if(m_poseInIn.isNew())
  {
    m_poseInIn.read();
    auto& pos= server_.sensors().floatingBasePos;
    pos[0] = m_poseIn.data.position.x;
    pos[1] = m_poseIn.data.position.y;
    pos[2] = m_poseIn.data.position.z;
    auto& rpy= server_.sensors().floatingBaseRPY;
    rpy[0] = m_poseIn.data.orientation.r;
    rpy[1] = m_poseIn.data.orientation.p;
    rpy[2] = m_poseIn.data.orientation.y;
  }
  if(m_velInIn.isNew())
  {
    m_velInIn.read();
    server_.sensors().floatingBaseVel[0] = m_velIn.data[0];
    server_.sensors().floatingBaseVel[1] = m_velIn.data[1];
    server_.sensors().floatingBaseVel[2] = m_velIn.data[2];
    server_.sensors().floatingBaseVel[3] = m_velIn.data[3];
    server_.sensors().floatingBaseVel[4] = m_velIn.data[4];
    server_.sensors().floatingBaseVel[5] = m_velIn.data[5];
  }
  if(m_qInIn.isNew())
  {
    m_qInIn.read();
    if(server_.sensors().encoders.size() != m_qIn.data.length())
    {
      server_.sensors().encoders.resize(m_qIn.data.length());
    }
    for(unsigned int i = 0; i < m_qIn.data.length(); ++i)
    {
      server_.sensors().encoders[i] = m_qIn.data[i];
    }
    coil::TimeValue coiltm(coil::gettimeofday());
    RTC::Time tm;
    tm.sec = static_cast<CORBA::ULong>(coiltm.sec());
    tm.nsec = static_cast<CORBA::ULong>(coiltm.usec()) * 1000;
    if(m_enabled)
    {
      compute_start = std::chrono::system_clock::now();
      server_.recv();
      server_.send();
      compute_end = std::chrono::system_clock::now();
      compute_time = compute_end - compute_start;
      double elapsed = compute_time.count() * 1000;
      if(elapsed > 5.1)
      {
        MC_UDP_WARNING("Total time spent in MCUDPSensors::onExecute (" << elapsed << ") exceeded 5.1ms")
      }
      server_.sensors().id += 1;
    }
  }
  return RTC::RTC_OK;
}

extern "C"
{

  void MCUDPSensorsInit(RTC::Manager * manager)
  {
    coil::Properties profile(mccontrol_spec);
    manager->registerFactory(profile, RTC::Create<MCUDPSensors>, RTC::Delete<MCUDPSensors>);
  }
};

#pragma GCC diagnostic pop
