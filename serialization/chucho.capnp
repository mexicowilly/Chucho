#
# Copyright 2013-2017 Will Mason
# 
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

@0xf18a45106d77ccd3;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("chucho::capnp");

struct Events {
    events @0 :List(Event);
}

struct Event {
    formattedMessage @0 :Text;
    secondsSinceEpoch @1 :UInt64;
    fileName @2 :Text;
    lineNumber @3 :UInt32;
    functionName @4 :Text;
    logger @5 :Text;
    levelName @6 :Text;
    marker @7 :Text;
    thread @8 :Text;
}
