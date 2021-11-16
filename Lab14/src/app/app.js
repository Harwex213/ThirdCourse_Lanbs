const {
    getRequestUrl,
    getEndpoint,
    handleError,
    parseBody,
} = require("./utils");
const { useRoute, resolveRoute } = require("../router/routeUtils");
const config = require("config");
const indexController = require("../controllers/indexController");
const graphQlController = require("../controllers/graphQlController");
const auditoriumsRoutes = require("../controllers/auditoriumsController");
const facultiesRoutes = require("../controllers/facultiesController");
const auditoriumTypesRoutes = require("../controllers/auditoriumTypesController");
const pulpitsRoutes = require("../controllers/pulpitsController");
const subjectsRoutes = require("../controllers/subjectsController");
const teachersRoutes = require("../controllers/teachersController");

useRoute("/", indexController);
useRoute("/faculties", facultiesRoutes);
useRoute("/pulpits", pulpitsRoutes);
if (config.get("database") === "mssql") {
    useRoute("/auditoriums", auditoriumsRoutes);
    useRoute("/auditorium-types", auditoriumTypesRoutes);
    useRoute("/subjects", subjectsRoutes);
    useRoute("/teachers", teachersRoutes);
}
if (config.get("useGraphQl")) {
    useRoute("/graphql", graphQlController);
}

const app = async (request, response) => {
    try {
        const url = getRequestUrl(request);
        const endpoint = getEndpoint(url);

        const handler = resolveRoute(request, endpoint);
        await parseBody(request);
        await handler(request, response);
    }
    catch (error) {
        handleError(error, request, response);
    }
}

module.exports = app;