const { Sequelize } = require("sequelize");
const initModels = require("./data-access/models/initModels");

const sequelize = new Sequelize("University", "sa", "Passw0rd1", {
    host: "localhost",
    dialect: "mssql"
});

(async () => {
    let transaction = null;
    try {
        await sequelize.authenticate();
        console.log('Connection has been established successfully.');
        const model = initModels(sequelize);

        transaction = await sequelize.transaction();
        await sequelize.model("AUDITORIUM").update({
            AUDITORIUM_CAPACITY: 1
        }, {
            where: {
            },
            transaction: transaction
        });
        console.log("Successfully updated all auditoriums_capacity to 1");

        setTimeout(async () => {
            await transaction.rollback();
            console.log("Successfully rollback transaction");
        }, 10000)

    } catch (error) {
        await transaction?.rollback();
        console.error('Unable to connect to the database:', error);
    }
})();