const parseRequestUrl = (request) => {
    const baseURL = "http" + "://" + request.headers.host + "/";
    return new URL(request.url, baseURL);
}

module.exports = {
    parseRequestUrl
}