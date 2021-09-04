-- Tablespasces
create tablespace TS_KOA
    datafile 'C:\app\tablespaces\pdb_main\TS_KOA.dbf'
    size 7M
    autoextend on next 5M
    maxsize 20M;

create temporary tablespace TS_KOA_TEMP
    tempfile 'C:\app\tablespaces\pdb_main\TS_KOA_TEMP.dbf'
    size 7M
    autoextend on next 5M
    maxsize 20M
    extent management local;

Select TABLESPACE_NAME, STATUS, contents logging from SYS.DBA_TABLESPACES;

Select file_name, tablespace_name, status from  dba_data_files
    union
    Select file_name, tablespace_name, status from dba_temp_files;
   
drop tablespace TS_KOA;
drop tablespace TS_KOA_TEMP;

-- Role
create role RL_KOA_CORE;
grant create session,
      create table, drop any table, 
      create view, drop any view,
      create procedure, drop any procedure
      to RL_KOA_CORE;

select * from dba_roles where role like 'RL%';
select * from dba_sys_privs where grantee = 'RL_KOA_CORE';

-- Profile
create profile PF_KOA_CORE limit
  password_life_time 180
  sessions_per_user 3
  failed_login_attempts 7
  password_lock_time 1
  password_reuse_time 10
  password_grace_time default
  connect_time 180
  idle_time 30;
  
select * from dba_profiles where profile = 'PF_KOA_CORE';
select * from dba_profiles where profile = 'DEFAULT';

-- User
create user U_KOA_CORE identified by Database_123098 --password
  default tablespace TS_KOA quota unlimited on TS_KOA
  temporary tablespace TS_KOA_TEMP
  profile PF_KOA_CORE
  account unlock 
  password expire;
  
grant RL_KOA_CORE to U_KOA_CORE;

-- Tables
create table TABLE_1
( 
  x number(3), 
  y varchar2(50)
);

drop table TABLE_1;
  
create view VIEW_1 (x, y) as select * from TABLE_1; 

-- Create table in tablespace
create tablespace KOA_QDATA
    datafile 'C:\app\tablespaces\pdb_main\KOA_QDATA.dbf'
    size 10M
    autoextend on next 5M
    maxsize 20M
    offline;

alter tablespace KOA_QDATA online;

alter user U_KOA_CORE quota 2M on KOA_QDATA;

create table KOA_T1
( 
  x number(3), 
  y varchar2(50)
) tablespace KOA_QDATA;

insert into KOA_T1
    values (1, 'a');
    
insert into KOA_T1
    values (2, 'b');
    
insert into KOA_T1
    values (3, 'c');
