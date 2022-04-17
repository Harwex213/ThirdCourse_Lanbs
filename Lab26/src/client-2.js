const http = require("http");
const { createVerify } = require("crypto");

const getPublicKey = () => new Promise((resolve, reject) => {
    const options = {
        host: "localhost",
        port: 3000,
        path: `/public-key`,
        method: "GET",
        headers: {
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

            console.log("Received public key from server");
            resolve(body);
        })
    }).end();
});

const getResource = (publicKey) => new Promise((resolve, reject) => {
    console.log("Attempting get resource");

    const options = {
        host: "localhost",
        port: 3000,
        path: `/resource`,
        method: "GET",
        headers: {
            "Accept": "application/json"
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

            const serverData = JSON.parse(body);

            console.log("Received resource from server", serverData);
            const verify = createVerify("SHA256");
            verify.update(serverData.data);
            verify.end();
            resolve(verify.verify(publicKey, serverData.signature, "hex"));
        })
    }).end();
});

(async () => {
    try {
        const publicKey = await getPublicKey();
        const result = await getResource(publicKey);
        console.log("Verifying digital signature result " + result);
    } catch (e) {
        console.log("Error in client - ", e);
    }
})();
