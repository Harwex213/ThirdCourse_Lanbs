const fs = require("fs");

const polishAlphabet = "aąbcćdeęfghijklłmnńoópqrsśtuvwxyzźż ,.!?:-;()[]'\"\n";
const russianAlphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

const [encryptTable, decryptTable] = ((alphabet) => {
    console.log(`Alphabet length: ${alphabet.length}`);

    const encryptTable = new Map();
    const decryptTable = new Map();
    let code = 1;
    for (let i = 0; i < alphabet.length; i++) {
        encryptTable.set(alphabet[i], new Map());
        for (let j = 0; j < alphabet.length; j++) {
            encryptTable.get(alphabet[i]).set(alphabet[j], code);
            decryptTable.set(code, alphabet[i] + alphabet[j])

            code++;
        }
    }

    return [encryptTable, decryptTable];
})(polishAlphabet);

const encryptViaCipherPorta = (sample) => {
    let textToEncrypt = sample.length % 2 === 0 ? sample : sample + " ";
    textToEncrypt = textToEncrypt.toLowerCase()
        .replaceAll(/\r\n/gi, "");

    let encrypted = [];
    for (let i = 0; i < textToEncrypt.length; i+=2) {
        let charOne = encryptTable.get(textToEncrypt[i]);
        let charTwo = charOne.get(textToEncrypt[i + 1]);
        encrypted.push(charTwo);
    }

    return encrypted;
}

const decryptViaCipherPorta = (encrypted) => {
    let decrypted = "";
    for (let i = 0; i < encrypted.length; i++) {
        decrypted += decryptTable.get(encrypted.charCodeAt(i))
    }

    return decrypted
}

const sample = fs.readFileSync("./text-copy.txt");
console.log("text length: " + sample.length)
console.time("encrypt");
const encrypted = String.fromCharCode(...encryptViaCipherPorta(sample.toString()));
fs.writeFileSync("./encrypted.txt", encrypted);
console.timeEnd("encrypt");

console.time("decrypt");
const decrypted = decryptViaCipherPorta(encrypted);
fs.writeFileSync("./decrypted.txt", decrypted);
console.timeEnd("decrypt");
