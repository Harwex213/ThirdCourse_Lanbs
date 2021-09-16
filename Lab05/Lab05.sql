-- tablespace
create tablespace KOA_QDATA
    datafile 'C:\app\tablespaces\pdb_main\KOA_QDATA.dbf'
    size 10M
    autoextend on next 5M
    maxsize 20M
    offline;

alter tablespace KOA_QDATA online;

alter user U_KOA_CORE quota 2M on KOA_QDATA;

-- tablespace files
Select file_name, tablespace_name, status from  dba_data_files
    union
    Select file_name, tablespace_name, status from dba_temp_files;
    
-- TABLE_1
create table KOA_TABLE_1
( 
  x number(38,0), 
  y varchar2(50)
) tablespace KOA_QDATA;

select table_name, tablespace_name from USER_TABLES;

insert into KOA_TABLE_1 
values (1, 'a');

insert into KOA_TABLE_1 
values (2, 'b');

insert into KOA_TABLE_1 
values (3, 'c');

select * from KOA_TABLE_1;

-- segments 
drop table KOA_TABLE_1;

select segment_name, segment_type, tablespace_name 
from user_segments
where tablespace_name='KOA_QDATA';

select * from USER_RECYCLEBIN;

flashback table KOA_TABLE_1 to before drop;

-- insertion
declare 
x int:=0;
BEGIN
while x<10000
  LOOP
  insert into KOA_TABLE_1 values (x, 'orcl the best');
  x:=x+1;
  end loop;
END;

select count(*) from KOA_TABLE_1;

-- extents
select segment_name, segment_type, tablespace_name, bytes, blocks, extents, buffer_pool
from user_segments
where tablespace_name='KOA_QDATA'
order by bytes desc;

select segment_name, segment_type, tablespace_name, bytes, blocks, extents, buffer_pool
from user_segments
order by tablespace_name, bytes desc;

-- log files
drop tablespace KOA_QDATA including contents and datafiles;

select GROUP#, SEQUENCE#, BYTES, MEMBERS, STATUS, FIRST_CHANGE# from v$log;
select * from v$logfile;

-- switch log
alter system switch logfile;

-- add log & logfiles
alter database add logfile group 4 'C:\app\oracle\oradata\orcl\REDO04.LOG'
size 50m blocksize 512;

alter database add logfile member 'C:\app\oracle\oradata\orcl\REDO041.LOG' to GROUP 4;
alter database add logfile member 'C:\app\oracle\oradata\orcl\REDO042.LOG' to GROUP 4;
alter database add logfile member 'C:\app\oracle\oradata\orcl\REDO043.LOG' to GROUP 4;

-- remove log & logfiles
alter database drop logfile member 'C:\app\oracle\oradata\orcl\REDO04.LOG';
alter database drop logfile member 'C:\app\oracle\oradata\orcl\REDO041.LOG';
alter database drop logfile member 'C:\app\oracle\oradata\orcl\REDO042.LOG';
alter database drop logfile member 'C:\app\oracle\oradata\orcl\REDO043.LOG';

alter database drop Logfile group 4;

-- turn on archievelog
select name, log_mode from v$database;
select instance_name, archiver, active_state from v$instance;

select * from v$log;
select * from v$archived_log;

-- control files
select name from v$controlfile;

-- pfile
create pfile = 'KOA_PFILE.ora' from spfile;

-- log.xml
select * from v$diag_info;
















