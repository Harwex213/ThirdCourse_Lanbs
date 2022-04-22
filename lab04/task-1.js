const alphabetStrToMap = (alphabet) => {
    const alphabetMap = new Map();
    for (let i = 0; i < alphabet.length; i++) {
        alphabetMap.set(alphabet[i], 0);
    }
    return alphabetMap;
}


const calcPercentage = (alphabet, sample) => {
    const occurrences = alphabetStrToMap(alphabet);
    for (let i = 0; i < sample.length; i++) {
        const amount = occurrences.get(sample[i]);
        if (typeof amount !== "undefined") {
            occurrences.set(sample[i], amount + 1);
        }
        else {
            occurrences.set(sample[i], 0);
        }
    }
    const percentage = Array.from(occurrences, ([_, value]) => value);
    for (let i = 0; i < percentage.length; i++) {
        percentage[i] = percentage[i] / sample.length;
    }

    return {
        chars: Array.from(occurrences, ([key, _]) => key),
        percentage
    }
}

const generateSequence = (start, to) => Array.from({ length: to }, (_, i) => i + start);

const polishAlphabet = "aąbcćdeęfghijklłmnńoópqrsśtuvwxyzźż ,.!?:-;()[]'\"\n\r";

const currentAlphabet = polishAlphabet;
const alphabetIndexes = generateSequence(0, currentAlphabet.length);
const cipherAlphabetIndexes = (() => {
    const indexes = [];
    for (let i = 0; i < alphabetIndexes.length; i++) {
        let newIndex = 5 * i + 7;
        while (newIndex > alphabetIndexes.length) {
            newIndex -= alphabetIndexes.length;
        }
        indexes.push(newIndex);
    }

    return indexes;
})();
const [encryptTable, decryptTable] = (() => {
    const encryptTable = new Map();
    const decryptTable = new Map();
    for (let i = 0; i < currentAlphabet.length; i++) {
        encryptTable.set(currentAlphabet[i], currentAlphabet[cipherAlphabetIndexes[i]]);
        decryptTable.set(currentAlphabet[cipherAlphabetIndexes[i]], currentAlphabet[i]);
    }
    return [encryptTable, decryptTable];
})();

const encryptViaAffineSubstitutions = (sample) => {
    let encrypted = "";
    for (let i = 0; i < sample.length; i++) {
        encrypted += encryptTable.get(sample[i]) ?? "";
    }

    return encrypted;
}

const decryptViaAffineSubstitutions = (encrypted) => {
    let decrypted = "";
    for (let i = 0; i < encrypted.length; i++) {
        decrypted += decryptTable.get(encrypted[i]) ?? "";
    }

    return decrypted;
}

console.log(encryptTable);
console.log(decryptTable);
console.log(alphabetIndexes.join(" "));
console.log(cipherAlphabetIndexes.join(" "))

// const sample = "anya i aleg eto lubaw";
// const encrypted = encryptViaAffineSubstitutions(sample);
// console.log("-- encrypted --", encrypted);
// console.log("-- decrypted --", decryptViaAffineSubstitutions(encrypted))

const express = require("express");
const bodyParser = require("body-parser");

const app = express();

app.use(bodyParser.json());

app.get("/", (req, res) => {
    res.sendFile(__dirname + "/views/affine.html");
});
app.post("/encrypt", (req, res) => {
    const sample = req.body.text ?? "";
    const encrypted = encryptViaAffineSubstitutions(sample);

    res.json({
        data: encrypted,
        ...calcPercentage(polishAlphabet, encrypted)
    });
});
app.post("/decrypt", (req, res) => {
    const sample = req.body.text ?? "";
    const decrypted = decryptViaAffineSubstitutions(sample);

    res.json({
        data: decrypted,
        ...calcPercentage(polishAlphabet, decrypted)
    });
});

app.listen(3000, "localhost", () => {
    console.log("Server successfully started.");
});
