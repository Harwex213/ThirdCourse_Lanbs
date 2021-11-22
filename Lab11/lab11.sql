-- task 1-2. exact && inexact selecting
DECLARE
    l_teacher TEACHER % rowtype;
BEGIN
    select * into l_teacher from TEACHER where TEACHER = 'СМЛВ';

    SYS.DBMS_OUTPUT.PUT_LINE(l_teacher.TEACHER || ' ' || l_teacher.TEACHER_NAME || ' ' || l_teacher.PULPIT);
    
EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlcode || ', ' || sqlerrm);
END;

-- task 3-4. exception too_many_rows; exception no_data_found; cursor's attrs;
DECLARE
    l_teacher TEACHER % rowtype;
BEGIN
    BEGIN
        select * into l_teacher from TEACHER;
    EXCEPTION
        when TOO_MANY_ROWS
            then
                SYS.DBMS_OUTPUT.PUT_LINE('TOO_MANY_ROWS');
        when others
            then
                SYS.DBMS_OUTPUT.PUT_LINE(' ' || sqlcode || ', ' || sqlerrm);
    END;
    BEGIN
        select * into l_teacher from TEACHER where TEACHER = 'СМЛВa';
    EXCEPTION
        when NO_DATA_FOUND
            then
                SYS.DBMS_OUTPUT.PUT_LINE('NO_DATA_FOUND');
        when others
            then
                SYS.DBMS_OUTPUT.PUT_LINE(' ' || sqlcode || ', ' || sqlerrm);
    END;
    BEGIN
        select * into l_teacher from TEACHER where TEACHER = 'СМЛВ';

        if (sql % found)
            then
                SYS.DBMS_OUTPUT.PUT_LINE('found row');
        else
                SYS.DBMS_OUTPUT.PUT_LINE('no found row');
        end if;
    
        if (sql % isopen)
            then
                SYS.DBMS_OUTPUT.PUT_LINE('cursor open');
        else
                SYS.DBMS_OUTPUT.PUT_LINE('cursor not open');
        end if;
    
        SYS.DBMS_OUTPUT.PUT_LINE('found rows: ' || (sql % rowcount));
        
        SYS.DBMS_OUTPUT.PUT_LINE(l_teacher.TEACHER || ' ' || l_teacher.TEACHER_NAME || ' ' || l_teacher.PULPIT);
    EXCEPTION
        when NO_DATA_FOUND
            then
                SYS.DBMS_OUTPUT.PUT_LINE('NO_DATA_FOUND');
        when others
            then
                SYS.DBMS_OUTPUT.PUT_LINE(' ' || sqlcode || ', ' || sqlerrm);
    END;
END;

-- task 5-6. update with commit/rollback;
BEGIN
    update  TEACHER set TEACHER_NAME = 'Жаровский Иван Михайлович' where TEACHER = 'ЖРСК';
    SYS.DBMS_OUTPUT.PUT_LINE('updated');
    
    update  TEACHER set TEACHER = 'ЖРСКjjjjjjjj' where TEACHER = 'ЖРСК';
    SYS.DBMS_OUTPUT.PUT_LINE('updated');

    commit;
EXCEPTION
    when others
        then
            rollback;
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 7-8. insert with commit/rollback;
BEGIN
    insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
        values ('КОА', 'Капорцев Олег Андреевич', 'ОВ');
    SYS.DBMS_OUTPUT.PUT_LINE('inserted');
    
    insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
        values ('КОА', 'Капорцев Олег Андреевич', 'ОВj');
    SYS.DBMS_OUTPUT.PUT_LINE('inserted');

    commit;
EXCEPTION
    when others
        then
            rollback;
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- TASK 9-10. delete with commit/rollback;
BEGIN
    delete from TEACHER where TEACHER = 'КОА';
    SYS.DBMS_OUTPUT.PUT_LINE('deleted rows: ' || (sql % rowcount));
    
    delete from TEACHER where TEACHER = 0;
    SYS.DBMS_OUTPUT.PUT_LINE('deleted rows: ' || (sql % rowcount));

    commit;
EXCEPTION
    when others
        then
            rollback;
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 11-13. Explicit cursor via loop, while, for in;
DECLARE
    cursor cl_teacher is select TEACHER, TEACHER_NAME, PULPIT from TEACHER where ROWNUM < 4;
    cursor cw_teacher is select TEACHER, TEACHER_NAME, PULPIT from TEACHER where ROWNUM < 4;
    cursor cf_teacher is select TEACHER, TEACHER_NAME, PULPIT from TEACHER where ROWNUM < 4;
    l_teacher       TEACHER.TEACHER % type;
    l_teacher_name  TEACHER.TEACHER_NAME % type;
    l_pulpit        TEACHER.PULPIT % type;
    row_teacher     TEACHER % rowtype;
BEGIN
    -- loop
    open cl_teacher;

    loop
        fetch cl_teacher into l_teacher, l_teacher_name, l_pulpit;
        exit when cl_teacher % notfound;

        SYS.DBMS_OUTPUT.PUT_LINE(l_teacher ||' '|| l_teacher_name ||' '|| l_pulpit);
    end loop;

    SYS.DBMS_OUTPUT.PUT_LINE('rows read: ' || cl_teacher % rowcount);
    SYS.DBMS_OUTPUT.PUT_LINE('');
    close cl_teacher;
    
    -- while
    open cw_teacher;
    fetch cw_teacher into row_teacher;
    
    while (cw_teacher % found)
    loop
        fetch cw_teacher into row_teacher;
        
        SYS.DBMS_OUTPUT.PUT_LINE(row_teacher.TEACHER ||' '|| row_teacher.TEACHER_NAME ||' '|| row_teacher.PULPIT);
    end loop;
    
    SYS.DBMS_OUTPUT.PUT_LINE('rows read: ' || cw_teacher % rowcount);
    SYS.DBMS_OUTPUT.PUT_LINE('');
    close cw_teacher;
    
    -- for
    for row_teacher in cf_teacher
    loop
        SYS.DBMS_OUTPUT.PUT_LINE(row_teacher.TEACHER ||' '|| row_teacher.TEACHER_NAME ||' '|| row_teacher.PULPIT);
    end loop;
    
    SYS.DBMS_OUTPUT.PUT_LINE('');
EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 14. printing auditoriums in groups by capacities
DECLARE
    cursor c_auditorium (min AUDITORIUM.AUDITORIUM % type, max AUDITORIUM.AUDITORIUM % type) is
        select * from AUDITORIUM
        where AUDITORIUM_CAPACITY >= min and AUDITORIUM_CAPACITY <= max
        for update;
    row_auditorium AUDITORIUM % rowtype;
BEGIN
    SYS.DBMS_OUTPUT.PUT_LINE('capacity 0 - 20');

    for row_auditorium in c_auditorium(0,20)
    loop
        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
    end loop;

    SYS.DBMS_OUTPUT.PUT_LINE('-----------------');
    SYS.DBMS_OUTPUT.PUT_LINE('capacity 21 - 30');

    ---------------------- while ----------------------
    open c_auditorium(21,30);
    fetch c_auditorium into row_auditorium;

    while (c_auditorium % found)
    loop
        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
        fetch c_auditorium into row_auditorium;
    end loop;
    close c_auditorium;

    SYS.DBMS_OUTPUT.PUT_LINE('-----------------');
    SYS.DBMS_OUTPUT.PUT_LINE('capacity 31 - 60');

    open c_auditorium(31,60);

    loop
        fetch c_auditorium into row_auditorium;
        exit when c_auditorium % notfound;

        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
    end loop;
    close c_auditorium;

    SYS.DBMS_OUTPUT.PUT_LINE('-----------------');
    SYS.DBMS_OUTPUT.PUT_LINE('capacity 61 - 80');

    for row_auditorium in c_auditorium(61,80)
    loop
        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
    end loop;

    SYS.DBMS_OUTPUT.PUT_LINE('-----------------');
    SYS.DBMS_OUTPUT.PUT_LINE('capacity 81 - ... ');

    for row_auditorium in c_auditorium(81,9999)
    loop
        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
    end loop;
EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 15. refcursor
DECLARE
     type auditorium_cursor_type is ref cursor return AUDITORIUM % rowtype;
     c_auditorium  auditorium_cursor_type;
     row_auditorium c_auditorium % rowtype;
BEGIN
    open c_auditorium for select * from AUDITORIUM;
    fetch c_auditorium into row_auditorium;

    while (c_auditorium % found)
    loop
        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
        fetch c_auditorium into row_auditorium;
    end loop;

    close c_auditorium;
EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 16. cursor subquery
DECLARE
    cursor curs_aut is 
        select auditorium_type, cursor
        (
            select AUDITORIUM
            from auditorium A
            where B.AUDITORIUM_TYPE = A.AUDITORIUM_TYPE
        )
        from AUDITORIUM_TYPE B;
    c_auditorium sys_refcursor;
    aum auditorium.auditorium%type;
    aut auditorium_type.auditorium_type%type;
    txt varchar2(1000);
begin
    open curs_aut;
    fetch curs_aut into aut, c_auditorium;
    
    while(curs_aut % found)
    loop
        txt:=rtrim(aut)||':';

        loop
            fetch c_auditorium into aum;
            exit when c_auditorium % notfound;
            
            txt := txt || rtrim(aum) || ', ';
        end loop;

        SYS.DBMS_OUTPUT.PUT_LINE(txt);
        fetch curs_aut into aut, c_auditorium;
    end loop;
    
    close curs_aut;
exception
    when others then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
end;


-- task 17. update current of; decrease auditorium_cap in 0.9
select * from AUDITORIUM where AUDITORIUM_CAPACITY >= 40 and AUDITORIUM_CAPACITY <= 80;
DECLARE
    cursor c_auditorium (min AUDITORIUM.AUDITORIUM % type, max AUDITORIUM.AUDITORIUM % type) is
        select * from AUDITORIUM
        where AUDITORIUM_CAPACITY >= min and AUDITORIUM_CAPACITY <= max
        for update;
BEGIN
    SYS.DBMS_OUTPUT.PUT_LINE('decrease the capacity by 10% from 40 to 80');

    for row_auditorium in c_auditorium(40,80)
    loop
        update AUDITORIUM set AUDITORIUM_CAPACITY = row_auditorium.AUDITORIUM_CAPACITY * 0.9
        where current of c_auditorium;

        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
    end loop;

EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 18. delete auditoriums with cap 0 to 20 via update current of
select * from AUDITORIUM where AUDITORIUM_CAPACITY <= 20;
DECLARE
    cursor c_auditorium (min AUDITORIUM.AUDITORIUM % type, max AUDITORIUM.AUDITORIUM % type) is
        select * from AUDITORIUM
        where AUDITORIUM_CAPACITY >= min and  AUDITORIUM_CAPACITY <= max
        for update;
    row_auditorium c_auditorium % rowtype;
BEGIN
    SYS.DBMS_OUTPUT.PUT_LINE('delete rows from 0 to 20');

    open c_auditorium(0, 20);
    fetch c_auditorium into row_auditorium;

    while (c_auditorium % found)
    loop
        delete from AUDITORIUM where current of c_auditorium;

        SYS.DBMS_OUTPUT.PUT_LINE(row_auditorium.AUDITORIUM || ' ' || row_auditorium.AUDITORIUM_NAME || ' ' || row_auditorium.AUDITORIUM_CAPACITY);
        fetch c_auditorium into row_auditorium;
    end loop;

EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 19. rowid usage
select * from auditorium;

DECLARE
    row_id_auditorium rowid;
BEGIN
    select ROWID into row_id_auditorium from AUDITORIUM where AUDITORIUM_NAME = '206-1';
    SYS.DBMS_OUTPUT.PUT_LINE('found rowid - ' || row_id_auditorium);

    update AUDITORIUM set AUDITORIUM_CAPACITY = 110 where ROWID = row_id_auditorium;

    delete AUDITORIUM where ROWID = row_id_auditorium;

    rollback;
EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;

-- task 20. Printing teachers by groups
DECLARE
    cursor cursor_teacher is select * from TEACHER;
    row_teacher teacher % rowtype;
BEGIN
    for row_teacher in cursor_teacher
    loop
        SYS.DBMS_OUTPUT.PUT_LINE(row_teacher.TEACHER || ' ' || row_teacher.TEACHER_NAME || ' ' || row_teacher.PULPIT);
        if MOD((cursor_teacher % rowcount), 3) = 0
            then
                SYS.DBMS_OUTPUT.PUT_LINE('-------------');
        end if;
    end loop;
EXCEPTION
    when others
        then
            SYS.DBMS_OUTPUT.PUT_LINE(sqlerrm);
END;




