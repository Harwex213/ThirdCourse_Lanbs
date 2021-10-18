const graphqlService = require("../services/graphQlService");
const tables = require("../dto/tables");
const tablesIdKeys = require("../dto/tablesIdKeys");

const getFaculties = async (args) => {
    return await graphqlService.getAllModels({
        table: tables.faculties,
        filter: args.faculty,
        filterKeyName: tablesIdKeys.faculties
    })
}

const getPulpits = async (args) => {
    return await graphqlService.getAllModels({
        table: tables.pulpits,
        filter: args.pulpit,
        filterKeyName: tablesIdKeys.pulpits
    })
}

const getTeachers = async (args) => {
    return await graphqlService.getAllModels({
        table: tables.teachers,
        filter: args.teacher,
        filterKeyName: tablesIdKeys.teachers
    })
}

const getSubjects = async (args) => {
    return await graphqlService.getAllModels({
        table: tables.subjects,
        filter: args.subject,
        filterKeyName: tablesIdKeys.subjects
    })
}

const setFaculty = async (args) => {
    return await graphqlService.setModel({
        table: tables.faculties,
        id: {
            name: tablesIdKeys.faculties,
            value: args.faculty.FACULTY
        },
        values: {
            ...args.faculty
        }
    })
}

const setPulpit = async (args) => {
    return await graphqlService.setModel({
        table: tables.pulpits,
        id: {
            name: tablesIdKeys.pulpits,
            value: args.pulpit.PULPIT
        },
        values: {
            ...args.pulpit
        }
    })
}

const setTeacher = async (args) => {
    return await graphqlService.setModel({
        table: tables.teachers,
        id: {
            name: tablesIdKeys.teachers,
            value: args.teacher.TEACHER
        },
        values: {
            ...args.teacher
        }
    })
}

const setSubject = async (args) => {
    return await graphqlService.setModel({
        table: tables.subjects,
        id: {
            name: tablesIdKeys.subjects,
            value: args.subject.SUBJECT
        },
        values: {
            ...args.subject
        }
    })
}

const delFaculty = async (args) => {
    return await graphqlService.deleteModel({
        table: tables.faculties,
        id: {
            name: tablesIdKeys.faculties,
            value: args.faculty
        }
    })
}

const delPulpit = async (args) => {
    return await graphqlService.deleteModel({
        table: tables.pulpits,
        id: {
            name: tablesIdKeys.pulpits,
            value: args.pulpit
        }
    })
}

const delTeacher = async (args) => {
    return await graphqlService.deleteModel({
        table: tables.teachers,
        id: {
            name: tablesIdKeys.teachers,
            value: args.teacher
        }
    })
}

const delSubject = async (args) => {
    return await graphqlService.deleteModel({
        table: tables.subjects,
        id: {
            name: tablesIdKeys.subjects,
            value: args.subject
        }
    })
}

const getTeachersByFaculty = async (args) => {
    return await graphqlService.getEntitiesByFaculty({
        table: tables.teachers,
        faculty: args.faculty
    })
}

const getSubjectsByFaculty = async (args) => {
    return await graphqlService.setModel({
        table: tables.subjects,
        faculty: args.faculty
    })
}

module.exports = {
    getFaculties,
    getPulpits,
    getTeachers,
    getSubjects,

    setFaculty,
    setPulpit,
    setTeacher,
    setSubject,

    delFaculty,
    delPulpit,
    delTeacher,
    delSubject,

    getTeachersByFaculty,
    getSubjectsByFaculty,
}