const http = require("http");
const mailSender = require("m0603_harwex");

// https://ethereal.email/messages
// mailSender.send("Test", "Nu davay razberem tobou napisannoe...");

http.createServer((request, response) => {
    response.writeHead(200, {"Content-type": "text/html"});
    response.end(`<div>${state}</div>`)
}).listen(4000);