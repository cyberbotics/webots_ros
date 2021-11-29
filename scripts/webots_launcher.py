#!/usr/bin/env python

# Copyright 1996-2020 Cyberbotics Ltd.
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

"""This launcher simply start Webots."""

import optparse
import os
import sys
import subprocess

optParser = optparse.OptionParser()
optParser.add_option("--world", dest="world", default="", help="Path to the world to load.")
optParser.add_option("--mode", dest="mode", default="realtime", help="Startup mode.")
optParser.add_option("--no-gui", dest="noGui", default="false", help="Start Webots with minimal GUI.")

optParser.add_option("--stream", dest="stream", default="false", help="Start the Webots streaming server.")
optParser.add_option("--stream-port", dest="stream_port", default="1234", help="Streaming server port.")
optParser.add_option("--stream-mode", dest="stream_mode", default="x3d", help="Streaming mode.")
optParser.add_option("--stream-monitor-activity", dest="stream_monitor_activity", default="false", help="Print a dot '.' on stdout every 5 seconds.")
optParser.add_option("--stream-disable-text-streams", dest="stream_disable_text_streams", default="false", help="Disable the streaming of stdout and stderr.")
options, args = optParser.parse_args()

if 'WEBOTS_HOME' not in os.environ:
    sys.exit('WEBOTS_HOME environment variable not defined.')

command = [os.path.join(os.environ['WEBOTS_HOME'], 'webots'), '--mode=' + options.mode, options.world]

if options.stream.lower() == 'true':
    stream_cmd = '--stream="port=' + options.stream_port + ';mode=' + options.stream_mode
    if options.stream_monitor_activity.lower() == 'true':
        stream_cmd += ';monitorActivity'
    if options.stream_disable_text_streams.lower() == 'true':
        stream_cmd += ';disableTextStreams'
    stream_cmd += '"'
    command.append(stream_cmd)

if options.noGui.lower() == 'true':
    command.append('--stdout')
    command.append('--stderr')
    command.append('--batch')
    command.append('--no-sandbox')
    command.append('--minimize')

subprocess.call(command)
