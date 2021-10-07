-- TASK 1-3. background processes
--full list of background processes
select name, description from v$bgprocess;

-- count of background processes that are working now
select name, description from v$bgprocess where paddr != hextoraw('00');

-- DBW count that are working now
select count(*) from v$bgprocess where paddr != hextoraw('00') and name like 'DBW%';

show parameter db_writer_processes; 

-- TASK 4-5. current sessions
select paddr, username, status, server  from v$session where username is not null;

-- TASK 6. current services
select name, network_name, pdb from v$services;

-- TASK 7. dispatcher params
alter system set max_dispatchers = 10;
show parameter dispatcher;

-- TASK 12. Services that serviced by LISTENER 
select ses.paddr, ses.username, ses.status, ses.server  from v$session ses
join v$process pr on ses.paddr = pr.addr where ses.username is not null and pr.background is null;

--OracleOraDB12Home4TNSListener
--C:\app\User\product\12.2.0\dbhome_1\network\admin\listener.ora