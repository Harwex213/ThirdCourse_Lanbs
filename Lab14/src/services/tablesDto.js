const Faculty = require("../data-access/mongodb/models/faculty");
const Pulpit = require("../data-access/mongodb/models/pulpit");
const isMsSql = require("config").get("database") === "mssql";

module.exports = {
    auditoriumsService: "AUDITORIUM",
    auditoriumTypesService: "AUDITORIUM_TYPE",
    facultiesService: isMsSql ? "FACULTY" : Faculty,
    pulpitsService: isMsSql ? "PULPIT" : Pulpit,
    teacherService: "TEACHER",
    subjectsService: "SUBJECT",
}