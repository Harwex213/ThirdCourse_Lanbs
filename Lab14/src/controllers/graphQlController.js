const resolvers = require("../graphql/resolvers");
const schema = require("../graphql/buildSchema");
const { graphql } = require("graphql");
const router = require("../router/router")();

const handleGraphQlRequest = async (request, response) => {
    const graphqlRequest = request.body;
    if (!(graphqlRequest && graphqlRequest.query)) {
        const error = new Error("Bad request. Request should have json body with queries objects");
        error.status = 400;
        error.stack = null;
        throw error;
    }

    const result = await graphql(schema, graphqlRequest.query, resolvers, null, graphqlRequest.variables);

    response.writeHead(200, {
        "Content-Type" : "application/json"
    })
    response.end(JSON.stringify(result));
}

router.post("/", handleGraphQlRequest);

module.exports = router;