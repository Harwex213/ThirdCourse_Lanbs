const express = require("express");
const https = require("https");
const fs = require("fs");

const options = {
    key: fs.readFileSync("./certificate/RS-LAB25-KOA.key").toString(),
    cert: fs.readFileSync("./certificate/RS-LAB25-KOA.crt").toString()
};

const app = express();

app.use(require("body-parser").urlencoded({
    extended: false
}))

let id = 18;
app.get("/", (req, res) =>
{
    res.send("Hello from Harwex. age " + id++);
})

https.createServer(options, app).listen({
    port: 3000
}, (host) =>
{
    console.log(host);
});
