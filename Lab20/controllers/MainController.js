const express = require("express");
const dataAccess = require("../data-access/repository");

const getDirectory = (request, response) => {
    response.render("get", {
        records: dataAccess.getRecords(),
        isRecordsDisabled: false,
    })
}

const getDirectoryWithAdd = (request, response) => {
    response.render("add", {
        records: dataAccess.getRecords(),
        isRecordsDisabled: true,
    })
}

const getDirectoryWithUpdate = (request, response) => {
    const records = dataAccess.getRecords();
    const record = records.find(record => record.id === request.query.id);

    response.render("update", {
        records: records,
        isRecordsDisabled: true,
        record: record
    })
}

const postAddRecord = (request, response) => {
    dataAccess.addRecord(request.body);
    response.redirect("/");
}

const postUpdateRecord = (request, response) => {
    dataAccess.updateRecord(request.body);
    response.redirect("/");
}

const postDeleteRecord = (request, response) => {
    dataAccess.deleteRecord({
        id: request.query.id,
    })
    response.redirect("/");
}

const router = express.Router();

router.get("/", getDirectory);
router.get("/Add", getDirectoryWithAdd);
router.get("/Update", getDirectoryWithUpdate);
router.post("/Add", postAddRecord);
router.post("/Update", postUpdateRecord);
router.post("/Delete", postDeleteRecord);

module.exports = router;