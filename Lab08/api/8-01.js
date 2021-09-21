const getConnection = (request, response, args) => {
    const { requestUrl, server } = args;

    response.write(`KeepAliveTimeout: ${server.keepAliveTimeout}\n`);

    const value = requestUrl.searchParams.get("set");
    if (value && Number.isInteger((Number(value)))) {
        server.keepAliveTimeout = Number(value);
        response.write(`KeepAliveTimeout after changing: ${server.keepAliveTimeout}\n`);
    }

    response.end();
}

module.exports = {
    getConnection
}