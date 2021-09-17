const getRequest = async (request, response, db) => {
    response.writeHead(200, {'Content-Type': 'application/json; charset=utf-8'});
    response.end(JSON.stringify(db.select()));
}

const postRequest = async (request, response, db) => {
    let user;

    request.on("data", data => {
        user = db.insert(JSON.parse(data));
    });

    request.on("end", () => response.end(JSON.stringify(user)))
}

const putRequest = async (request, response, db) => {
    request.on("data", data => {
        try {
            db.update(JSON.parse(data));
        } catch (e) {
            response.statusCode = 404;
            response.end(e.message);
        }
    });

    request.on("end", () => response.end())
}

const deleteRequest = async (request, response, db) => {
    const baseURL =  request.protocol + '://' + request.headers.host + '/';
    const requestUrl = new URL(request.url, baseURL);
    const id = requestUrl.searchParams.get("id");

    if (id === null) {
        response.statusCode = 400;
        response.end("Specify the id");
        return;
    }

    try {
        const deletedUser = db.delete(Number(id));
        response.end(JSON.stringify(deletedUser));
    } catch (e) {
        response.statusCode = 404;
        response.end(e.message);
    }
}

module.exports = {
    getRequest,
    postRequest,
    putRequest,
    deleteRequest
}