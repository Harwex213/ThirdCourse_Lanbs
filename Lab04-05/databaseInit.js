const db = new (require("./database/database"))();
const dbCrud = require("./api/db/crudHandling");
const statistic = require("./statistic/statistic");

db.on("GET", dbCrud.getRequest);
db.on("POST", dbCrud.postRequest);
db.on("PUT", dbCrud.putRequest);
db.on("DELETE", dbCrud.deleteRequest);
db.on("COMMIT", async () => {
    db.commit();
    if (statistic.isGathering()) {
        statistic.onCommit();
    }
});

module.exports = db;