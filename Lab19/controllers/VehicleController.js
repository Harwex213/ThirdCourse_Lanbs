const express = require("express");
const dataAccess = require("../data-access/Repository");
const model = "Vehicle";

const getVehicles = async (request, response, next) => {
    try {
        const vehicles = await dataAccess.getModels(model);

        response.render("get", {
            title: "Vehicles",
            data: vehicles.map(vehicle => `${vehicle.Id}: ${vehicle.Name} for $${vehicle.Price}. ` +
                `Amount: ${vehicle.Amount}. VehicleType: ${vehicle.VehicleTypeId}`)
        });
    }
    catch (e) {
        next(e);
    }
}

const postVehicle = async (request, response, next) => {
    try {
        const body = request.body;
        const vehicle = await dataAccess.postModel(model, {
            Name: body.name,
            Price: body.price,
            Amount: body.amount,
            VehicleTypeId: body.vehicleType,
        });

        response.render("post", {
            title: "Create vehicle",
            status: `Successfully created vehicle with ${vehicle.Id}: ${vehicle.Name} for $${vehicle.Price}. ` +
                `Amount: ${vehicle.Amount}. VehicleType: ${vehicle.VehicleTypeId}`
        });
    }
    catch (e) {
        next(e);
    }
}

const putVehicle = async (request, response, next) => {
    try {
        const body = request.body;
        const id = {
            Id: Number(request.body.id)
        };
        const vehicle = {
            Name: body.name,
            Price: body.price,
            Amount: body.amount,
            VehicleTypeId: body.vehicleType,
        };

        const isUpdated = await dataAccess.putModel(model, id, vehicle);

        let status = "Successfully updated vehicle with id " +
            `${vehicle.Id}: ${vehicle.Name} for $${vehicle.Price}. ` +
            `Amount: ${vehicle.Amount}. VehicleType: ${vehicle.VehicleTypeId}`
        if (isUpdated[0] === 0) {
            status = "Not one vehicle was updated. Try to delete vehicle with id " + id.Id;
        }

        response.render("put", {
            title: "Update vehicle",
            status
        });
    }
    catch (e) {
        next(e);
    }
}

const deleteVehicle = async (request, response, next) => {
    try {
        const id = {
            Id: Number(request.params.vehicleId)
        };
        const isDeleted = await dataAccess.deleteModel(model, id);

        let status = "Successfully deleted vehicle with id " + id.Id;
        if (isDeleted === 0) {
            status = "Not one vehicle was deleted. Try to delete vehicle with id " + id.Id;
        }

        response.render("delete", {
            title: "Delete vehicle",
            status
        });
    }
    catch (e) {
        next(e);
    }
}

const router = express.Router();

router.get("/", getVehicles);
router.post("/", postVehicle);
router.put("/", putVehicle)
router.delete("/:vehicleId", deleteVehicle);

module.exports = router;