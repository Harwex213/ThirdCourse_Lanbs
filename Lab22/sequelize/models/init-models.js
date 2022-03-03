const DataTypes = require("sequelize").DataTypes;
const _commits = require("./commits");
const _repos = require("./repos");
const _users = require("./users");

function initModels(sequelize) {
  const commits = _commits(sequelize, DataTypes);
  const repos = _repos(sequelize, DataTypes);
  const users = _users(sequelize, DataTypes);

  commits.belongsTo(repos, { as: "repo", foreignKey: "repoId"});
  repos.hasMany(commits, { as: "commits", foreignKey: "repoId"});
  repos.belongsTo(users, { as: "author", foreignKey: "authorId"});
  users.hasMany(repos, { as: "repos", foreignKey: "authorId"});

  return {
    commits,
    repos,
    users,
  };
}
module.exports = initModels;
module.exports.initModels = initModels;
module.exports.default = initModels;
