-- task 1-2, hello world
BEGIN
DBMS_OUTPUT.PUT_LINE('Hello World');
END;

-- task 3-4. Inner block + sql exception
DECLARE 
  x NUMBER(1) := 6;
  y NUMBER(1) := 0;
  z NUMBER(1);
BEGIN
  BEGIN
    z := x / y;
  EXCEPTION
    WHEN OTHERS
    THEN DBMS_OUTPUT.PUT_LINE('ERROR: ' || sqlerrm);
  END;
END;

-- task 5. Compilator's warning types at the moment
SHOW PARAMETERS plsql_warnings;

-- task 6-8. All symbols, keywords and Oracle Server params linked with PL/SQL
SELECT * FROM v$reserved_words
WHERE length = 1;

SELECT * FROM v$reserved_words
WHERE length > 1;

SHOW PARAMETER;
SELECT * FROM v$parameter;

-- task 9
DECLARE
n1 NUMBER(1) := 1;
n2 NUMBER := 678;
n3 BINARY_INTEGER := 123.645;
n4 PLS_INTEGER := 123.656; 
n5 NATURAL := 45.67;       
n6 NATURALN := 45.67;
n7 POSITIVE := 45.67;
n8 POSITIVEN := 45.67;
n9 SIGNTYPE := 0;         
fix NUMBER := 1234.5678;
fix1 NUMBER(3,2) := 3.14;
fix2 NUMBER(5,3) := 12.345;
fix3 NUMBER(4,-2) := 160.65768;
float1 BINARY_FLOAT := 12345.6789;
double1 BINARY_DOUBLE := 123456.789;
e1 NUMBER := 127E-2;
b1 BOOLEAN := true;
BEGIN
    DBMS_OUTPUT.PUT_LINE('task 10');
    DBMS_OUTPUT.PUT_LINE('n1 = ' || n1);
    DBMS_OUTPUT.PUT_LINE('n2 = ' || n2);
    DBMS_OUTPUT.PUT_LINE('n3 = ' || n3);
    DBMS_OUTPUT.PUT_LINE('n4 = ' || n4);
    DBMS_OUTPUT.PUT_LINE('n5 = ' || n5);
    DBMS_OUTPUT.PUT_LINE('n6 = ' || n6);
    DBMS_OUTPUT.PUT_LINE('n7 = ' || n7);
    DBMS_OUTPUT.PUT_LINE('n8 = ' || n8);
    DBMS_OUTPUT.PUT_LINE('n9 = ' || n9);
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 11');
    DBMS_OUTPUT.PUT_LINE('n2 / n5 = ' || (n2 / n5));
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 12-13');
    DBMS_OUTPUT.PUT_LINE('fix = ' || fix);
    DBMS_OUTPUT.PUT_LINE('fix1 = ' || fix1);
    DBMS_OUTPUT.PUT_LINE('fix2 = ' || fix2);
    DBMS_OUTPUT.PUT_LINE('fix3 = ' || fix3);
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 14-15');
    DBMS_OUTPUT.PUT_LINE('float1 = ' || float1);
    DBMS_OUTPUT.PUT_LINE('double1 = ' || double1);
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 15');
    DBMS_OUTPUT.PUT_LINE('e1 = ' || e1);
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 16');
    DBMS_OUTPUT.PUT_LINE(   
        case
            when b1 then 'TRUE'
            when b1 is null then 'NULL'
            else 'FALSE'
        end
    );
END;

-- task 18. constants
DECLARE
    constNumber CONSTANT NUMBER := 21;
    constVarchar CONSTANT VARCHAR2(10) := 'Varchar2';
    constChar CONSTANT CHAR(5) := 'Char';
BEGIN
    DBMS_OUTPUT.PUT_LINE(constNumber); 
    DBMS_OUTPUT.PUT_LINE(constVarchar); 
    DBMS_OUTPUT.PUT_LINE(constChar);
END;

-- task 19-20. %type + %rowtype
DECLARE
    pulpit1 pulpit.pulpit%TYPE;
    faculty1 faculty%ROWTYPE;
BEGIN 
    pulpit1 := 'ПОИТ';
    DBMS_OUTPUT.PUT_LINE(pulpit1);
    faculty1.faculty := 'ФИТ';
    faculty1.faculty_name := 'Факультет информационных технологий';
    DBMS_OUTPUT.PUT_LINE(faculty1.faculty);
END;

-- task 21-26
DECLARE
    x NUMBER(1) := 5;
BEGIN
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 21 - if');
    IF x > 5 THEN
        DBMS_OUTPUT.PUT_LINE('x > 5 ');
    ELSIF x < 5 THEN
        DBMS_OUTPUT.PUT_LINE('x < 5');
    ELSE
        DBMS_OUTPUT.PUT_LINE('x = 5');
    END IF;
    
    x:= -2;
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 23 - case');
    CASE
        WHEN x BETWEEN 0 AND 5 THEN
            DBMS_OUTPUT.PUT_LINE('10 <= ' || x || ' <= 20');
        WHEN x BETWEEN 6 AND 10 THEN
            DBMS_OUTPUT.PUT_LINE('BETWEEN 21 AND 40');
        ELSE
            DBMS_OUTPUT.PUT_LINE('ELSE');
    END CASE;
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 24 - loop');
    DBMS_OUTPUT.PUT_LINE('x = ' || x);
    loop
        x:= x + 1;
        DBMS_OUTPUT.PUT_LINE('x = ' || x);
    exit when x > 2;    
    end loop;
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 25 - for');
    for k in -3..-1
    loop 
        x:= k;
        DBMS_OUTPUT.PUT_LINE('x = ' || x);
    end loop;    
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('task 26 - while');
    while (x < 3)
    loop
        x:= x + 1;
        DBMS_OUTPUT.PUT_LINE('x = ' || x);
    end loop;
END;




    










