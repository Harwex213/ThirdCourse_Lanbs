const express = require("express");
const bodyParser = require("body-parser");
const morgan = require("morgan");
const isMsSql = require("config").get("database") === "mssql";
const isUseGraphQl = require("config").get("useGraphQl");
const auditoriumsRoutes = require("./controllers/auditoriumsController");
const facultiesRoutes = require("./controllers/facultiesController");
const auditoriumTypesRoutes = require("./controllers/auditoriumTypesController");
const pulpitsRoutes = require("./controllers/pulpitsController");
const subjectsRoutes = require("./controllers/subjectsController");
const teachersRoutes = require("./controllers/teachersController");

const app = express();

app.use(morgan("dev"));
app.use(bodyParser.urlencoded({
    extended: false
}));
app.use(bodyParser.json());

app.use("/", express.static(__dirname + "/views"));

app.use("/faculties", facultiesRoutes);
app.use("/pulpits", pulpitsRoutes);
if (isMsSql) {
    app.use("/auditoriums", auditoriumsRoutes);
    app.use("/auditorium-types", auditoriumTypesRoutes);
    app.use("/subjects", subjectsRoutes);
    app.use("/teachers", teachersRoutes);
}
if (isUseGraphQl) {
    const { graphqlHTTP } = require("express-graphql");
    const schema = require("./graphql/buildSchema");
    const resolvers = require("./graphql/resolvers");

    app.use("/graphql", graphqlHTTP({
        schema: schema,
        rootValue: resolvers,
        graphiql: true
    }))
}

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