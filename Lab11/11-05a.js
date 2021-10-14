const RpcWebsocketClient = require("rpc-websockets").Client;

const client = new RpcWebsocketClient("ws://localhost:4000");

const makeCall = async (method, params, resultMessage) => {
    try {
        const result = await client.call(method, params);
        console.log(resultMessage + ": " + result);
    } catch (e) {
        console.log(e.message);
    }
}

client.on("open", async () => {
    await makeCall("square", { r: 3 }, "square(3)");
    await makeCall("square", { a: 3, b: 5}, "square(3,5)");

    await makeCall("sum", [2], "sum(2)");
    await makeCall("sum", [2, 4, 6, 8, 10], "sum(2, 4, 6, 8, 10)");

    await makeCall("mul", [3], "mul(3)");
    await makeCall("mul", [3, 5, 7, 9, 11, 13], "mul(3, 5, 7, 9, 11, 13)");

    client.login({ login: "Harwex", password: "1111" }).then(async () => {
        await makeCall("fib", [1] , "fib(1)");
        await makeCall("fib", [2], "fib(2)");
        await makeCall("fib", [7], "fib(7)");

        await makeCall("fact", [0], "fact(0)");
        await makeCall("fact", [5], "fact(5)");
        await makeCall("fact", [10], "fact(10)");
    })
})