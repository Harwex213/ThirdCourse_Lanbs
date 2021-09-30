const process = require("process");
const server = require("../serverInit");
const database = require("./../databaseInit");
const statistic = require("./../statistic/statistic");

const getDuration = (stringToParse) => {
    const number = Number(stringToParse);
    return number < 0 ? 0 : number * 1000;
}

let stopServerTimerId = null;
let autoCommitIntervalId = null;

process.stdin.unref();
process.stdin.setEncoding("utf-8");
process.stdin.on("readable", () => {
    let chunk = null;
    while ((chunk = process.stdin.read()) != null) {
        const [command, x] = chunk.replace(/ +/g, ' ').trim().split(" ");
        switch (command) {
            case "sd":
                if (stopServerTimerId) {
                    clearTimeout(stopServerTimerId);
                }
                if (x) {
                    stopServerTimerId = setTimeout(() => server.close(), getDuration(x));
                }
                break;
            case "sc":
                if (autoCommitIntervalId) {
                    clearInterval(autoCommitIntervalId);
                }
                if (x) {
                    autoCommitIntervalId = setInterval(() => database.emit("COMMIT"), getDuration(x));
                    autoCommitIntervalId.unref();
                }
                break;
            case "ss":
                if (x) {
                    statistic.startGathering(getDuration(x));
                }
                else {
                    statistic.stopGathering()
                }
                break;
        }
    }
})