const fs = require("fs");
const keyRow = "oleg";
const keyCol = "kaportsev";

const calcPriorities = (text) => {
    const textSplit = text.split("");
    const textPriorities = [...textSplit].sort();

    for (let i = 0; i < textPriorities.length; i++) {
        textSplit[textSplit.findIndex(item => item === textPriorities[i])] = i;
    }

    return textSplit;
};
const keyRowPriorities = calcPriorities(keyRow);
const keyColPriorities = calcPriorities(keyCol);

// console.log(keyRow.split(""))
// console.log(keyRowPriorities)
// console.log(keyCol.split(""))
// console.log(keyColPriorities)

const getRowsPriority = (iterations) => {
    const arr = [];
    for (let iteration = 0; iteration < iterations; iteration++) {
        for (let j = 0; j < keyRowPriorities.length; j++) {
            arr.push(keyRowPriorities[j] + (keyRowPriorities.length * iteration));
        }
    }
    return arr;
}

const insertRows = (table, iteration) => {
    for (let i = 0; i < keyRowPriorities.length; i++) {
        table.push({
            priority: keyRowPriorities[i] + (keyRowPriorities.length * iteration),
            row: new Array(keyRowPriorities.length)
        })
    }

    // console.log(table);
}

const encrypt = (message) => {
    const table = [];
    let rowsIteration = 0;
    let rowsIndex = -1;
    let colsIndex = 0;
    for (let i = 0; i < message.length; i++) {
        if (i % (keyRowPriorities.length * keyColPriorities.length) === 0) {
            insertRows(table, rowsIteration);
            rowsIteration++;
        }
        if (i % keyColPriorities.length === 0) {
            rowsIndex++;
            colsIndex = 0;
        }
        table[rowsIndex].row[colsIndex] = {
            priority: keyColPriorities[colsIndex],
            char: message[i]
        }
        colsIndex++;
    }
    const maxSymbolsCount = keyRowPriorities.length * keyColPriorities.length * rowsIteration;
    if (message.length < maxSymbolsCount) {
        for (let i = message.length; i < maxSymbolsCount; i++) {
            if (i % keyColPriorities.length === 0) {
                rowsIndex++;
                colsIndex = 0;
            }
            table[rowsIndex].row[colsIndex] = {
                priority: keyColPriorities[colsIndex],
                char: "\0"
            }
            colsIndex++;
        }
    }

    for (const row of table) {
        row.row.sort((a, b) => a.priority - b.priority);
    }
    table.sort((a, b) => a.priority - b.priority);

    // console.log(table.length);

    return table.map(item => item.row.map(item => item.char).join("")).join("");
}

const decrypt = (message) => {

    const table = [];
    let rowsIndex = -1;
    let colsIndex = 0;
    for (let i = 0; i < message.length; i++) {
        if (i % (keyRowPriorities.length * keyColPriorities.length) === 0) {
            for (let j = 0; j < keyRowPriorities.length; j++) {
                table.push(new Array(keyColPriorities.length));
            }
        }
        if (i % keyColPriorities.length === 0) {
            rowsIndex++;
            colsIndex = 0;
        }
        table[rowsIndex][colsIndex] = message[i];
        colsIndex++;
    }

    const currentCaseRowsPriorities = getRowsPriority(table.length / keyRowPriorities.length);

    let decrypted = "";
    for (let i = 0; i < table.length; i++) {
        for (let j = 0; j < table[i].length; j++) {
            decrypted += table[currentCaseRowsPriorities[i]][keyColPriorities[j]] ?? "";
        }
    }

    return decrypted.replaceAll("\0", "");
}

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