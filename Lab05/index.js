const express = require("express");
const bodyParser = require("body-parser");
const task1 = require("./task-1");
const task2 = require("./task-2");

const polishAlphabet = "aąbcćdeęfghijklłmnńoópqrsśtuvwxyzźż ,.!?:-;()[]'\"\n\r";

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

const app = express();

app.use(bodyParser.json());

app.get("/task1", (req, res) => {
    res.sendFile(__dirname + "/views/index.html");
});

app.get("/task2", (req, res) => {
    res.sendFile(__dirname + "/views/index.html");
});

app.post("/encrypt/task1", (req, res) => {
    const sample = req.body.text.toLowerCase() ?? "";
    console.time("task-1-encrypt");
    const encrypted = task1.encrypt(sample);
    console.timeEnd("task-1-encrypt");

    res.json({
        data: encrypted,
        ...calcPercentage(polishAlphabet, encrypted)
    });
});
app.post("/decrypt/task1", (req, res) => {
    const sample = req.body.text ?? "";
    console.time("task-1-decrypt");
    const decrypted = task1.decrypt(sample);
    console.timeEnd("task-1-decrypt");

    res.json({
        data: decrypted,
        ...calcPercentage(polishAlphabet, decrypted)
    });
});

app.post("/encrypt/task2", (req, res) => {
    const sample = req.body.text.toLowerCase() ?? "";
    console.time("task-2-encrypt");
    const encrypted = task2.encrypt(sample);
    console.timeEnd("task-2-encrypt");

    res.json({
        data: encrypted,
        ...calcPercentage(polishAlphabet, encrypted)
    });
});
app.post("/decrypt/task2", (req, res) => {
    const sample = req.body.text ?? "";
    console.time("task-2-decrypt");
    const decrypted = task2.decrypt(sample);
    console.timeEnd("task-2-decrypt");

    res.json({
        data: decrypted,
        ...calcPercentage(polishAlphabet, decrypted)
    });
});

app.listen(3000, "localhost", () => {
    console.log("Server successfully started.");
});