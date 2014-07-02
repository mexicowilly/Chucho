-- 
--  Copyright 2013-2014 Will Mason
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
    event_id NUMBER(10) PRIMARY KEY,
    formatted_message VARCHAR2(4000) NOT NULL,
    timestmp TIMESTAMP(0) NOT NULL,
    file_name VARCHAR2(1024) NOT NULL,
    line_number NUMBER(10, 0) NOT NULL,
    function_name VARCHAR2(256) NOT NULL,
    logger VARCHAR2(1024) NOT NULL,
    level_name VARCHAR2(256) NOT NULL,
    marker VARCHAR2(1024),
    thread VARCHAR2(256) NOT NULL
);

CREATE SEQUENCE chucho_event_id_seq MINVALUE 1 START WITH 1;

CREATE TRIGGER chucho_event_id_seq_trigger
    BEFORE INSERT ON chucho_event
    FOR EACH ROW
    BEGIN
       SELECT chucho_event_id_seq.NEXTVAL INTO :NEW.event_id FROM DUAL;
    END;
/
