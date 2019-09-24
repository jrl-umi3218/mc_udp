#include <mc_control/mc_global_controller.h>
#include <mc_rbdyn/rpy_utils.h>
#include <mc_udp/client/Client.h>

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
      LOG_INFO("Stopping connection")
      running = false;
    }
    else if(token == "hs" || token == "GoToHalfSitPose" || token == "half_sitting")
    {
      ctl.GoToHalfSitPose_service();
    }
    else
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

  po::options_description desc("MCUDPControl options");
  // clang-format off
  desc.add_options()
    ("help", "Display help message")
    ("host,h", po::value<std::string>(&host), "Connection host")
    ("port,p", po::value<int>(&port), "Connection port")
    ("conf,f", po::value<std::string>(&conf_file), "Configuration file");
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
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
  LOG_INFO("Connecting UDP sensors client to " << host << ":" << port)
  mc_udp::Client sensorsClient(host, port);
  LOG_INFO("Connecting UDP control client to " << host << ":" << port + 1)
  mc_udp::Client controlClient(host, port + 1);
  bool init = false;
  // RTC port to robot force sensors
  std::unordered_map<std::string, std::string> fsensors;
  fsensors["rfsensor"] = "RightFootForceSensor";
  fsensors["lfsensor"] = "LeftFootForceSensor";
  fsensors["rhsensor"] = "RightHandForceSensor";
  fsensors["lhsensor"] = "LeftHandForceSensor";
  std::map<std::string, sva::ForceVecd> wrenches;
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
  auto gripperIndex = [&](const std::vector<std::string> & joints) {
    std::vector<int> gIndex;
    for(const auto & j : joints)
    {
      gIndex.push_back(refIndex(j));
    }
    return gIndex;
  };
  std::map<std::string, std::vector<int>> grippers;
  std::map<std::string, std::vector<int>> grippersAll;
  std::map<std::string, std::vector<double>> gripperState;
  for(const auto & g : controller.gripperActiveJoints())
  {
    grippers[g.first] = gripperIndex(g.second);
    gripperState[g.first].resize(g.second.size());
  }
  for(const auto & g : controller.gripperJoints())
  {
    grippersAll[g.first] = gripperIndex(g.second);
  }
  auto updateGripperState = [&](const std::vector<double> & qIn) {
    for(auto & g : gripperState)
    {
      for(size_t i = 0; i < g.second.size(); ++i)
      {
        if(qIn[grippers[g.first][i]] != -1)
        {
          g.second[i] = qIn[grippers[g.first][i]];
        }
      }
    }
  };
  std::vector<std::string> ignoredJoints;
  uint64_t prev_id = 0;
  std::vector<double> qInit;
  using duration_ms = std::chrono::duration<double, std::milli>;
  duration_ms udp_run_dt{0};
  controller.controller().logger().addLogEntry("perf_UDP", [&udp_run_dt]() { return udp_run_dt.count(); });
  signal(SIGINT, handler);
  std::thread cli_thread([&controller]() { cli(controller); });
  while(running)
  {
    using clock = typename std::conditional<std::chrono::high_resolution_clock::is_steady,
                                            std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
    if(sensorsClient.recv())
    {
      auto start = clock::now();
      auto & sc = sensorsClient.sensors();
      controller.setEncoderValues(sc.encoders);
      controller.setJointTorques(sc.torques);
      Eigen::Vector3d rpy;
      rpy << sensorsClient.sensors().orientation[0], sensorsClient.sensors().orientation[1],
          sensorsClient.sensors().orientation[2];
      controller.setSensorOrientation(Eigen::Quaterniond(mc_rbdyn::rpyToMat(rpy)));
      Eigen::Vector3d pos;
      pos << sc.position[0], sc.position[1], sc.position[2];
      controller.setSensorPosition(pos);
      Eigen::Vector3d vel;
      vel << sensorsClient.sensors().angularVelocity[0], sensorsClient.sensors().angularVelocity[1],
          sensorsClient.sensors().angularVelocity[2];
      controller.setSensorAngularVelocity(vel);
      Eigen::Vector3d acc;
      acc << sensorsClient.sensors().angularAcceleration[0], sensorsClient.sensors().angularAcceleration[1],
          sensorsClient.sensors().angularAcceleration[2];
      controller.setSensorAcceleration(acc);

      // Floating base sensor
      if(controller.robot().hasBodySensor("FloatingBase"))
      {
        controller.setSensorPositions(
            controller.robot(),
            {{"FloatingBase", {sc.floatingBasePos[0], sc.floatingBasePos[1], sc.floatingBasePos[2]}}});
        Eigen::Vector3d fbRPY;
        controller.setSensorOrientations(
            controller.robot(),
            {{"FloatingBase", Eigen::Quaterniond(mc_rbdyn::rpyToMat(
                                  {sc.floatingBaseRPY[0], sc.floatingBaseRPY[1], sc.floatingBaseRPY[2]}))}});
        controller.setSensorAngularVelocities(
            controller.robot(),
            {{"FloatingBase", {sc.floatingBaseVel[0], sc.floatingBaseVel[1], sc.floatingBaseVel[2]}}});
        controller.setSensorLinearVelocities(
            controller.robot(),
            {{"FloatingBase", {sc.floatingBaseVel[3], sc.floatingBaseVel[4], sc.floatingBaseVel[5]}}});
        controller.setSensorAccelerations(
            controller.robot(),
            {{"FloatingBase", {sc.floatingBaseAcc[0], sc.floatingBaseAcc[1], sc.floatingBaseAcc[2]}}});
      }

      for(const auto & fs : sc.fsensors)
      {
        Eigen::Vector6d reading;
        reading << fs.reading[3], fs.reading[4], fs.reading[5], fs.reading[0], fs.reading[1], fs.reading[2];
        wrenches[fsensors.at(fs.name)] = sva::ForceVecd(reading);
      }
      controller.setWrenches(wrenches);
      updateGripperState(sensorsClient.sensors().encoders);
      controller.setActualGripperQ(gripperState);
      if(!init)
      {
        auto init_start = clock::now();
        qInit = sc.encoders;
        controller.init(qInit);
        controller.setGripperCurrentQ(gripperState);
        for(const auto & g : gripperState)
        {
          controller.setGripperTargetQ(g.first, g.second);
        }
        controller.running = true;
        init = true;
        auto init_end = clock::now();
        duration_ms init_dt = init_end - init_start;
        LOG_INFO("[MCUDPControl] Init duration " << init_dt.count())
        sensorsClient.init();
        controlClient.init();
      }
      else
      {
        if(prev_id + 1 != sc.id)
        {
          LOG_WARNING("[MCUDPControl] Missed one or more sensors reading (previous id: "
                      << prev_id << ", current id: " << sensorsClient.sensors().id << ")")
        }
        if(controller.run())
        {
          const auto & robot = controller.robot();
          const auto & mbc = robot.mbc();
          const auto & rjo = controller.ref_joint_order();
          auto & qOut = controlClient.control().encoders;
          if(qOut.size() != rjo.size())
          {
            qOut.resize(rjo.size());
          }
          for(size_t i = 0; i < rjo.size(); ++i)
          {
            const auto & jN = rjo[i];
            bool skipJoint = false;
            if(std::find(ignoredJoints.begin(), ignoredJoints.end(), jN) == ignoredJoints.end())
            {
              if(robot.hasJoint(jN))
              {
                auto jIndex = robot.jointIndexByName(jN);
                if(mbc.q[jIndex].size() == 1)
                {
                  qOut[i] = mbc.q[robot.jointIndexByName(jN)][0];
                }
                else
                {
                  skipJoint = true;
                }
              }
            }
            else
            {
              skipJoint = true;
            }
            if(skipJoint)
            {
              qOut[i] = qInit[i];
            }
          }
          auto gripperQOut = controller.gripperQ();
          for(const auto & g : grippersAll)
          {
            for(size_t i = 0; i < g.second.size(); ++i)
            {
              if(g.second[i] != -1)
              {
                qOut[g.second[i]] = gripperQOut[g.first][i];
              }
            }
          }
          controlClient.control().id = sensorsClient.sensors().id;
          controlClient.send();
        }
      }
      prev_id = sc.id;
      udp_run_dt = clock::now() - start;
    }
  }
  return 0;
}
