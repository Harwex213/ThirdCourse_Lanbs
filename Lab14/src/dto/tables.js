const Faculty = require("../data-access/mongodb/models/faculty");
const Pulpit = require("../data-access/mongodb/models/pulpit");
const isMsSql = require("config").get("database") === "mssql";

module.exports = {
    auditoriums: "AUDITORIUM",
    auditoriumTypes: "AUDITORIUM_TYPE",
    faculties: isMsSql ? "FACULTY" : Faculty,
    pulpits: isMsSql ? "PULPIT" : Pulpit,
    teachers: "TEACHER",
    subjects: "SUBJECT",
}