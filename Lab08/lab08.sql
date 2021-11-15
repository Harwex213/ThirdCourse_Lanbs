--2. instance parameters by system user
select name, description from v$parameter;

--3. connect to pdb and select pdbs list and their files, roles and users
-- connect system/password@localhost:1521/orcl
select * from dba_tablespaces;
select * from dba_data_files;
select * from dba_roles;
select * from dba_users;

-- 4. --
-- 5. --
-- 6. --
-- 7-8. select with timing
create table table_1
( 
  idkey int primary key,
  someText varchar(50)
);

declare 
x int:=0;
BEGIN
while x<100000
  LOOP
  insert into table_1 values (x, 'orcl the best');
  x:=x+1;
  end loop;
END;

set timing on
select * from table_1;
--time: 4.67 sec

-- 9. describe
describe table_1

-- 10. segments
select * from user_segments;

-- 11. view with segments, extents amount, blocks and size in KB
create view table_1_view as 
select count(*) segments_count, sum(extents) extents_count, sum(blocks) bloks_count, sum(bytes) memory_size from user_segments;

SELECT * FROM table_1_view;