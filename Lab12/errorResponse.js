let errorId = 0;
const errorResponse = (response, error) => {
    response.writeHead(400, {
        "Content-Type" : "application/json"
    })
    response.end(JSON.stringify({
        error: errorId++,
        message: error.message
    }));
}

module.exports = errorResponse;