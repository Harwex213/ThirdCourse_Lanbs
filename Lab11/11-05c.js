const RpcWebsocketClient = require("rpc-websockets").Client;

const client = new RpcWebsocketClient("ws://localhost:4000");

const makeCall = async (method, params, resultMessage) => {
    const result = await client.call(method, params);
    console.log(resultMessage + ": " + result);
    return result;
}

client.on("open", async () => {
    client.login({ login: "Harwex", password: "1111" }).then(async () => {
        try {
            const result = await makeCall("sum", [
                    await makeCall("square", { r: 3 }, "square(3)"),
                    await makeCall("square", { a: 5, b: 4}, "square(5, 4)"),
                    await makeCall("mul", [3, 5, 7, 9, 11, 13], "mul(3, 5, 7, 9, 11, 13)")
                ])
                + await makeCall("fibSum", [7], "fibSum(7)")
                * await makeCall("mul", [2, 4, 6], "mul(2, 4, 6)");

            console.log("Final result: " + result);
        } catch (e) {
            console.log("Error occurred: " + e.message);
        }
    })
})