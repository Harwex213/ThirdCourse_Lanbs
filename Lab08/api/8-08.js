const getResponse = (request, response, args) => {
    const { requestUrl } = args;
    const code = requestUrl.searchParams.get("code");
    const message = requestUrl.searchParams.get("mess");
    if (Number(code) && message) {
        response.writeHead(Number(code), message);
    }
    response.end();
}

module.exports = {
    getResponse
}