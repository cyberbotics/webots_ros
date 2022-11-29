#!/usr/bin/env python

# Copyright 1996-2023 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
This launcher simply starts Webots.
Sets the environment variable WEBOTS_EXTRA_PROJECT_PATH if finds ROS packages
being exported as '<webots_ros webots_extra_project_path="${prefix}"/>'
"""

import optparse
import os
import sys
import subprocess


def get_plugin_paths(package, attrib):
    """
    Given a package name and an attribute to search for, uses rospack to find packages
    being exported.
    For a package to be found by this method, the package.xml should have:

    <export>
        <package attrib="${prefix}"/>
    </export>

    Returns:
        Path to the found packages joined by a ':'
    """
    command = ["rospack", "plugins", f"--attrib={attrib}", f"{package}"]
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, _ = process.communicate()
    stdout = stdout.decode("utf-8").replace("\n", "")
    found_paths = stdout.split(" ")
    return ":".join(found_paths[1::2])

webots_extra_project_path = get_plugin_paths("webots_ros", "webots_extra_project_path")

if webots_extra_project_path:
    if 'WEBOTS_EXTRA_PROJECT_PATH' not in os.environ:
        os.environ['WEBOTS_EXTRA_PROJECT_PATH'] = webots_extra_project_path
    else:
        os.environ['WEBOTS_EXTRA_PROJECT_PATH'] += f":{webots_extra_project_path}"

optParser = optparse.OptionParser()
optParser.add_option("--world", dest="world", default="", help="Path to the world to load.")
optParser.add_option("--mode", dest="mode", default="realtime", help="Startup mode.")
optParser.add_option("--no-gui", dest="noGui", default="false", help="Start Webots with minimal GUI.")
optParser.add_option("--stream", dest="stream", default="false", help="Start Webots streaming server.")
options, args = optParser.parse_args()

if 'WEBOTS_HOME' not in os.environ:
    sys.exit('WEBOTS_HOME environment variable not defined.')

command = [os.path.join(os.environ['WEBOTS_HOME'], 'webots'), '--mode=' + options.mode, options.world]

if options.stream.lower() != 'false':
    if options.stream.lower() == 'true':
        command.append('--stream="port=1234;mode=x3d;monitorActivity"')
    else:
        command.append('--stream="' + options.stream + '"')

if options.noGui.lower() == 'true':
    command.append('--stdout')
    command.append('--stderr')
    command.append('--batch')
    command.append('--minimize')

subprocess.call(command)
