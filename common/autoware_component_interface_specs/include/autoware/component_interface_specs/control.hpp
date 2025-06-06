// Copyright 2022 TIER IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef AUTOWARE__COMPONENT_INTERFACE_SPECS__CONTROL_HPP_
#define AUTOWARE__COMPONENT_INTERFACE_SPECS__CONTROL_HPP_

#include <autoware/component_interface_specs/utils.hpp>
#include <rclcpp/qos.hpp>

#include <autoware_control_msgs/msg/control.hpp>

namespace autoware::component_interface_specs::control
{

struct ControlCommand
{
  using Message = autoware_control_msgs::msg::Control;
  static constexpr char name[] = "/control/command/control_cmd";
  static constexpr size_t depth = 1;
  static constexpr auto reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  static constexpr auto durability = RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
};

}  // namespace autoware::component_interface_specs::control

#endif  // AUTOWARE__COMPONENT_INTERFACE_SPECS__CONTROL_HPP_
