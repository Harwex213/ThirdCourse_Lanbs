const express = require("express");
const bodyParser = require("body-parser");
const morgan = require("morgan");
const auditoriumsRoutes = require("./controllers/auditoriumsController");
const facultiesRoutes = require("./controllers/facultiesController");
const auditoriumTypesRoutes = require("./controllers/auditoriumTypesController");
const pulpitsRoutes = require("./controllers/pulpitsController");
const subjectsRoutes = require("./controllers/subjectsController");

const app = express();

app.use(morgan("dev"));
app.use(bodyParser.urlencoded({
    extended: false
}));
app.use(bodyParser.json());

app.use("/", express.static("./views"));

app.use("/auditoriums", auditoriumsRoutes);
app.use("/auditorium-types", auditoriumTypesRoutes);
app.use("/faculties", facultiesRoutes);
app.use("/pulpits", pulpitsRoutes);
app.use("/subjects", subjectsRoutes);

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
            message: error.message
        }
    });
});

module.exports = app;