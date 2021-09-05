-- create table
create table KOA_t
(
    x number(3) primary key,
    s varchar2(50)
);

-- insert
insert into KOA_t
    values (1, 'a');
    
insert into KOA_t
    values (2, 'b');
    
insert into KOA_t
    values (3, 'c');
Commit;

-- update
update HDV_T set x = 10, s = 'b' where x = 1;
update HDV_T set x = 10, s = 'c' where x = 2;
commit;

-- condition
select count(*) from KOA_t;
select * from KOA_t where s = 'c';

-- delete 
delete from KOA_t where x = 3;
commit;

-- create table references
create table KOA_t1
(
    x number(3), 
    s varchar2(50),
    constraint connection foreign key (x) REFERENCES KOA_t(x)
);

insert into KOA_t1 (x, s)
values (1,'iii');

insert into KOA_t1 (x, s)
values (2, 'b');
commit; 

select t.x, t.s, t1.s from KOA_t t inner join KOA_t1 t1 on t.x = t1.x;

select t.x, t.s, t1.s from KOA_t t left join KOA_t1 t1 on t.x = t1.x;

select t.x, t.s, t1.s from KOA_t t right join KOA_t1 t1 on t.x = t1.x;

-- drop
drop table KOA_T;
drop table KOA_T1;