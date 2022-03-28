const fs = require("fs");
const rows = 4;
// const cols = 2;

function transpose(a) {
    return Object.keys(a[0]).map(function(c) {
        return a.map(function(r) { return r[c]; });
    });
}

const reverse = (message, cols) => {
    const table = [];

    let k = 0;
    for (let i = 0; i < rows; i++) {
        table.push([]);
        for (let j = 0; j < cols; j++) {
            table[i].push(message[k++]);
        }
    }

    return transpose(table).map(row => row.join("")).join("");
}

const encrypt = (message) => {
    const mod = message.length % rows;
    const addStrLength = mod > 0 ? rows - mod : 0;
    const addStr = new Array(addStrLength + 1).join(" ");
    const sourceText = message.concat(addStr);
    const cols = (sourceText.length) / rows;
    // console.log("encrypt length: ", sourceText.length);
    // console.log("cols: ", cols);

    return reverse(sourceText, cols);
}

const decrypt = (message) => {
    const cols = message.length / rows;
    // console.log("decrypt length: ", message.length);
    // console.log("cols: ", cols);

    const table = [];

    for (let i = 0; i < rows; i++) {
        table.push([]);
        let k = i;
        for (let j = 0; j < cols; j++) {
            table[i].push(message[k]);
            k += rows;
        }
    }

    return table.map(row => row.join("")).join("");
};

module.exports = {
    encrypt,
    decrypt
}
// const plain = fs.readFileSync("./test.txt").toString();
// console.log("plain text (first 150 symbols): ", plain.substring(0, 150));
//
// const encrypted = encrypt(plain);
// fs.writeFileSync("test-encrypted.txt", encrypted);
//
// console.log("encrypted (first 150 symbols): ", encrypted.substring(0, 150), encrypted.length);
//
// const decrypted = decrypt(encrypted);
// console.log("decrypted (first 150 symbols): ", decrypted.substring(0, 150));

