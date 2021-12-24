-- TASK 1. Create table
create table student(
    id int primary key,
    name varchar2(50),
    lastname varchar2(50)
);

insert into student(id,name, lastname) values (1, 'Иван', 'Северин');
insert into student(id,name, lastname) values (2, 'Кирилл', 'Кабайло');
insert into student(id,name, lastname) values (3, 'Дмитрий', 'Моргунов');
insert into student(id,name, lastname) values (4, 'Аннф', 'Кордюкова');
insert into student(id,name, lastname) values (5, 'Иван', 'Гурин');
insert into student(id,name, lastname) values (6, 'Андрей', 'Акунович');
insert into student(id,name, lastname) values (7, 'Анна', 'Колесникова');
insert into student(id,name, lastname) values (8, 'Денис', 'Цуранов');
insert into student(id,name, lastname) values (9, 'Виктор', 'Барановский');
insert into student(id,name, lastname) values (10, 'Валера', 'Макелин');

select * from student;
-- TASK 3. Create trigger before (insert, delete, update)
create or replace trigger student_before
    before insert or update or delete on student
begin
    SYS.DBMS_OUTPUT.PUT_LINE('Before');
end;

insert into student(id,name, lastname) values (15, 'Денис', 'Макелин');
update student set name = 'Василий' where id = 15;
delete student WHERE id = 15;

select * from student;

drop trigger student_before;

-- TASK 5. Create trigger before each row (insert, delete, update)
create or replace trigger students_before_each_rows
    before insert or update or delete
        on student for each row
begin
    SYS.DBMS_OUTPUT.PUT_LINE('Before each row');
end;

update student set name = 'Василий' where name = 'Денис';

select * from student;
rollback;

drop trigger students_before_each_rows;

-- TASK 7. Create trigger after (insert, delete, update)
create or replace trigger student_after
    after insert or update or delete on student
begin
    SYS.DBMS_OUTPUT.PUT_LINE('After');
end;

insert into student(id,name, lastname) values (15,'Денис', 'Макелин');
update student set name = 'Василий' where id = 15;
delete student WHERE id = 15;

select * from student;

drop trigger student_after;

-- TASK 8 create triggers dml each rows
create or replace trigger students_after_each_row
    after insert or update or delete on student
        for each row
begin
    SYS.DBMS_OUTPUT.PUT_LINE('After each row');
end;

insert into student(id,name, lastname) values (15,'Денис', 'Макелин');
insert into student(id,name, lastname) values (16,'Денис', 'Макелин');
insert into student(id,name, lastname) values (17,'Денис', 'Макелин');

update student set name = 'Васютин' where lastname = 'Макелин';
delete student WHERE id >= 15;

select * from student;

drop trigger student_after_insert_each_row;
drop trigger student_after_update_each_row;
drop trigger student_after_delete_each_row;

-- TASK 9 create TABLE AUDIT
CREATE TABLE AUDIT_CUSTOM
(
    OperationDate date,
    OperationType varchar2(10 char),
    TriggerName varchar(50 char),
    DATA varchar2(200 char)
);

-- TASK 10 && TASK 6
create or replace trigger student_logger_predicate
    after insert or update or delete on student
        for each row
begin
    if inserting then
            insert into AUDIT_CUSTOM (OperationDate, OperationType, TriggerName, DATA)
                values (
                        sysdate,
                        'insert',
                        'student_logger_predicate',
                        'Old name: ' || :old.name || 'New name: ' || :new.name ||
                        'Old lastname: ' || :old.lastname || 'New lastname: ' || :new.lastname);
    elsif updating then
            insert into AUDIT_CUSTOM (OperationDate, OperationType, TriggerName, DATA)
                values (
                        sysdate,
                        'update',
                        'student_logger_predicate',
                        'Old name: ' || :old.name || 'New name: ' || :new.name ||
                        'Old lastname: ' || :old.lastname || 'New lastname: ' || :new.lastname);
    elsif deleting
        then
            insert into AUDIT_CUSTOM (OperationDate, OperationType, TriggerName, DATA)
                values (
                        sysdate,
                        'delete',
                        'student_logger_predicate',
                        'Old name: ' || :old.name || ', New name: ' || :new.name ||
                        '; Old lastname: ' || :old.lastname || ', New lastname: ' || :new.lastname);
    end if;
end;

-- drop trigger student_logger_predicate;

insert into student values (21, 'Валентина', 'Маргунова');
update student set name = 'Екатерина' where name = 'Валентина';
delete from student where lastname = 'Маргунова';

select * from AUDIT_CUSTOM;

-- TASK 11
insert into student values (1, '', '');
select * from user_triggers;

-- TASK 12
drop table student;

create or replace trigger ban_drop_student
  before drop on database
begin
    if(ORA_DICT_OBJ_NAME  = 'STUDENT') THEN
    SYS.DBMS_OUTPUT.PUT_LINE('delete function: banned');
    RAISE_APPLICATION_ERROR(-20000,'Cant delete this table');
    END IF;
END;

-- TASK 13
drop table AUDIT_CUSTOM;
select * from user_triggers;

-- TASK 14
create or replace trigger trnewtab
instead of insert or update or delete on vnewtab
for each row
begin
  if inserting then dbms_output.put_line('insert:'||:new.x);
  else if updating then dbms_output.put_line('update:'||rtrim(:old.y) ||'->'||:new.y);
  else if deleting then dbms_output.put_line('delete:'||:old.x);
  end if;
end if;
end;








