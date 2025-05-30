// Copyright 2024 TIER IV, Inc.
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

#include "autoware/trajectory/path_point.hpp"
#include "autoware/trajectory/utils/crossed.hpp"
#include "lanelet2_core/primitives/LineString.h"

#include <autoware/pyplot/pyplot.hpp>

#include <autoware_planning_msgs/msg/path_point.hpp>
#include <geometry_msgs/msg/point.hpp>

#include <boost/geometry/geometries/linestring.hpp>

#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include <iostream>
#include <vector>

autoware_planning_msgs::msg::PathPoint path_point(double x, double y)
{
  autoware_planning_msgs::msg::PathPoint p;
  p.pose.position.x = x;
  p.pose.position.y = y;
  p.longitudinal_velocity_mps = 1.0;
  return p;
}

int main()
{
  using autoware::experimental::trajectory::Trajectory;

  pybind11::scoped_interpreter guard{};

  auto plt = autoware::pyplot::import();

  std::vector<autoware_planning_msgs::msg::PathPoint> points = {
    path_point(0.49, 0.59), path_point(0.61, 1.22), path_point(0.86, 1.93), path_point(1.20, 2.56),
    path_point(1.51, 3.17), path_point(1.85, 3.76), path_point(2.14, 4.26), path_point(2.60, 4.56),
    path_point(3.07, 4.55), path_point(3.61, 4.30), path_point(3.95, 4.01), path_point(4.29, 3.68),
    path_point(4.90, 3.25), path_point(5.54, 3.10), path_point(6.24, 3.18), path_point(6.88, 3.54),
    path_point(7.51, 4.25), path_point(7.85, 4.93), path_point(8.03, 5.73), path_point(8.16, 6.52),
    path_point(8.31, 7.28), path_point(8.45, 7.93), path_point(8.68, 8.45), path_point(8.96, 8.96),
    path_point(9.32, 9.36)};

  {
    std::vector<double> x;
    std::vector<double> y;

    for (const auto & p : points) {
      x.push_back(p.pose.position.x);
      y.push_back(p.pose.position.y);
    }

    plt.scatter(Args(x, y), Kwargs("label"_a = "Original", "color"_a = "red"));
  }

  auto trajectory = Trajectory<autoware_planning_msgs::msg::PathPoint>::Builder{}.build(points);

  if (!trajectory) {
    std::cerr << "Failed to build trajectory" << std::endl;
    return 1;
  }

  trajectory->align_orientation_with_trajectory_direction();

  lanelet::LineString2d line_string;
  line_string.push_back(lanelet::Point3d(lanelet::InvalId, 7.5, 8.6, 0.0));
  line_string.push_back(lanelet::Point3d(lanelet::InvalId, 10.2, 7.7, 0.0));

  auto s = autoware::experimental::trajectory::crossed(*trajectory, line_string);
  auto crossed = trajectory->compute(s.at(0));

  plt.plot(
    Args(
      std::vector<double>{line_string[0].x(), line_string[1].x()},
      std::vector<double>{line_string[0].y(), line_string[1].y()}),
    Kwargs("color"_a = "purple"));

  plt.scatter(
    Args(crossed.pose.position.x, crossed.pose.position.y),
    Kwargs("label"_a = "Crossed on trajectory", "color"_a = "purple"));

  trajectory->longitudinal_velocity_mps().range(s.at(0), trajectory->length()).set(0.0);

  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> x_stopped;
  std::vector<double> y_stopped;

  for (double i = 0.0; i <= trajectory->length(); i += 0.005) {
    auto p = trajectory->compute(i);
    if (p.longitudinal_velocity_mps > 0.0) {
      x.push_back(p.pose.position.x);
      y.push_back(p.pose.position.y);
    } else {
      x_stopped.push_back(p.pose.position.x);
      y_stopped.push_back(p.pose.position.y);
    }
  }

  plt.plot(Args(x, y), Kwargs("label"_a = "Trajectory", "color"_a = "blue"));
  plt.plot(Args(x_stopped, y_stopped), Kwargs("label"_a = "Stopped", "color"_a = "orange"));

  plt.axis(Args("equal"));
  plt.grid();
  plt.legend();
  plt.show();
}
