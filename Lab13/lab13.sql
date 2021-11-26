-- task 1. local procedure -> printing teachers with set pulpit
declare
    procedure get_teachers(pcode in TEACHER.PULPIT % type)
        is
            cursor c_teacher is select * from TEACHER where pulpit = pcode;
        begin
            for row_teacher in c_teacher
            loop
                dbms_output.put_line(row_teacher.TEACHER_NAME || ', ' || row_teacher.PULPIT);
            end loop;
        exception
            when others then dbms_output.put_line(sqlerrm);
        end get_teachers;
begin
    get_teachers('ศั่า');
end;

-- task 2. local func -> return teachers amount with set pulpit
declare
    function get_num_teachers(pcode TEACHER.PULPIT % type)
        return number
        is
            teachers_amount number := 0;
        begin
            select count(*) into teachers_amount from teacher where pulpit = pcode;
            return teachers_amount;
        exception
            when others then dbms_output.put_line(sqlerrm);
        end get_num_teachers;
begin
     dbms_output.put_line(get_num_teachers('ศั่า'));
end;

-- task 3. procedures
create or replace procedure get_teachers(fcode in FACULTY.FACULTY % type)
is
    cursor c_teacher is
        select T.TEACHER_NAME, T.PULPIT from TEACHER T inner join PULPIT P 
        on T.PULPIT = P.PULPIT
        where P.FACULTY = fcode;
begin
    dbms_output.put_line('teachers from faculty ' || fcode);
    for row_teacher in c_teacher
    loop
        dbms_output.put_line(row_teacher.TEACHER_NAME || ', pulpit: ' || TRIM(row_teacher.PULPIT));
    end loop;
exception
    when others then dbms_output.put_line(sqlerrm);
end get_teachers;

create or replace procedure get_subjects(pcode in SUBJECT.PULPIT % type)
is
    cursor c_subject is select * from SUBJECT where pulpit = pcode;
begin
    dbms_output.put_line('subjects from pulpit ' || pcode);
    for row_subject in c_subject
    loop
        dbms_output.put_line(row_subject.SUBJECT_NAME);
    end loop;
exception
    when others then dbms_output.put_line(sqlerrm);
end get_subjects;

begin
    get_teachers('าฮย');
    dbms_output.put_line('');
    get_subjects('ศั่า');
end;

-- task 4. functions, same as latets procedures
create or replace function get_num_teachers(fcode FACULTY.FACULTY % type)
return number
is
    teachers_amount number := 0;
begin
    select count(*) into teachers_amount
        from TEACHER T 
        inner join PULPIT P on T.PULPIT = P.PULPIT
        where P.FACULTY = fcode;
    return teachers_amount;
exception
    when others then dbms_output.put_line(sqlerrm);
end get_num_teachers;

create or replace function get_num_subjects(pcode SUBJECT.PULPIT % type)
return number
is
    subjects_amount number := 0;
begin
    select count(*) into subjects_amount from SUBJECT where pulpit = pcode; 
    return subjects_amount;
exception
    when others then dbms_output.put_line(sqlerrm);
end get_num_subjects;

begin
    dbms_output.put_line(get_num_teachers('าฮย'));
    dbms_output.put_line(get_num_subjects('ศั่า'));
end;

-- task 5-6. package
create or replace package teachers as
    procedure get_teachers (fcode in FACULTY.FACULTY % type);
    procedure get_subjects(pcode in SUBJECT.PULPIT % type);
    function get_num_teachers(fcode FACULTY.FACULTY % type) return number;
    function get_num_subjects(pcode SUBJECT.PULPIT % type) return number;
end teachers;

create or replace package body teachers as
    procedure get_teachers(fcode in FACULTY.FACULTY % type)
        is
            cursor c_teacher is
                select T.TEACHER_NAME, T.PULPIT from TEACHER T inner join PULPIT P 
                on T.PULPIT = P.PULPIT
                where P.FACULTY = fcode;
        begin
            dbms_output.put_line('teachers from faculty ' || fcode);
            for row_teacher in c_teacher
            loop
                dbms_output.put_line(row_teacher.TEACHER_NAME || ', pulpit: ' || TRIM(row_teacher.PULPIT));
            end loop;
        exception
            when others then dbms_output.put_line(sqlerrm);
        end get_teachers;

    procedure get_subjects(pcode in SUBJECT.PULPIT % type)
        is
            cursor c_subject is select * from SUBJECT where pulpit = pcode;
        begin
            dbms_output.put_line('subjects from pulpit ' || pcode);
            for row_subject in c_subject
            loop
                dbms_output.put_line(row_subject.SUBJECT_NAME);
            end loop;
        exception
            when others then dbms_output.put_line(sqlerrm);
        end get_subjects;

    function get_num_teachers(fcode FACULTY.FACULTY % type) return number
        is
            teachers_amount number := 0;
        begin
            select count(*) into teachers_amount
                from TEACHER T 
                inner join PULPIT P on T.PULPIT = P.PULPIT
                where P.FACULTY = fcode;
            return teachers_amount;
        exception
            when others then dbms_output.put_line(sqlerrm);
        end get_num_teachers;
        
    function get_num_subjects(pcode SUBJECT.PULPIT % type) return number
        is
            subjects_amount number := 0;
        begin
            select count(*) into subjects_amount from SUBJECT where pulpit = pcode; 
            return subjects_amount;
        exception
            when others then dbms_output.put_line(sqlerrm);
        end get_num_subjects;    
end teachers;

begin
    dbms_output.put_line(teachers.get_num_teachers('าฮย'));
    dbms_output.put_line(teachers.get_num_subjects('ศั่า'));
end;



