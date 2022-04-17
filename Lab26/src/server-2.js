const {
    generateKeyPairSync, createSign
} = require("crypto");
const { readFileSync } = require("fs");
const express = require("express");
const bodyParser = require("body-parser");

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: false
}))

const serverResource = readFileSync("./server-resource.txt").toString();
const { privateKey, publicKey } = generateKeyPairSync('rsa', {
    modulusLength: 2048,
});

app.get("/public-key", (request, response) => {
    console.log("Get public key");
    response.send(publicKey.export({
        type: "pkcs1",
        format: "pem"
    }));
})

app.get("/resource", (request, response) => {
    const sign = createSign("SHA256");
    sign.update(serverResource);
    sign.end();
    const signature = sign.sign(privateKey, "hex");

    console.log("Get Resource");
    response.json({
        data: serverResource,
        signature: signature,
    });
});

app.listen(3000, () => console.log("We started"));
