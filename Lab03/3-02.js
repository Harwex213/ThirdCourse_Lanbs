const http = require("http");
const fs = require("fs");
const process = require("process");

const fact = (n) => (n !== 1 ? n * fact(n - 1) : n);

const calcFactNextTick = (n, callback) => {
    process.nextTick(() => {
        callback(fact(n));
    });
}
const calcFactImmediate = (n, callback) => {
    setImmediate(() => {
        callback(fact(n));
    });
}


http.createServer((request, response) => {
    const baseURL =  request.protocol + '://' + request.headers.host + '/';
    const requestUrl = new URL(request.url,baseURL);
    let k = null;

    switch (requestUrl.pathname) {
        case "/fact":
            response.writeHead(200, {"Content-type": "application/json"});
            k = requestUrl.searchParams.get("k");
            response.end(JSON.stringify({
                k,
                fact: String(fact(Number(k)))
            }));
            break;
        case "/fact/next-tick":
            response.writeHead(200, {"Content-type": "application/json"});
            k = requestUrl.searchParams.get("k");
            calcFactNextTick(Number(k), (fact) => response.end(JSON.stringify({ k, fact })));
            break;
        case "/fact/immediate":
            response.writeHead(200, {"Content-type": "application/json"});
            k = requestUrl.searchParams.get("k");
            calcFactImmediate(Number(k), (fact) => response.end(JSON.stringify({ k, fact })));
            break;
        case "/":
            response.writeHead(200, {"Content-type": "text/html"});
            response.write(fs.readFileSync("index.html"));
            response.end();
            break;
    }
}).listen(4000);

console.log("We started");