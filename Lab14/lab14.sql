CREATE DATABASE LINK igorDb
   CONNECT TO skvortsoff
   IDENTIFIED BY "9I50ybkubu" 
   USING 'IGOR';
   
drop database link igorDb;

ALTER SESSION CLOSE DATABASE LINK igorDb;

SELECT * FROM student@igorDb;

insert into student@igorDb values (170, 'Privet', 'Olezhiku');

update student@igorDb set name = 'Ne privet' where id = 170;

-- global
CREATE PUBLIC DATABASE LINK igorDbPublic
   CONNECT TO skvortsoff
   IDENTIFIED BY "9I50ybkubu" 
   USING 'IGOR';
   
drop public database link igorDbPublic                                                                                                                                                                                                                                      ;

SELECT * FROM student@igorDbPublic;

insert into student@igorDbPublic values (1549, 'Olezhik', 'Privet');

update student@igorDbPublic set name = 'Ne privet' where id = 1549;

delete from student@igorDbPublic where id = 1549;

