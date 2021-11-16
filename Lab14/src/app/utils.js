const getRequestUrl = (request) => {
    const baseURL = "http" + "://" + request.headers.host + "/";
    return new URL(request.url, baseURL);
}

const getEndpoint = (url) => {
    return url.pathname.match(/[A-Z0-9-./]+/gi)[0];
}

const parseUrlParams = (url) => {
    const urlSearchParams = new URLSearchParams(url.search);
    return Object.fromEntries(urlSearchParams.entries());
}

const parseBody = (request) => new Promise((resolve => {
    let body = "";

    request.on("data", (data) => body += data);
    request.on("end", () => {
        if (body) {
            request.body = JSON.parse(body);
        }
        resolve();
    });
}))

const handleError = (error, request, response) => {
    response.status = error.status;
    response.writeHead(400, {
        "Content-Type" : "text/plain; charset=utf-8"
    })
    response.end(JSON.stringify({
        message: error.message
    }));
}

module.exports = {
    getRequestUrl,
    getEndpoint,
    handleError,
    parseBody,
    parseUrlParams
}