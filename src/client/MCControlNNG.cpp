#include <mc_nng/client/Client.h>

#include <mc_control/mc_global_controller.h>
#include <mc_rbdyn/rpy_utils.h>

int main(int argc, char * argv[])
{
  std::string conf_file = "";
  if(argc > 1)
  {
    conf_file = argv[1];
  }
  mc_control::MCGlobalController controller(conf_file);
  mc_rtc::Configuration config = controller.configuration().config("NNG", mc_rtc::Configuration{});
  std::string uri = config("uri", std::string("tcp://localhost:4444"));
  int timeout = config("timeout", 4);
  LOG_INFO("Connecting NNG client to " << uri << " (timeout: " << timeout << ")")
  mc_nng::Client client(uri, timeout);
  bool init = false;
  // RTC port to robot force sensors
  std::unordered_map<std::string, std::string> fsensors;
  fsensors["rfsensor"] = "RightFootForceSensor";
  fsensors["lfsensor"] = "LeftFootForceSensor";
  fsensors["rhsensor"] = "RightHandForceSensor";
  fsensors["lhsensor"] = "LeftHandForceSensor";
  std::map<std::string, sva::ForceVecd> wrenches;
  // FIXME Write gripper control
  std::vector<std::string> ignoredJoints = {
    "R_HAND_J0",
    "R_HAND_J1",
    "L_HAND_J0",
    "L_HAND_J1"
  };
  uint64_t prev_id = 0;
  std::vector<double> qInit;
  while(1)
  {
    if(client.recv())
    {
      controller.setEncoderValues(client.sensors().encoders);
      controller.setJointTorques(client.sensors().torques);
      Eigen::Vector3d rpy;
      rpy << client.sensors().orientation[0], client.sensors().orientation[1], client.sensors().orientation[2];
      controller.setSensorOrientation(Eigen::Quaterniond(mc_rbdyn::rpyToMat(rpy)));
      Eigen::Vector3d vel;
      vel << client.sensors().angularVelocity[0], client.sensors().angularVelocity[1], client.sensors().angularVelocity[2];
      controller.setSensorAngularVelocity(vel);
      Eigen::Vector3d acc;
      acc << client.sensors().angularAcceleration[0], client.sensors().angularAcceleration[1], client.sensors().angularAcceleration[2];
      controller.setSensorAcceleration(acc);
      for(const auto & fs : client.sensors().fsensors)
      {
        Eigen::Vector6d reading;
        reading << fs.reading[3], fs.reading[4], fs.reading[5], fs.reading[0], fs.reading[1], fs.reading[2];
        wrenches[fsensors.at(fs.name)] = sva::ForceVecd(reading);
      }
      controller.setWrenches(wrenches);
      if(!init)
      {
        qInit = client.sensors().encoders;
        controller.init(qInit);
        controller.running = true;
        init = true;
      }
      else
      {
        if(prev_id + 1 != client.sensors().id)
        {
          LOG_WARNING("[MCControlNNG] Missed one or more sensors reading (previous id: " << prev_id << ", current id: " << client.sensors().id << ")")
        }
      }
      prev_id = client.sensors().id;
      if(controller.run())
      {
        const auto & robot = controller.robot();
        const auto & mbc = robot.mbc();
        const auto & rjo = controller.ref_joint_order();
        auto & qOut = client.control().encoders;
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
        // FIXME VERY DIRTY AND STRANGE...
        client.control().id = client.sensors().id + 2;
        client.send();
      }
    }
    else
    {
      LOG_WARNING("[MCControlNNG] Failed to received sensors data within the timeout")
    }
  }
  return 0;
}