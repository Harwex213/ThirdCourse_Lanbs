const getHeaders = (request, response) => {
    response.setHeader("Content-Type", "text/html; charset=utf-8");
    response.setHeader("MyHeader", "1");
    response.writeHead(200);

    response.write(`<div>Request  headers: ${JSON.stringify(request.headers)}</div>`);
    response.write(`<br>`);
    response.write(`<div>Response headers: ${JSON.stringify(response.getHeaders())}<div>`);

    response.end();
}

module.exports = {
    getHeaders
}