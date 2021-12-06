ALTER SESSION SET nls_date_format='dd-mm-yyyy hh24:mi:ss';

ALTER SYSTEM SET JOB_QUEUE_PROCESSES = 5;

-- tables for tasks
create table teacher_for_job
(
  teacher varchar(200),
  teacher_name varchar(200),
  pulpit varchar(100)
);

create table teacher_for_job_2
(
  teacher varchar(200),
  teacher_name varchar(200),
  pulpit varchar(100)
);

create or replace procedure reinsert_teacher is
  cursor tcursor is select * from teacher;
begin
    delete teacher_for_job;
    for n in tcursor
    loop
      insert into teacher_for_job (teacher, teacher_name, pulpit) values (n.teacher, n.teacher_name, n.pulpit);
    end loop;
    commit;
end reinsert_teacher;

select * from teacher_for_job;
select * from teacher_for_job_2;

delete teacher_for_job;
delete teacher_for_job_2;

create table job_status
(
  status   nvarchar2(50),
  error_message nvarchar2(500),
  datetime date default sysdate
);

select * from job_status;

-- procedure
create or replace procedure teacher_job is
  err_message varchar2(500);
  cursor c_teacher is select * from teacher_for_job where rtrim(PULPIT) = 'ศั่า';
begin
    for n in c_teacher
      loop
        insert into teacher_for_job_2 (teacher, teacher_name, pulpit) values (n.teacher, n.teacher_name, n.pulpit);
      end loop;
    delete from teacher_for_job where rtrim(PULPIT) = 'ศั่า';
    insert into job_status (status) values ('SUCCESS');
    commit;
exception
  when others then
      err_message := sqlerrm;
      insert into job_status (status, error_message) values ('FAIL', err_message);
      rollback;
end teacher_job;

begin
    reinsert_teacher();
--    teacher_job();
end;

-- dbms_job
declare job_number user_jobs.job%type;
begin
  dbms_job.submit(job_number, 'BEGIN teacher_job(); END;', sysdate, 'sysdate + 10/24');
  commit;
  dbms_output.put_line(job_number);
end;

select job, what, last_date, last_sec, next_date, next_sec, broken from user_jobs;
select job, what, last_date, last_sec, next_date, next_sec, broken from dba_jobs;
delete user_jobs where job = 21;

begin
  dbms_job.run(22);
end;

begin
  dbms_job.broken(21, true, null);
end;

begin
  dbms_job.remove(21);
end;

begin
  dbms_job.isubmit(99, 'BEGIN teacher_job(); END;', sysdate, 'sysdate + 10/24');
end;

-- dbms_sheldure
begin
dbms_scheduler.create_schedule(
  schedule_name => 'Schedule_1',
  start_date => sysdate,
  repeat_interval => 'FREQ=DAILY',
  comments => 'Schedule_1 DAILY start now'
);
end;

begin
dbms_scheduler.create_program(
  program_name => 'Program_1',
  program_type => 'STORED_PROCEDURE',
  program_action => 'teacher_job',
  number_of_arguments => 0,
  enabled => true,
  comments => 'Program_1 - teacher_job'
);
end;

select * from  user_scheduler_programs;
select * from user_scheduler_schedules;

begin
dbms_scheduler.create_job(
  job_name => 'Job_1',
  program_name => 'Program_1',
  schedule_name => 'Schedule_1',
  enabled => true
);
end;

begin
  DBMS_SCHEDULER.DISABLE('Job_1');
end;

begin
  DBMS_SCHEDULER.RUN_JOB('Job_1');
end;

begin
  DBMS_SCHEDULER.STOP_JOB('Job_1');
end;

begin
  DBMS_SCHEDULER.DROP_JOB( JOB_NAME => 'Job_1');
end;










