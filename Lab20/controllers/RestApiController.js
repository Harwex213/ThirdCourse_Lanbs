const express = require("express");
const dataAccess = require("../data-access/repository");

const getRecords = (request, response) => {
    response.json(dataAccess.getRecords());
}

const postRecord = (request, response) => {
    dataAccess.addRecord(request.body);
    response.sendStatus(201);
}

const putRecord = (request, response) => {
    if (!request.body.id) {
        response.sendStatus(400);
    }

    const result = dataAccess.updateRecord(request.body);
    if (result) {
        response.sendStatus(200);
    }
    else {
        response.sendStatus(404);
    }
}

const deleteRecord = (request, response) => {
    if (!request.query.id) {
        response.sendStatus(404);
    }

    const result = dataAccess.deleteRecord({
        id: request.query.id,
    })
    if (result) {
        response.sendStatus(200);
    }
    else {
        response.sendStatus(404);
    }
}

const router = express.Router();

router.get("/", getRecords);
router.post("/", postRecord);
router.put("/", putRecord);
router.delete("/", deleteRecord);

module.exports = router;