const http = require("http");
const { createDiffieHellman, createDecipheriv, createCipheriv, createVerify } = require("crypto");
const fs = require("fs/promises");

let clientDiffieHellman = null;
let clientKey = null;
let secret = null;
let iv = null;
let serverPublicKey = null;

const getServerContext = () => new Promise((resolve, reject) => {
    const options = {
        host: "localhost",
        port: 3000,
        path: "/handshake/server-context",
        method: "POST",
        headers: {
            "Content-Type": "application/json",
            "Accept": "application/json"
        }
    }

    http.request(options, (response) => {
        let body = "";
        response.on("data", (chunk) => body += chunk);
        response.on("end", () => {
            if (!body) {
                reject("Server context has not been received");
                return;
            }
            const serverContext = JSON.parse(body);
            console.log("Received server context", serverContext);

            clientDiffieHellman = createDiffieHellman(serverContext.prime, "hex", serverContext.generator, "hex");
            clientKey = clientDiffieHellman.generateKeys("hex");
            console.log("Client key: " + clientKey);

            secret = clientDiffieHellman.computeSecret(serverContext.serverKey, "hex");
            console.log("Client secret: " + secret.toString("hex"));
            resolve();
        })
    }).end();
});

const sendClientContext = () => new Promise((resolve, reject) => {
    const options = {
        host: "localhost",
        port: 3000,
        path: "/handshake/new-client",
        method: "POST",
        headers: {
            "Content-Type": "application/json",
            "Accept": "application/json"
        }
    }

    const request = http.request(options, (response) => {
        if (response.statusCode !== 200) {
            reject("Server response status code is " + response.statusCode);
            return;
        }

        let body = "";
        response.on("data", (chunk) => body += chunk);
        response.on("end", () => {
            if (response.statusCode !== 200) {
                reject("Server response status code is " + response.statusCode);
                return;
            }
            if (!body) {
                reject("Server handshake-check-info has not been received");
                return;
            }
            const checkInfo = JSON.parse(body);
            console.log("Received handshake-check-info", checkInfo);

            serverPublicKey = checkInfo.publicRsaKey;
            iv = Buffer.from(checkInfo.iv, "hex");
            const decipher = createDecipheriv("aes-256-gcm", secret, iv);
            const sessionId = decipher.update(checkInfo.data, "hex", "utf-8");
            resolve(sessionId);
        })
    });
    request.write(JSON.stringify({
        clientKey
    }));
    request.end();
});

const checkClientKey = (sessionId) => new Promise((resolve, reject) => {
    const options = {
        host: "localhost",
        port: 3000,
        path: "/handshake/check",
        method: "POST",
        headers: {
            "Content-Type": "application/json",
            "Accept": "application/json"
        }
    }
    const dataToEncrypt = Number(sessionId) + 10;
    const cipher = createCipheriv("aes-256-gcm", secret, iv);
    const encrypted = cipher.update(dataToEncrypt.toString(), "utf-8", "hex");

    const request = http.request(options, (response) => {
        if (response.statusCode !== 200) {
            reject("Server response status code is " + response.statusCode);
            return;
        }

        console.log("Client check was successful");
        resolve();
    });
    request.write(JSON.stringify({
        sessionId: sessionId,
        data: encrypted
    }));
    request.end();
});

const getResource = (sessionId) => new Promise((resolve, reject) => {
    console.log("Attempting get resource");

    const options = {
        host: "localhost",
        port: 3000,
        path: `/resource?sessionId=${sessionId}`,
        method: "GET",
        headers: {
            "Content-Type": "text/plain",
            "Accept": "text/plain"
        }
    }

    http.request(options, (response) => {
        if (response.statusCode !== 200) {
            reject("Server response status code is " + response.statusCode);
            return;
        }

        let body = "";
        response.on("data", (chunk) => body += chunk);
        response.on("end", () => {
            if (response.statusCode !== 200) {
                reject("Server response status code is " + response.statusCode);
                return;
            }

            const [data, signature] = body.split("$");

            console.log("Received encrypted resource from server");
            const decipher = createDecipheriv("aes-256-gcm", secret, iv);
            const decrypted = decipher.update(data, "hex", "utf-8");
            const verify = createVerify("SHA256");
            verify.update(decrypted);
            verify.end();
            if (verify.verify(serverPublicKey, signature, "hex") === false) {
                reject("Digital Signature verifying error");
            }
            resolve({
                encrypted: body,
                decrypted: decrypted
            });
        })
    }).end();
});

(async () => {
    try {
        await getServerContext();
        const sessionId = await sendClientContext();
        await checkClientKey(sessionId);
        const { encrypted, decrypted } = await getResource(sessionId);
        fs.writeFile("client-encrypted.txt", encrypted);
        fs.writeFile("client-decrypted.txt", decrypted);
    } catch (e) {
        console.log("Error in client - ", e);
    }
})();
