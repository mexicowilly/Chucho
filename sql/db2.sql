-- 
--  Copyright 2013-2016 Will Mason
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

CREATE TABLE chucho_event
(
    event_id INTEGER NOT NULL GENERATED ALWAYS AS IDENTITY (START WITH 1, INCREMENT BY 1, NO CACHE),
    formatted_message VARCHAR(4000) NOT NULL,
    timestmp TIMESTAMP NOT NULL,
    file_name VARCHAR(1024) NOT NULL,
    line_number INTEGER NOT NULL,
    function_name VARCHAR(256) NOT NULL,
    logger VARCHAR(1024) NOT NULL,
    level_name VARCHAR(256) NOT NULL,
    marker VARCHAR(1024),
    thread VARCHAR(256) NOT NULL
);