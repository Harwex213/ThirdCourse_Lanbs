const express = require("express");
const dataAccess = require("../data-access/Repository");
const model = "Client";

const getClients = async (request, response, next) => {
    try {
        const clients = await dataAccess.getModels(model);

        response.render("get", {
            title: "Clients",
            data: clients.map(client => `${client.Id}: ${client.FirstName} ${client.LastName}. ` +
                `Passport: ${client.Passport}. Email: ${client.Email}`)
        });
    }
    catch (e) {
        next(e);
    }
}

const postClient = async (request, response, next) => {
    try {
        const body = request.body;
        const client = await dataAccess.postModel(model, {
            FirstName: body.firstName,
            LastName: body.lastName,
            Passport: body.passport,
            Email: body.email ?? null,
        });

        response.render("post", {
            title: "Create client",
            status: `Successfully created client with ${client.Id}: ${client.FirstName} ${client.LastName}. ` +
                `Passport: ${client.Passport}. Email: ${client.Email}`
        });
    }
    catch (e) {
        next(e);
    }
}

const putClient = async (request, response, next) => {
    try {
        const body = request.body;
        const id = {
            Id: Number(request.body.id)
        };
        const client = {
            FirstName: body.firstName,
            LastName: body.lastName,
            Passport: body.passport,
            Email: body.email ?? null,
        };

        const isUpdated = await dataAccess.putModel(model, id, client);

        let status = "Successfully updated client with id " +
            `${id.Id}: ${client.FirstName} ${client.LastName}. ` +
            `Passport: ${client.Passport}. Email: ${client.Email}`
        if (isUpdated[0] === 0) {
            status = "Not one client was updated. Try to delete client with id " + id.Id;
        }

        response.render("put", {
            title: "Update client",
            status
        });
    }
    catch (e) {
        next(e);
    }
}

const deleteClient = async (request, response, next) => {
    try {
        const id = {
            Id: Number(request.params.clientId)
        };
        const isDeleted = await dataAccess.deleteModel(model, id);

        let status = "Successfully deleted client with id " + id.Id;
        if (isDeleted === 0) {
            status = "Not one client was deleted. Try to delete client with id " + id.Id;
        }

        response.render("delete", {
            title: "Delete client",
            status
        });
    }
    catch (e) {
        next(e);
    }
}

const router = express.Router();

router.get("/", getClients);
router.post("/", postClient);
router.put("/", putClient)
router.delete("/:clientId", deleteClient);

module.exports = router;