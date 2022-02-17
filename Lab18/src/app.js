const express = require("express");
const bodyParser = require("body-parser");
const morgan = require("morgan");
const auditoriumsRoutes = require("./controllers/auditoriumsController");
const facultiesRoutes = require("./controllers/facultiesController");
const auditoriumTypesRoutes = require("./controllers/auditoriumTypesController");
const pulpitsRoutes = require("./controllers/pulpitsController");
const subjectsRoutes = require("./controllers/subjectsController");
const teachersRoutes = require("./controllers/teachersController");
const { Auditorium: model } = require("./constants/database");
const crudService = require("./services/crudService");

const app = express();

app.use(morgan("dev"));
app.use(bodyParser.urlencoded({
    extended: false
}));
app.use(bodyParser.json());

app.use("/", express.static(__dirname + "/views"));

app.use("/api/faculties", facultiesRoutes);
app.use("/api/pulpits", pulpitsRoutes);
app.use("/api/auditoriums", auditoriumsRoutes);
app.use("/api/auditorium-types", auditoriumTypesRoutes);
app.use("/api/subjects", subjectsRoutes);
app.use("/api/teachers", teachersRoutes);
app.get("/api/auditoriumsgt60", async (request, response, next) => {
    try {
        response.status(200).json(await crudService(model).getAllModelsWithScope("sgt60"));
    } catch (e) {
        next(e);
    }
})

app.use((request, response, next) => {
    const error = new Error("Bad request");
    error.status = 400;
    error.stack = null;
    next(error);
});

app.use((error, request, response, next) => {
    response.status(error.status || 500);
    response.json({
        error: {
            message: error.original?.message ?? error.message
        }
    });
});

module.exports = app;