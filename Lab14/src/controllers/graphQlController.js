const resolvers = require("../graphql/resolvers");
const schema = require("../graphql/buildSchema");
const { graphql } = require("graphql");

const handleGraphQlRequest = async (request, response) => {
    const graphqlRequest = request.body;
    if (!(graphqlRequest && graphqlRequest.query)) {
        response.writeHead(400, {
            "Content-Type" : "application/json"
        })
        response.end(JSON.stringify({
            message: "Bad request. Request should have json body with queries objects"
        }));
    }

    const result = await graphql(schema, graphqlRequest.query, resolvers);

    response.writeHead(200, {
        "Content-Type" : "application/json"
    })
    response.end(JSON.stringify(result));
}

module.exports = {
    handleGraphQlRequest
}