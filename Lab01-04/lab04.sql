-- existed pdbs
select name, open_mode from v$pdbs;

-- existed instances
select instance_name, host_name, status, instance_role, version from v$instance;

-- oracle 12c components
select * from v$sga_dynamic_components;
select comp_name,version, status from dba_registry;

-- My PDB exploration
SELECT  *  FROM  DICTIONARY;
ALTER DATABASE OPEN;

select * from dba_tablespaces;

select file_name, tablespace_name, status from  dba_data_files
union
select file_name, tablespace_name, status from dba_temp_files;

select r.role, r.password_required, r.common, p.privilege
from dba_roles r inner join dba_sys_privs p on r.role = p.grantee
order by r.role;

select * from dba_profiles order by profile;

select u.username, u.user_id, u.account_status, u.expiry_date, u.default_tablespace, u.profile, r.granted_role
from dba_users u inner join dba_role_privs r on u.username = r.grantee
order by u.expiry_date desc;

-- C##KOA user
select username, type, status, osuser, machine, program, service_name from V$SESSION where username is not null;



