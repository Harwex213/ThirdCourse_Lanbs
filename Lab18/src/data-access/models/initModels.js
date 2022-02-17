const DataTypes = require("sequelize").DataTypes;
const _AUDITORIUM = require("./auditorium");
const _AUDITORIUM_TYPE = require("./auditoriumTypes");
const _FACULTY = require("./faculty");
const _PULPIT = require("./pulpit");
const _SUBJECT = require("./subject");
const _TEACHER = require("./teacher");

function initModels(sequelize) {
  const AUDITORIUM = _AUDITORIUM(sequelize, DataTypes);
  const AUDITORIUM_TYPE = _AUDITORIUM_TYPE(sequelize, DataTypes);
  const FACULTY = _FACULTY(sequelize, DataTypes);
  const PULPIT = _PULPIT(sequelize, DataTypes);
  const SUBJECT = _SUBJECT(sequelize, DataTypes);
  const TEACHER = _TEACHER(sequelize, DataTypes);

  AUDITORIUM.belongsTo(AUDITORIUM_TYPE, { as: "AUDITORIUM_TYPE_AUDITORIUM_TYPE", foreignKey: "AUDITORIUM_TYPE"});
  AUDITORIUM_TYPE.hasMany(AUDITORIUM, { as: "AUDITORIa", foreignKey: "AUDITORIUM_TYPE"});
  PULPIT.belongsTo(FACULTY, { as: "FACULTY_FACULTY", foreignKey: "FACULTY"});
  FACULTY.hasMany(PULPIT, { as: "PULPITs", foreignKey: "FACULTY"});
  SUBJECT.belongsTo(PULPIT, { as: "PULPIT_PULPIT", foreignKey: "PULPIT"});
  PULPIT.hasMany(SUBJECT, { as: "SUBJECTs", foreignKey: "PULPIT"});
  TEACHER.belongsTo(PULPIT, { as: "PULPIT_PULPIT", foreignKey: "PULPIT"});
  PULPIT.hasMany(TEACHER, { as: "TEACHERs", foreignKey: "PULPIT"});

  return {
    AUDITORIUM,
    AUDITORIUM_TYPE,
    FACULTY,
    PULPIT,
    SUBJECT,
    TEACHER,
  };
}
module.exports = initModels;
module.exports.initModels = initModels;
module.exports.default = initModels;
