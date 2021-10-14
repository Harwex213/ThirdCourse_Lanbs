const RpcWebsocketClient = require("rpc-websockets").Client;
const { parallel } = require("async");

const client = new RpcWebsocketClient("ws://localhost:4000");

const makeCall = async (method, params, resultMessage, callback) => {
    try {
        const result = await client.call(method, params);
        callback(null, resultMessage + ": " + result);
    } catch (e) {
        callback(resultMessage + ": " + e.message, null);
    }
}

const makeProtectedCall = (method, params, resultMessage, callback) => {
    client.login({ login: "Harwex", password: "1111" }).then(async () => {
        await makeCall(method, params, resultMessage, callback);
    });
}

client.on("open", () => parallel([
    (callback) => makeCall("square", { r: 3 }, "square(3)", callback),
    (callback) => makeCall("square", { a: 3, b: 5}, "square(3,5)", callback),

    (callback) => makeCall("sum", [2], "sum(2)", callback),
    (callback) => makeCall("sum", [2, 4, 6, 8, 10], "sum(2, 4, 6, 8, 10)", callback),

    (callback) => makeCall("mul", [3], "mul(3)", callback),
    (callback) => makeCall("mul", [3, 5, 7, 9, 11, 13], "mul(3, 5, 7, 9, 11, 13)", callback),

    (callback) => makeProtectedCall("fib", [1] , "fib(1)", callback),
    (callback) => makeProtectedCall("fib", [2] , "fib(2)", callback),
    (callback) => makeProtectedCall("fib", [7] , "fib(7)", callback),

    (callback) => makeProtectedCall("fact", [0], "fact(0)", callback),
    (callback) => makeProtectedCall("fact", [5], "fact(5)", callback),
    (callback) => makeProtectedCall("fact", [10], "fact(10)", callback),
], (errors, results) => {
    console.log("Occupied errors:");
    console.log(errors);

    console.log();
    console.log("Results:");
    console.log(results);
}))