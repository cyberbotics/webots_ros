#!/usr/bin/env python

# Copyright 1996-2022 Cyberbotics Ltd.
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

"""Replace the webots:// URLs with https://raw.githubusercontent.com/cyberbotics/webots/<version>/
   in sample worlds."""


import re
import sys
from pathlib import Path


def replace_url(file, tag, revert=False):
    url = 'https://raw.githubusercontent.com/cyberbotics/webots/'

    with open(file, 'r') as fd:
        content = fd.read()
    if revert:
        # revert any tag
        content = re.sub(url + '[^/]+/', 'webots://', content)
    else:
        content = re.sub('webots://', url + tag + '/', content)
    with open(file, 'w', newline='\n') as fd:
        fd.write(content)


def replace_projects_urls(tag, revert=False):
    paths = []
    paths.extend(Path('./worlds').rglob('*.wbt'))

    for path in paths:
        replace_url(path, tag, revert)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit('Missing argument: commit sha or tag.')

    if sys.argv[1] == 'webots':
        replace_projects_urls(None, True)
    else:
        replace_projects_urls(sys.argv[1])
