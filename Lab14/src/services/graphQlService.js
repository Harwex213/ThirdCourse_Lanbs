const dataAccess = require("../data-access/mssql/data-access");

const getAllModels = async ({ table, filter, filterKeyName }) => {
    let result = await dataAccess(table).getEntities();

    return filter ? result.filter(entity => entity[filterKeyName].trim() === filter.trim()) : result;
}

const setModel = async ({ table, id, values }) => {
    if ((await dataAccess(table).tryFindEntity(id)).isExist) {
        await dataAccess(table).deleteEntity(id);
    }
    return await dataAccess(table).createEntity(id, values);
}

const deleteModel = async ({ table, id }) => {
    if ((await dataAccess(table).tryFindEntity(id)).isExist) {
        await dataAccess(table).deleteEntity(id);
        return true;
    }
    return false;
}

const getEntitiesByFaculty = async ({ table, faculty }) => {
    if (faculty) {
        return await dataAccess().query(`Select * from faculty f 
                                               join pulpit p 
                                                   on f.FACULTY = p.FACULTY
                                               join ${table} t 
                                                   on t.PULPIT = p.PULPIT
                                               where f.FACULTY = '${faculty}'`);
    }
    return await dataAccess(table).getEntities();
}

module.exports = {
    getAllModels,
    setModel,
    deleteModel,
    getEntitiesByFaculty
}