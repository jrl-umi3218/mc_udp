/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#include <mc_udp/client/Client.h>

#include <mc_control/mc_global_controller.h>
#include <mc_rbdyn/rpy_utils.h>
#include <mc_rtc/version.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <signal.h>

static bool running = true;

void handler(int)
{
  running = false;
}

void cli(mc_control::MCGlobalController & ctl)
{
  while(running)
  {
    std::string ui;
    std::getline(std::cin, ui);
    std::stringstream ss;
    ss << ui;
    std::string token;
    ss >> token;
    if(token == "stop")
    {
      mc_rtc::log::info("Stopping connection");
      running = false;
    }
    else if(token == "hs" || token == "GoToHalfSitPose" || token == "half_sitting")
    {
      ctl.GoToHalfSitPose_service();
    }
    else if(token.size())
    {
      std::cerr << "Unkwown command " << token << std::endl;
    }
  }
}

template<typename T>
void updateConfig(mc_rtc::Configuration & config, po::variables_map & vm, const char * name, T & param)
{
  if(!vm.count(name))
  {
    if(config.has(name))
    {
      param = static_cast<T>(config(name));
    }
    else
    {
      config.add(name, param);
    }
  }
  else
  {
    config.add(name, param);
  }
}

int main(int argc, char * argv[])
{
  std::string conf_file = "";
  std::string host = "localhost";
  int port = 4444;

  /** Whether encoder desired velocities should be sent */
  bool withEncoderVelocity = false;

  /** Whether to use one or two client(s) */
  bool singleClient = false;

  po::options_description desc("MCUDPControl options");
  // clang-format off
  desc.add_options()
    ("help", "Display help message")
    ("encoderVelocity", "Send/receive encoder velocities")
    ("host,h", po::value<std::string>(&host), "Connection host")
    ("port,p", po::value<int>(&port), "Connection port")
    ("conf,f", po::value<std::string>(&conf_file), "Configuration file")
    ("single,s", "Use a single client");
  // clang-format on

  /** If the robot name is NOT_SET then warn once */
  bool warnedNOT_SET = false;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }

  if(vm.count("encoderVelocity"))
  {
    mc_rtc::log::info("[UDP] Sending/Receiving encoder velocities");
    withEncoderVelocity = true;
  }

  if(vm.count("single"))
  {
    singleClient = true;
  }

  if(mc_rtc::MC_RTC_VERSION != mc_rtc::version())
  {
    mc_rtc::log::error("mc_udp was compiled with {} but mc_rtc is at version {}, you might face subtle issues or "
                       "unexpected crashes, please recompile mc_udp",
                       mc_rtc::MC_RTC_VERSION, mc_rtc::version());
  }

  mc_control::MCGlobalController::GlobalConfiguration gconfig(conf_file, nullptr);
  auto config = gconfig.config;
  if(!config.has("UDP"))
  {
    config.add("UDP");
  }
  config = config("UDP");
  updateConfig(config, vm, "host", host);
  updateConfig(config, vm, "port", port);
  mc_control::MCGlobalController controller(gconfig);
  if(singleClient)
  {
    mc_rtc::log::info("Connect UDP client to {};{}", host, port);
  }
  else
  {
    mc_rtc::log::info("Connecting UDP sensors client to {}:{}", host, port);
  }
  mc_udp::Client sensorsClient(host, port);
  mc_udp::Client * controlClientPtr = &sensorsClient;
  if(!singleClient)
  {
    mc_rtc::log::info("Connecting UDP control client to {}:{}", host, port + 1);
    controlClientPtr = new mc_udp::Client(host, port + 1);
  }
  mc_udp::Client & controlClient = *controlClientPtr;
  bool init = false;
  // RTC port to robot force sensors
  std::unordered_map<std::string, std::string> fsensors;
  fsensors["rfsensor"] = "RightFootForceSensor";
  fsensors["lfsensor"] = "LeftFootForceSensor";
  fsensors["rhsensor"] = "RightHandForceSensor";
  fsensors["lhsensor"] = "LeftHandForceSensor";
  std::map<std::string, std::map<std::string, sva::ForceVecd>> robot_wrenches;
  auto refIndex = [&controller](const std::string & jName) {
    const auto & rjo = controller.robot().refJointOrder();
    for(size_t i = 0; i < rjo.size(); ++i)
    {
      if(rjo[i] == jName)
      {
        return static_cast<int>(i);
      }
    }
    return -1;
  };
  std::map<size_t, double> ignoredJoints;
  std::map<size_t, double> ignoredVelocities;
  if(config.has("IgnoredJoints"))
  {
    const auto & c = config("IgnoredJoints");
    std::vector<std::string> joints = c("joints", std::vector<std::string>{});
    std::map<std::string, double> ignoredValues = c("values", std::map<std::string, double>{});
    std::map<std::string, double> ignoredVelocityValues = c("velocities", std::map<std::string, double>{});

    for(const auto & jN : joints)
    {
      if(controller.robot().hasJoint(jN))
      {
        const auto & rjo = controller.robot().refJointOrder();
        const auto idx = std::distance(rjo.begin(), std::find(rjo.begin(), rjo.end(), jN));
        double qInit = 0;
        double alphaInit = 0;
        // Ignored joint values
        if(ignoredValues.count(jN) > 0)
        { // Use user-provided value for the ignored joint
          qInit = ignoredValues[jN];
        }
        else
        {
          // Use halfsitting configuration
          qInit = controller.robot().stance().at(jN)[0];
        }
        ignoredJoints[idx] = qInit;

        // Ignored velocity values
        if(ignoredVelocityValues.count(jN) > 0)
        { // Use user-provided value for the ignored joint
          alphaInit = ignoredVelocityValues[jN];
        }
        ignoredVelocities[idx] = alphaInit;
        mc_rtc::log::warning("[UDP] Joint {} is ignored, value = {}, velocity= {}", jN, qInit, alphaInit);
      }
      else
      {
        mc_rtc::log::warning("[UDP] Ignored joint {} is not present in robot ", jN, controller.robot().name());
      }
    }
  }
  uint64_t prev_id = 0;
  using duration_ms = std::chrono::duration<double, std::milli>;
  duration_ms udp_run_dt{0};
  controller.controller().logger().addLogEntry("perf_UDP", [&udp_run_dt]() { return udp_run_dt.count(); });
  signal(SIGINT, handler);
  std::thread cli_thread([&controller]() { cli(controller); });
  std::vector<double> qIn;
  std::vector<double> alphaIn;
  using clock = typename std::conditional<std::chrono::high_resolution_clock::is_steady,
                                          std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
  auto mc_udp_start = clock::now();
  while(running)
  {
    if(sensorsClient.recv())
    {
      auto start = clock::now();
      const std::string & mainRobotName = controller.robot().name();
      if(!sensorsClient.sensors().messages.count(mainRobotName))
      {
        mc_rtc::log::error("Server is providing sensors message for:");
        for(const auto & m : sensorsClient.sensors().messages)
        {
          mc_rtc::log::error("- {}", m.first);
        }
        mc_rtc::log::error_and_throw<std::runtime_error>("Server is not providing sensors message for main robot ({})",
                                                         mainRobotName);
      }
      for(const auto & msg : sensorsClient.sensors().messages)
      {
        bool isMain = mainRobotName == msg.first;
        if(!controller.controller().robots().hasRobot(msg.first))
        {
          if(msg.first == "NOT_SET")
          {
            if(!warnedNOT_SET)
            {
              mc_rtc::log::warning("Server is providing data for the NOT_SET robot, the server configuration should be "
                                   "updated, assuming this is the main robot");
              warnedNOT_SET = true;
            }
          }
          else
          {
            mc_rtc::log::error("Server is providing data for a robot that is not controlled by this controller: {}",
                               msg.first);
            continue;
          }
        }
        const auto & sensors = msg.second;
        auto & robot = msg.first != "NOT_SET" ? controller.robot(msg.first) : controller.robot();
        Eigen::Vector3d rpy;
        rpy << sensors.orientation[0], sensors.orientation[1], sensors.orientation[2];
        Eigen::Vector3d pos;
        pos << sensors.position[0], sensors.position[1], sensors.position[2];
        Eigen::Vector3d vel;
        vel << sensors.angularVelocity[0], sensors.angularVelocity[1], sensors.angularVelocity[2];
        Eigen::Vector3d acc;
        acc << sensors.linearAcceleration[0], sensors.linearAcceleration[1], sensors.linearAcceleration[2];
        if(isMain)
        {
          qIn = sensors.encoders;
          alphaIn = sensors.encoderVelocities;
          // Ignore encoder value for ignored joints
          for(const auto & j : ignoredJoints)
          {
            qIn[j.first] = j.second;
          }
          if(withEncoderVelocity)
          {
            for(const auto & j : ignoredVelocities)
            {
              alphaIn[j.first] = j.second;
            }
          }
          controller.setEncoderValues(qIn);
          controller.setEncoderVelocities(alphaIn);
          controller.setJointTorques(sensors.torques);
          controller.setSensorOrientation(Eigen::Quaterniond(mc_rbdyn::rpyToMat(rpy)));
          controller.setSensorPosition(pos);
          controller.setSensorAngularVelocity(vel);
          controller.setSensorLinearAcceleration(acc);
        }
        else
        {
          controller.setEncoderValues(robot.name(), sensors.encoders);
          controller.setEncoderVelocities(robot.name(), sensors.encoderVelocities);
          controller.setJointTorques(robot.name(), sensors.torques);
          controller.setSensorOrientation(robot.name(), Eigen::Quaterniond(mc_rbdyn::rpyToMat(rpy)));
          controller.setSensorPosition(robot.name(), pos);
          controller.setSensorAngularVelocity(robot.name(), vel);
          controller.setSensorLinearAcceleration(robot.name(), acc);
        }
        // Floating base sensor
        if(robot.hasBodySensor("FloatingBase"))
        {
          controller.setSensorPositions(
              robot.name(),
              {{"FloatingBase", {sensors.floatingBasePos[0], sensors.floatingBasePos[1], sensors.floatingBasePos[2]}}});
          Eigen::Vector3d fbRPY;
          controller.setSensorOrientations(
              robot.name(), {{"FloatingBase", Eigen::Quaterniond(mc_rbdyn::rpyToMat({sensors.floatingBaseRPY[0],
                                                                                     sensors.floatingBaseRPY[1],
                                                                                     sensors.floatingBaseRPY[2]}))}});
          controller.setSensorAngularVelocities(
              robot.name(),
              {{"FloatingBase", {sensors.floatingBaseVel[0], sensors.floatingBaseVel[1], sensors.floatingBaseVel[2]}}});
          controller.setSensorLinearVelocities(
              robot.name(),
              {{"FloatingBase", {sensors.floatingBaseVel[3], sensors.floatingBaseVel[4], sensors.floatingBaseVel[5]}}});
          controller.setSensorLinearAccelerations(
              robot.name(),
              {{"FloatingBase", {sensors.floatingBaseAcc[0], sensors.floatingBaseAcc[1], sensors.floatingBaseAcc[2]}}});
        }
        auto & wrenches = robot_wrenches[msg.first];
        for(const auto & fs : sensors.fsensors)
        {
          Eigen::Vector6d reading;
          reading << fs.reading[3], fs.reading[4], fs.reading[5], fs.reading[0], fs.reading[1], fs.reading[2];
          wrenches[fsensors.at(fs.name)] = sva::ForceVecd(reading);
        }
        controller.setWrenches(robot.name(), wrenches);
      }
      auto & sc = sensorsClient.sensors().messages.at(controller.robot().name());
      if(!init)
      {
        auto init_start = clock::now();
        controller.init(qIn);
        controller.running = true;
        init = true;
        auto init_end = clock::now();
        duration_ms init_dt = init_end - init_start;
        for(const auto & robot : controller.controller().robots())
        {
          const auto & rjo = robot.module().ref_joint_order();
          if(rjo.size() == 0)
          {
            continue;
          }
          auto & cc = controlClient.control().messages[robot.name()];
          auto & qOut = cc.encoders;
          auto & alphaOut = cc.encoderVelocities;
          if(qOut.size() != rjo.size())
          {
            qOut.resize(rjo.size());
          }
          if(withEncoderVelocity && alphaOut.size() != rjo.size())
          {
            alphaOut.resize(rjo.size());
          }
        }
        mc_rtc::log::info("[MCUDPControl] Init duration {}", init_dt.count());
        sensorsClient.init();
        if(!singleClient)
        {
          controlClient.init();
        }
      }
      else
      {
        if(prev_id + 1 != sc.id)
        {
          mc_rtc::log::warning("[MCUDPControl] Missed one or more sensors reading (previous id: {}, current id: {})",
                               prev_id, sc.id);
        }
        if(controller.run())
        {
          for(const auto & robot : controller.controller().outputRobots())
          {
            const auto & rjo = robot.module().ref_joint_order();
            if(rjo.size() == 0)
            {
              continue;
            }
            const auto & mbc = robot.mbc();
            auto & cc = controlClient.control().messages[robot.name()];
            auto & qOut = cc.encoders;
            auto & alphaOut = cc.encoderVelocities;
            for(size_t i = 0; i < rjo.size(); ++i)
            {
              const auto & jN = rjo[i];
              if(robot.hasJoint(jN))
              {
                auto jIndex = robot.jointIndexByName(jN);
                if(mbc.q[jIndex].size() == 1)
                {
                  auto jIdx = robot.jointIndexByName(jN);
                  qOut[i] = mbc.q[jIdx][0];
                  if(withEncoderVelocity)
                  {
                    alphaOut[i] = mbc.alpha[jIdx][0];
                  }
                }
              }
            }

            // Ignore QP output for ignored joints
            for(const auto & j : ignoredJoints)
            {
              qOut[j.first] = j.second;
            }
            if(withEncoderVelocity)
            {
              for(const auto & j : ignoredVelocities)
              {
                alphaOut[j.first] = j.second;
              }
            }
            cc.id = sc.id;
          }
          controlClient.send();
        }
      }
      prev_id = sc.id;
      udp_run_dt = clock::now() - start;
    }
    else if(!init)
    {
      duration_ms elapsed = clock::now() - mc_udp_start;
      if(elapsed.count() > 5000)
      {
        mc_rtc::log::warning("No messages received yet, requesting stream again");
        mc_udp_start = clock::now();
        sensorsClient.sendHello();
        if(controlClientPtr)
        {
          controlClient.sendHello();
        }
      }
    }
  }
  return 0;
}
