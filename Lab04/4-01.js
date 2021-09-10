const http = require("http");
const db = new (require("./database/database"))();

const getResponse = async (request, response) => {
    response.writeHead(200, {'Content-Type': 'application/json; charset=utf-8'});
    response.end(JSON.stringify(db.select()));
}

const postResponse = async (request, response) => {
    let user;

    request.on("data", data => {
        user = db.insert(JSON.parse(data));
    });

    request.on("end", () => response.end(JSON.stringify(user)))
}

const putResponse = async (request, response) => {
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

const deleteResponse = async (request, response) => {
    const baseURL =  request.protocol + '://' + request.headers.host + '/';
    const requestUrl = new URL(request.url, baseURL);
    const id = requestUrl.searchParams.get("id");

    if (id === null) {
        response.statusCode = 400;
        response.end("Specify the id");
    }

    try {
        const deletedUser = db.delete(Number(id));
        response.end(JSON.stringify(deletedUser));
    } catch (e) {
        response.statusCode = 404;
        response.end(e.message);
    }
}

db.on("GET", getResponse);
db.on("POST", postResponse);
db.on("PUT", putResponse);
db.on("DELETE", deleteResponse);

http.createServer((request, response) => {
    const baseURL =  request.protocol + '://' + request.headers.host + '/';
    const requestUrl = new URL(request.url, baseURL);

    switch (requestUrl.pathname) {
        case "/api/db":
            switch (request.method)
            {
                case 'GET':
                case 'POST':
                case 'PUT':
                case 'DELETE':
                    db.emit(request.method, request, response);
                    break;
                default:
                    break;
            }
            break;
        case "/":

            break;
    }
}).listen(4000, () => console.log("We started"));