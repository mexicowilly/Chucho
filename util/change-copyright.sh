#
# Copyright 2013-2019 Will Mason
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

if [ "$#" -ne 2 ] ; then
    echo "You must provide the old year as arg 1 and the new year as arg 2"
    exit
fi

for FILE in `find . \( -type d \( -name build -o -name util -o -name cmake-build\* \) -prune \) -o \( -type f -exec egrep -q '^.+Copyright.+2013-.*$' {} \; -print \) | sort`
do
    awk -f util/change-copyright.awk old=$1 new=$2 ${FILE} > ${FILE}.copyright.change
    mv -f ${FILE}.copyright.change ${FILE}
    echo "Changed ${FILE}"
done
echo "# Don't forget to change the scripts in the util directory"
echo "# Also, don't forget to change the main_page.hpp file"
