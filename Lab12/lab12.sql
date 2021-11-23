-- task 1. add two columns to teacher
alter table TEACHER add BIRTHDAY date;
alter table TEACHER add SALARY number(6,2);

DECLARE
    cursor c_teacher is select TEACHER, BIRTHDAY, SALARY from TEACHER;
    l_data TEACHER.BIRTHDAY % type;
    l_salary TEACHER.SALARY % type;
BEGIN
    for row_teacher in c_teacher
    loop
        l_data := TO_DATE(TRUNC(DBMS_RANDOM.VALUE(TO_CHAR(DATE '1960-01-01','J'), TO_CHAR(DATE '2000-12-31','J'))),'J');
        SYS.DBMS_OUTPUT.PUT_LINE(l_data);
        
        l_salary := Round(DBMS_RANDOM.Value(20000, 200000)) / 100;
        SYS.DBMS_OUTPUT.PUT_LINE(l_salary);

        update TEACHER set TEACHER.BIRTHDAY = l_data, TEACHER.SALARY = l_salary
            where TEACHER.TEACHER = row_teacher.TEACHER;

    end loop;
END;

select * from TEACHER;

-- task 2. printing teachers 
DECLARE
    LastName   TEACHER.TEACHER_NAME % type;
    FirstName  TEACHER.TEACHER_NAME % type;
    Patronymic TEACHER.TEACHER_NAME % type;
    pos_name INT:= 0;
    pos_patronymic INT:= 0;
    CURSOR c_teacher IS SELECT TEACHER_NAME FROM TEACHER;
BEGIN
    for l_teachername in c_teacher
    loop
        pos_name := INSTR(l_teachername.TEACHER_NAME, ' ');
        pos_patronymic := INSTR(l_teachername.TEACHER_NAME, ' ', pos_name + 1);

        LastName := SUBSTR(l_teachername.TEACHER_NAME, 1, pos_name);

        FirstName := SUBSTR(l_teachername.TEACHER_NAME, pos_name, pos_patronymic - 1);
        FirstName := SUBSTR(FirstName, 1, 2);

        Patronymic := SUBSTR(l_teachername.TEACHER_NAME, pos_patronymic);
        Patronymic := SUBSTR(Patronymic, 1, 2);

        SYS.DBMS_OUTPUT.PUT_LINE(LastName || ' ' || FirstName || '.' || Patronymic || '.');

    end loop;
END;

-- task 3. teachres born in monday
select * from TEACHER where extract(month from BIRTHDAY) = 1;

-- task 4. teachers born in next month
create view TEACHER_BIRTHDAY_NEXT_MONTH as
    select * from TEACHER
    where to_char(BIRTHDAY, 'MONTH') = to_char(sysdate + 30, 'MONTH');

select * from TEACHER_BIRTHDAY_NEXT_MONTH;

-- task 5. count teachers by birthday's month
create view TEACHER_BIRTHDAY_BY_MONTH as select
    sum(case when extract(month from BIRTHDAY) = 1  then 1 else 0 end) as "January",
    sum(case when extract(month from BIRTHDAY) = 2  then 1 else 0 end) as "February ",
    sum(case when extract(month from BIRTHDAY) = 3  then 1 else 0 end) as "March ",
    sum(case when extract(month from BIRTHDAY) = 4  then 1 else 0 end) as "April ",
    sum(case when extract(month from BIRTHDAY) = 5  then 1 else 0 END) as "May",
    sum(case when extract(month from BIRTHDAY) = 6  then 1 else 0 end) as "June",
    sum(case when extract(month from BIRTHDAY) = 7  then 1 else 0 end) as "Jule",
    sum(case when extract(month from BIRTHDAY) = 8  then 1 else 0 end) as "August",
    sum(case when extract(month from BIRTHDAY) = 9  then 1 else 0 end) as "September",
    sum(case when extract(month from BIRTHDAY) = 10 then 1 else 0 end) as "October",
    sum(case when extract(month from BIRTHDAY) = 11 then 1 else 0 end) as "November",
    sum(case when extract(month from BIRTHDAY) = 12 then 1 else 0 end) as "December"
from TEACHER;

select * from TEACHER_BIRTHDAY_BY_MONTH;

-- task 6. printing teachers that have anniversary in next year via cursor
DECLARE
    birthday_year int;
    cursor c_teacher is select * from TEACHER;
    row_teacher TEACHER % rowtype;
BEGIN
    for row_teacher in c_teacher
    loop
        birthday_year := to_number(to_char(sysdate, 'YYYY')) + 1 - to_number(to_char(row_teacher.BIRTHDAY, 'YYYY'));

        if (mod(birthday_year, 10) = 0)
            then
                SYS.DBMS_OUTPUT.PUT_LINE(row_teacher.TEACHER_NAME|| ' '|| row_teacher.BIRTHDAY|| ' years: '|| birthday_year);
        end if;
    end loop;
END;

-- task 7. salaries
DECLARE
    SUM_SALARY INT := 0;
    QUANTITY_IN_FACULTY INT := 0;
    cursor c_teacher is select * from TEACHER;
    cursor c_pulpit is select * from PULPIT;
    cursor c_faculty is select * from FACULTY;
BEGIN
    SYS.DBMS_OUTPUT.PUT_LINE('average salaries by pulpits');
    for row_pulpit in c_pulpit
    loop
        for row_teacher in c_teacher
        loop
            if row_teacher.PULPIT = row_pulpit.PULPIT
            then
                SUM_SALARY := SUM_SALARY + row_teacher.SALARY;
                QUANTITY_IN_FACULTY := QUANTITY_IN_FACULTY + 1;
            end if;
        end loop;

        SYS.DBMS_OUTPUT.PUT_LINE(row_pulpit.PULPIT_NAME || ', avg salary: ' || floor(SUM_SALARY/QUANTITY_IN_FACULTY));
        QUANTITY_IN_FACULTY := 0;
        SUM_SALARY := 0;
    end loop;
    
    SYS.DBMS_OUTPUT.PUT_LINE('');
    SYS.DBMS_OUTPUT.PUT_LINE('average salaries by faculties');
    for row_faculty in c_faculty
    loop
        for row_pulpit in c_pulpit
        loop
            if row_faculty.FACULTY = row_pulpit.FACULTY
            then
                for row_teacher in c_teacher
                loop
                    if row_teacher.PULPIT = row_pulpit.PULPIT
                    then
                        SUM_SALARY := SUM_SALARY + row_teacher.SALARY;
                        QUANTITY_IN_FACULTY := QUANTITY_IN_FACULTY + 1;
                    end if;
                end loop;
            end if;
        end loop;
        
        SYS.DBMS_OUTPUT.PUT_LINE(row_faculty.FACULTY_NAME || ', avg salary: ' || floor(SUM_SALARY/QUANTITY_IN_FACULTY));
        QUANTITY_IN_FACULTY := 0;
        SUM_SALARY := 0;
    end loop;

    for row_teacher in c_teacher
        loop
            SUM_SALARY := SUM_SALARY + row_teacher.SALARY;
            QUANTITY_IN_FACULTY := QUANTITY_IN_FACULTY + 1;
    end loop;

    SYS.DBMS_OUTPUT.PUT_LINE('');
    SYS.DBMS_OUTPUT.PUT_LINE('average ' || floor(SUM_SALARY/QUANTITY_IN_FACULTY));
END;

-- tsak 8. record types
DECLARE
    TYPE DETAIL_STUDENT IS RECORD
    (
         FACULTY VARCHAR2(50),
         COURSE INT
    );
    TYPE STUDENT IS RECORD
    (
        NAME2 VARCHAR2(30),
        LASTNAME VARCHAR2(30),
        OLD2 INT,
        DETAIL DETAIL_STUDENT
    );
    student_1 STUDENT;
    student_2 STUDENT;
BEGIN
    student_1.NAME2 := 'Oleg';
    student_1.LASTNAME := 'Kaportsev';
    student_1.OLD2 := 20;
    student_1.DETAIL.FACULTY := 'FIT';
    student_1.DETAIL.COURSE := 3;

    student_2 := student_1;

    SYS.DBMS_OUTPUT.PUT_LINE(
        'Name: ' || student_2.NAME2 ||' '||
        'Lastname: ' || student_2.LASTNAME ||' '||
        'Old: ' || student_2.OLD2 ||' '||
        'Faculty: ' || student_2.DETAIL.FACULTY ||' '||
        'Course: ' || student_2.DETAIL.COURSE
    );
END;








