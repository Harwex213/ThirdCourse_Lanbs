const fs = require("fs");
const { buildSchema } = require("graphql");

module.exports = buildSchema(fs.readFileSync(__dirname + "/schema.graphql").toString());