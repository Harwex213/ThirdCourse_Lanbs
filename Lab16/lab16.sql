ALTER SESSION SET nls_date_format='dd-mm-yyyy hh24:mi:ss';
alter database datafile '/u01/app/oracle/oradata/ORCL/orclpdb/TS_SKVORTSOFF.DBF' autoextend on maxsize unlimited;

-- TASK 1 create T_RANGE key id
create table T_RANGE
(
  id number,
  time_id date
)
partition by range(id)
(
  partition p1 values less than (100),
  partition p2 values less than (200),
  partition p3 values less than (300),
  partition pmax values less than (maxvalue)
);

insert into T_RANGE(id, time_id) values(1,'01-02-2018');
insert into T_RANGE(id, time_id) values(150,'01-02-2017');
insert into T_RANGE(id, time_id) values(250,'01-02-2016');
insert into T_RANGE(id, time_id) values(350,'01-02-2015');
commit;

select * from T_RANGE partition(p1);
select * from T_RANGE partition(p2);
select * from T_RANGE partition(p3);
select * from T_RANGE partition(pmax);

select * from user_tab_partitions where TABLE_NAME = 'T_RANGE';
-- drop table T_RANGE;

-- TASK 2 create T_INTERVAL key date
create table T_INTERVAL
(
  id number,
  time_id date
)
partition by range(time_id) interval (numtoyminterval(1,'month'))
(
  partition p0 values less than  (to_date ('1-4-2009', 'dd-mm-yyyy')),
  partition p1 values less than  (to_date ('1-8-2015', 'dd-mm-yyyy')),
  partition p2 values less than  (to_date ('1-12-2018', 'dd-mm-yyyy'))
);
-- drop table  T_INTERVAL;

insert into T_INTERVAL(id, time_id) values(50,'01-03-2008');
insert into T_INTERVAL(id, time_id) values(105,'01-05-2009');
insert into T_INTERVAL(id, time_id) values(105,'01-06-2014');
insert into T_INTERVAL(id, time_id) values(205,'01-01-2015');
insert into T_INTERVAL(id, time_id) values(305,'01-10-2016');
insert into T_INTERVAL(id, time_id) values(405,'01-02-2018');
insert into T_INTERVAL(id, time_id) values(505,'01-01-2019');
insert into T_INTERVAL(id, time_id) values(1005,'01-01-2019');
commit;

select * from T_INTERVAL partition(p0);
select * from T_INTERVAL partition(p1);
select * from T_INTERVAL partition(p2);
select * from T_INTERVAL partition(SYS_P1501);
select * from T_INTERVAL;

select * from user_tab_partitions where TABLE_NAME = 'T_INTERVAL';

-- TASK 3 create T_HASH key VARCHAR2
create table T_HASH
(
  str varchar2 (50),
  id number
)
partition by hash (str)
(
  partition k1,
  partition k2,
  partition k3,
  partition k4
);

insert into T_HASH (str,id) values('string1', 1);
insert into T_HASH (str,id) values('string2', 2);
insert into T_HASH (str,id) values('string3', 3);
insert into T_HASH (str,id) values('string4', 4);
insert into T_HASH (str,id) values('string5', 5);
commit;

select * from T_HASH partition(k1);
select * from T_HASH partition(k2);
select * from T_HASH partition(k3);
select * from T_HASH partition(k4);

select * from user_tab_partitions where TABLE_NAME = 'T_HASH';

-- TASK 4 create T_LIST
create table T_LIST
(
  x char(3)
)
partition by list (x)
(
  partition p1 values ('a', 'b', 'c'),
  partition p2 values ('d', 'e', 'f'),
  partition p3 values ('g'),
  partition p4 values (default)
);

insert into  T_LIST(x) values('a');
insert into  T_LIST(x) values('b');
insert into  T_LIST(x) values('c');
insert into  T_LIST(x) values('d');
insert into  T_LIST(x) values('e');
insert into  T_LIST(x) values('x');
insert into  T_LIST(x) values('y');
commit;

select * from T_LIST partition (p1);
select * from T_LIST partition (p2);
select * from T_LIST partition (p3);
select * from T_LIST partition (p4);

-- TASK 6 update T_RANGE, T_LIST
alter table T_LIST enable row movement;
alter table T_RANGE enable row movement;

update T_LIST partition(p1)
    set x='b' where x = 'a';

update T_RANGE partition(p1)
    set id=1 where id = 10;

select * from T_LIST partition(p1);
select * from T_LIST;

-- TASK 7 alter table merge
alter table T_RANGE merge partitions p1,p2 into partition p5;
select * from T_RANGE partition(p5);

-- TASK 8 alter table split
alter table T_INTERVAL split partition p2 at (to_date ('1-06-2018', 'dd-mm-yyyy'))
  into (partition p6, partition p5);

select * from T_INTERVAL partition (p5);
select * from T_INTERVAL partition (p6);

-- TASK 9 alter table exchange
create table T_LIST1(x char(3));
alter table T_LIST exchange partition  p2 with table T_LIST1 without validation;

select * from T_LIST partition (p2);
select * from T_LIST1;

select * from USER_PART_TABLES;
select * from user_segments;