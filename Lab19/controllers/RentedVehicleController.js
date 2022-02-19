const express = require("express");
const dataAccess = require("../data-access/Repository");
const dateFormat = require("dateformat");
const model = "RentedVehicle";

const getRentedVehicles = async (request, response, next) => {
    try {
        const rentedVehicles = await dataAccess.getModels(model);

        response.render("get", {
            title: "RentedVehicles",
            data: rentedVehicles.map(rentedVehicle => `${rentedVehicle.Id}: ` +
                `clientId: ${rentedVehicle.ClientId}, vehicleId: ${rentedVehicle.VehicleId}. ` +
                `DateFrom: ${dateFormat(rentedVehicle.DateFrom, "shortDate")}.` +
                `DateTo: ${dateFormat(rentedVehicle.DateTo, "shortDate")}`)
        });
    }
    catch (e) {
        next(e);
    }
}

const postRentedVehicle = async (request, response, next) => {
    try {
        const body = request.body;
        const rentedVehicleToCreate = {
            ClientId: body.clientId,
            VehicleId: body.vehicleId,
            DateFrom: dateFormat(body.dateFrom, "yyyy-mm-dd HH:MM:ss.l"),
            DateTo: dateFormat(body.dateTo, "yyyy-mm-dd HH:MM:ss.l"),
        };
        if (new Date(rentedVehicleToCreate.DateFrom) > new Date(rentedVehicleToCreate.DateTo)) {
            const error = new Error("Date from cannot be more than date to");
            error.status = 400;
            next(error);
        }
        const rentedVehicle = await dataAccess.postModel(model, rentedVehicleToCreate);

        response.render("post", {
            title: "Create rentedVehicle",
            status: `Successfully created rentedVehicle with ${rentedVehicle.Id}: ` +
                `clientId: ${rentedVehicle.ClientId}, vehicleId: ${rentedVehicle.VehicleId}. ` +
                `DateFrom: ${dateFormat(rentedVehicle.DateFrom, "shortDate")}.` +
                `DateTo: ${dateFormat(rentedVehicle.DateTo, "shortDate")}`
        });
    }
    catch (e) {
        next(e);
    }
}

const putRentedVehicle = async (request, response, next) => {
    try {
        const body = request.body;
        const id = {
            Id: Number(request.body.id)
        };
        const rentedVehicle = {
            ClientId: body.clientId,
            VehicleId: body.vehicleId,
            DateFrom: dateFormat(body.dateFrom, "yyyy-mm-dd HH:MM:ss.l"),
            DateTo: dateFormat(body.dateTo, "yyyy-mm-dd HH:MM:ss.l"),
        };
        if (new Date(rentedVehicle.DateFrom) > new Date(rentedVehicle.DateTo)) {
            const error = new Error("Date from cannot be more than date to");
            error.status = 400;
            next(error);
        }

        const isUpdated = await dataAccess.putModel(model, id, rentedVehicle);

        let status = "Successfully updated rentedVehicle with id " + id.Id +
            `: clientId: ${rentedVehicle.ClientId}, vehicleId: ${rentedVehicle.VehicleId}. ` +
            `DateFrom: ${dateFormat(rentedVehicle.DateFrom, "shortDate")}.` +
            `DateTo: ${dateFormat(rentedVehicle.DateTo, "shortDate")}`
        if (isUpdated[0] === 0) {
            status = "Not one rentedVehicle was updated. Try to delete rentedVehicle with id " + id.Id;
        }

        response.render("put", {
            title: "Update rentedVehicle",
            status
        });
    }
    catch (e) {
        next(e);
    }
}

const deleteRentedVehicle = async (request, response, next) => {
    try {
        const id = {
            Id: Number(request.params.rentedVehicleId)
        };
        const isDeleted = await dataAccess.deleteModel(model, id);

        let status = "Successfully deleted rentedVehicle with id " + id.Id;
        if (isDeleted === 0) {
            status = "Not one rentedVehicle was deleted. Try to delete rentedVehicle with id " + id.Id;
        }

        response.render("delete", {
            title: "Delete rentedVehicle",
            status
        });
    }
    catch (e) {
        next(e);
    }
}

const router = express.Router();

router.get("/", getRentedVehicles);
router.post("/", postRentedVehicle);
router.put("/", putRentedVehicle)
router.delete("/:rentedVehicleId", deleteRentedVehicle);

module.exports = router;