const express = require("express");
const auditoriumsService = require("../services/auditoriumsService");

const getAuditoriums = async (request, response, next) => {
    try {
        response.status(200).json(await auditoriumsService.getAllAuditoriums());
    } catch (e) {
        next(e);
    }
};

const postAuditoriums = async (request, response, next) => {
    try {
        const body = request.body;
        const id = {
            name: "auditorium",
            value: body.auditorium
        };
        const values = [body.auditorium, body.auditorium_name, body.auditorium_capacity, body.auditorium_type];
        response.status(200).json(await auditoriumsService.createAuditorium(id, values));
    } catch (e) {
        next(e);
    }
};

const putAuditoriums = async (request, response, next) => {
    try {
        const body = request.body;
        const id = {
            name: "auditorium",
            value: body.auditorium
        };
        const values = {
            auditorium_name: body.auditorium_name,
            auditorium_capacity: body.auditorium_capacity,
            auditorium_type: body.auditorium_type
        };
        response.status(200).json(await auditoriumsService.updateAuditorium(id, values));
    } catch (e) {
        next(e);
    }
};

const deleteAuditoriums = async (request, response, next) => {
    try {
        const id = {
            name: "auditorium",
            value: request.params.auditoriumId.toString()
        };
        response.status(200).json(await auditoriumsService.deleteAuditorium(id));
    } catch (e) {
        next(e);
    }
};

const router = express.Router();

router.get("/", getAuditoriums);
router.post("/", postAuditoriums);
router.put("/", putAuditoriums)
router.delete("/:auditoriumId", deleteAuditoriums);

module.exports = router;