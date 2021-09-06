const http = require("http");

http.createServer((request, response) => {
    response.writeHead(200, {"Content-type": "text/html"});

    let headersString = "";
    for(const key in request.headers)
    {
        headersString += "<p>" + key + ": " + request.headers[key] + "</p>";
    }

    let body = ""
    request.on("data", (data) => {
        body += data;
        console.log("body", body);
    })
    request.on("end", () =>  response.end(
        `<!doctype html>
                <html lang="en">
                <head>
                    <meta charset="UTF-8">
                    <meta name="viewport"
                          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
                    <meta http-equiv="X-UA-Compatible" content="ie=edge">
                    <title>Document</title>
                </head>
                <body>
                    Method: ${request.method} <br>
                    HTTP Version: ${request.httpVersion} <br>
                    URI: ${request.url} <br>
                    Headers: ${headersString} <br>
                    Body: ${body} <br>
                </body>
                </html>`));
}).listen(4000);

console.log("We started");