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

useRoute("/", indexController);
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