-- 
--  Copyright 2013-2019 Will Mason
--  
--     Licensed under the Apache License, Version 2.0 (the "License");
--     you may not use this file except in compliance with the License.
--     You may obtain a copy of the License at
--  
--       http://www.apache.org/licenses/LICENSE-2.0
--  
--     Unless required by applicable law or agreed to in writing, software
--     distributed under the License is distributed on an "AS IS" BASIS,
--     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
--     See the License for the specific language governing permissions and
--     limitations under the License.
-- 

BEGIN;
DROP TABLE IF EXISTS chucho_event;
COMMIT;

BEGIN;
CREATE TABLE chucho_event
(
   event_id INTEGER PRIMARY KEY AUTOINCREMENT,
   formatted_message TEXT NOT NULL,
   timestmp INTEGER NOT NULL,
   file_name TEXT NOT NULL,
   line_number INTEGER NOT NULL,
   function_name TEXT NOT NULL,
   logger TEXT NOT NULL,
   level_name TEXT NOT NULL,
   marker TEXT,
   thread TEXT NOT NULL,
   host_name TEXT NOT NULL,
   process_id INTEGER NOT NULL
);
COMMIT;
