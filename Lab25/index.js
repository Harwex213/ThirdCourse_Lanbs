const express = require("express");
const https = require("https");
const fs = require("fs");

const options = {
    key: fs.readFileSync("./certificate/RS-LAB25-KOA.key").toString(),
    cert: fs.readFileSync("./certificate/RS-LAB25-KOA.crt").toString()
};

const app = express();

app.get("/", (req, res) =>
{
    res.send("Hello from Harwex.");
})

https.createServer(options, app).listen({
    port: 3000
}, (host) =>
{
    console.log(host);
});
