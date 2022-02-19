const { Sequelize } = require("sequelize");
const initModels = require("../models/init-models");

const sequelize = new Sequelize("DbViaWeb", "sa", "Passw0rd1", {
    host: "localhost",
    dialect: "mssql"
});

(async () => {
    try {
        await sequelize.authenticate();
        console.log("Connection has been established successfully.");

        initModels(sequelize);
        console.log("Defining models of database.");
    } catch (error) {
        console.error("Unable to connect to the database:", error);
    }
})();

module.exports = {
    sequelize,
};