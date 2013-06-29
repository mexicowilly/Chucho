#
# Copyright 2013 Will Mason
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

function usage() {
    echo "Accepted arguments:"
    echo "    -c arg : The number of clients to run"
    echo "    -h arg : The host to which to connect"
    echo "    -m arg : The number of minutes to run"
    echo "    -p arg : The client program"
    echo "    -v arg : The number of messages per second to write"
}

if [ "$#" -eq 0 ] ; then
    usage
    exit
fi

HOST=localhost
MESSAGES_PER_SECOND=1
TOTAL_MINUTES=1
COUNT=1
ARGS=`getopt c:h:m:p:v: $*`
echo $ARGS
if [ $? != 0 ] ; then
    usage
    exit 1
fi
set -- $ARGS
for ARG
do
    case "$ARG" in
        -c)
            COUNT=$2
            ;;
        -h)
            HOST=$2
            ;;
        -m)
            TOTAL_MINUTES=$2
            ;;
        -p)
            PROGRAM="$2"
            ;;
        -v)
            MESSAGES_PER_SECOND=$2
            ;;
        --)
            shift
            break
            ;;
    esac
    shift
done
if [ ! -x "$PROGRAM" ] ; then
    echo The program $PROGRAM is not executable
    exit 1
fi
echo Client program: $PROGRAM
echo Host: $HOST
echo Messages/second: $MESSAGES_PER_SECOND
echo Total minutes: $TOTAL_MINUTES
echo Number of clients: $COUNT
for ((I=1; I<=$COUNT; I++))
do
    "$PROGRAM" --log client.$I --messages_per_second $MESSAGES_PER_SECOND --total_minutes $TOTAL_MINUTES --host $HOST &
    sleep .10
done
wait
