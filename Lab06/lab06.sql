-- general size of SGA
select * from v$sga;

-- sizes of general components of SGA + Granule size
select component,
      current_size,
      max_size,
      granule_size from v$sga_dynamic_components
      
-- free memory
select current_size from v$sga_dynamic_free_memory;

-- size Default, Keep, Recycle
select component, current_size 
from v$sga_dynamic_components 
where component like '%DEFAULT%'
  or  component like '%KEEP%'
  or  component like '%RECYCLE%';
  
  alter system set large_pool_size = 12m;
  show parameter large_pool_size;
  
-- table keep
create table TestTable_keep (k int, s varchar(50)) storage(buffer_pool keep);

insert into TestTable_keep values (1, 'a');

-- table default
create table TestTable_default (k int, s varchar(50)) storage(buffer_pool default);

insert into TestTable_default values (1, 'a');

select * from TestTable_keep;
select * from TestTable_default;

select segment_name, segment_type, tablespace_name, buffer_pool 
from user_segments 
where segment_name in('TESTTABLE_KEEP', 'TESTTABLE_DEFAULT');

-- buffer pool (SGA)
show parameter log_buffer;

-- 10 biggest objects of shared pool (SGA)
select pool, name, bytes 
from v$sgastat 
where pool = 'shared pool'
order by bytes desc fetch first 10 rows only;

-- free memory of large pool (SGA)
select pool, name, bytes from v$sgastat where pool = 'large pool' and name = 'free memory';

-- current sessions
select username, service_name, server
from v$session 
where username is not null;





