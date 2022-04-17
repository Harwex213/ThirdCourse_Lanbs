const {
    createDiffieHellman,
    createCipheriv,
    randomBytes,
    createDecipheriv,
    generateKeyPairSync, createSign
} = require("crypto");
const { readFileSync } = require("fs");

const serverResource = readFileSync("./src/server-resource.txt").toString();
const serverDiffieHellman = createDiffieHellman(256);
const serverDiffieHellmanContext = {
    serverKey: serverDiffieHellman.generateKeys("hex"),
    prime: serverDiffieHellman.getPrime("hex"),
    generator: serverDiffieHellman.getGenerator("hex"),
};

let sessionId = 0;
const sessions = { };

const express = require("express");
const bodyParser = require("body-parser");

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: false
}))

app.post("/handshake/server-context", (request, response) => {
    response.json(serverDiffieHellmanContext);
})

app.post("/handshake/new-client", (request, response) => {
    const clientKey = request.body?.clientKey;
    if (!clientKey) {
        console.log("Attempt to create new client was abandoned due to missing clientKey");
        response.sendStatus(409);
        return;
    }
    console.log("Received client key", request.body.clientKey);

    sessionId++;
    const currentSessionId = sessionId.toString();
    sessions[currentSessionId] = { };
    const currentSession = sessions[currentSessionId];
    console.log("Start creating new client session - ", currentSessionId);

    const { privateKey, publicKey } = generateKeyPairSync('rsa', {
        modulusLength: 2048,
    });

    currentSession.privateRsaKey = privateKey;
    currentSession.secret = serverDiffieHellman.computeSecret(clientKey, "hex");
    currentSession.iv = randomBytes(16);
    currentSession.checked = false;
    console.log("Session " + currentSessionId + ": ", currentSession);

    const cipher = createCipheriv("aes-256-gcm", currentSession.secret, currentSession.iv);
    const encrypted = cipher.update(currentSessionId, "utf-8", "hex");
    response.json({
        publicRsaKey: publicKey.export({
            type: "pkcs1",
            format: "pem"
        }),
        iv: currentSession.iv.toString("hex"),
        data: encrypted,
    })
})

app.post("/handshake/check", (request, response) => {
    const { sessionId, data } = request.body;
    if (!sessionId || !data) {
        console.log("Attempt to handshake check client was abandoned due to missing sessionId");
        response.sendStatus(409);
        return;
    }

    const session = sessions[sessionId];
    const decipher = createDecipheriv("aes-256-gcm", session.secret, session.iv);
    const decrypted = decipher.update(data, "hex", "utf-8");

    if ((Number(decrypted) - 10) === Number(sessionId)) {
        session.checked = true;
        response.sendStatus(200);
        console.log("Handshake check client " + sessionId + " was successful");
        return;
    }

    console.log("Attempt to handshake check client was abandoned due to invalid data");
    response.sendStatus(409);
})

app.get("/resource", (request, response) => {
    const { sessionId } = request.query;
    if (!sessionId) {
        console.log("Attempt to get resource was abandoned due to missing sessionId");
        response.sendStatus(401);
        return;
    }

    const session = sessions[sessionId];
    if (session.checked === false) {
        console.log("Attempt to get resource was abandoned due to unchecked stage of session");
        response.sendStatus(401);
        return;
    }

    const cipher = createCipheriv("aes-256-gcm", session.secret, session.iv);
    const encrypted = cipher.update(serverResource, "utf-8", "hex");

    const sign = createSign("SHA256");
    sign.update(serverResource);
    sign.end();
    const signature = sign.sign(session.privateRsaKey);

    console.log("Get Resource to session " + sessionId);
    response.send(encrypted + "$" + signature.toString("hex"));
})

app.listen(3000, () => console.log("We started"));
