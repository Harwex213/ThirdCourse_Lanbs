const http = require("http");
const send = require("m0603_harwex");
const fs = require("fs");

// https://ethereal.email/messages

const sendIndex = (_, response) => {
    response.writeHead(200, {
        "Content-type": "text/html"
    });
    response.write(fs.readFileSync("index.html"));
    response.end();
}

const sendMail = (request, response) => {
    let mailBody;
    request.on("data", (data) => {
        mailBody = JSON.parse(data);
    })
    request.on("end", async () => {
        try {
            await send(mailBody.subject, mailBody.message);
            response.writeHead(200);
            response.end(`Message send.`);
        } catch (e) {
            response.writeHead(500);
            response.end(`Interval error.\n ${e.message}`);
        }
    })
}

const routes = {
    "/": sendIndex,
    "/sendMail": sendMail
}

http.createServer((request, response) => {
    const baseURL =  request.protocol + '://' + request.headers.host + '/';
    const requestUrl = new URL(request.url, baseURL);

    const controllerMethod = routes[requestUrl.pathname];
    if (controllerMethod) {
        controllerMethod(request, response);
    }
    else {
        response.end();
    }
}).listen(4000, "localhost", () => console.log("We started"));