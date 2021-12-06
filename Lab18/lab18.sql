CREATE TABLE test_table ( 
    MEGAID   NUMBER PRIMARY KEY , 
    NAME1       VARCHAR2(30),  
    DATE1     DATE, 
    NUMBER1       NUMBER
);
drop table test_table
select * from test_table

-- EXPORT
create or replace directory UTLDATA AS 'C:\labka';

CREATE OR REPLACE PROCEDURE teacher_export
IS
rc sys_refcursor;
doc DBMS_XMLDOM.DOMDocument;
BEGIN
OPEN rc FOR SELECT * FROM U_KOA_CORE.teacher;
doc := DBMS_XMLDOM.NewDOMDocument(XMLTYPE(rc));
DBMS_XMLDOM.WRITETOFILE(doc, 'UTLDATA\teacher_export.xml');
END teacher_export;

begin
    teacher_export();
end;
