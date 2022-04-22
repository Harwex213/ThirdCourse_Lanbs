const crypto = require("crypto");
const sjcl = require("sjcl-complete");

const { publicKey, privateKey } = crypto.generateKeyPairSync("rsa", {
    modulusLength: 2048,
});

const onRsaEncrypt = () => {
    try {
        const startTime = performance.now();
        const data = document.querySelector("#rsa-data").value;
        const encryptedData = crypto.publicEncrypt(publicKey, Buffer.from(data),);
        const endTime = performance.now();

        document.querySelector("#rsa-after-encrypt").innerHTML = encryptedData.toString("base64");
        document.querySelector("#rsa-after-encrypt-time").innerHTML = "Exec time: " + (endTime - startTime);
    } catch (e) {
        document.querySelector("#error").innerHTML = e.message;
    }
}

const onRsaDecrypt = () => {
    try {
        const startTime = performance.now();
        const encryptedData = document.querySelector("#rsa-encrypted").value;
        const decryptedData = crypto.privateDecrypt(privateKey, Buffer.from(encryptedData, "base64"));
        const endTime = performance.now();

        document.querySelector("#rsa-after-decrypt").innerHTML = decryptedData.toString();
        document.querySelector("#rsa-after-decrypt-time").innerHTML = "Exec time: " + (endTime - startTime);
    } catch (e) {
        document.querySelector("#error").innerHTML = e.message;
    }
}

const elGamalKeys = sjcl.ecc.elGamal.generateKeys(sjcl.ecc.curves.k256);
const publicKem = elGamalKeys.pub.kem();
const egPublicKey = publicKem.key;
const egSecretKey = elGamalKeys.sec.unkem(publicKem.tag);

const onElGamalEncrypt = () => {
    try {
        const startTime = performance.now();
        const data = document.querySelector("#eg-data").value;
        const encrypted = sjcl.encrypt(egPublicKey, data);
        const endTime = performance.now();

        document.querySelector("#eg-after-encrypt").innerHTML = Buffer.from(encrypted).toString("base64");
        document.querySelector("#eg-after-encrypt-time").innerHTML = "Exec time: " + (endTime - startTime);
    } catch (e) {
        console.log(e);
    }
}

const onElGamalDecrypt = () => {
    try {
        const startTime = performance.now();
        const encryptedData = document.querySelector("#eg-encrypted").value;
        const decoded = Buffer.from(encryptedData, "base64").toString();
        const decrypted = sjcl.decrypt(egSecretKey, decoded);
        const endTime = performance.now();

        document.querySelector("#eg-after-decrypt").innerHTML = decrypted;
        document.querySelector("#eg-after-decrypt-time").innerHTML = "Exec time: " + (endTime - startTime);
    } catch (e) {
        console.log(e);
    }
}
