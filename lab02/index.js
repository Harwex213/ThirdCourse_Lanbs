const http = require("http");
const bodyParser = require("body-parser");
const express = require("express");
const { handleEntropy, handleInformationCount} = require("./core");

const app = express();

app.use(bodyParser.json());

const views = ["/entropy", "/info-count"];
views.forEach((view) => {
    app.get(view, (request, response) => {
        response.sendFile(__dirname + `/views${view}.html`)
    });
});

app.post("/api/entropy", handleEntropy);
app.post("/api/info-count", handleInformationCount);

http.createServer(app).listen(3000, "localhost", () => {
    console.log("Server started");
});