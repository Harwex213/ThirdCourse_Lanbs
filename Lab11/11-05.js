const RpcWebsocketServer = require("rpc-websockets").Server;

const square = (args) => {
    const { r, a, b } = args;

    if (r) {
        return Math.PI * r * r;
    }

    if (a && b) {
        return a * b;
    }

    return 0;
}

const sum = (params) => params.reduce((previousValue, currentValue) => previousValue + currentValue);

const mul = (params) => params.reduce((previousValue, currentValue) => previousValue * currentValue);

const fib = (n) => {
    const result = [];

    for (let i = 0; i < n; i++) {
        switch (i) {
            case 0:
                result.push(0);
                break;
            case 1:
                result.push(1);
                break;
            case 2:
                result.push(1);
                break;
            default:
                result.push(result[i - 2] + result[i - 1]);
                break;
        }
    }

    return result;
}

const fibSum = (n) => n < 2 ? n : fibSum(n - 1) + fibSum(n - 2);

const factorial = (n) => n === 0 ? 1 : n * factorial(n - 1);

const server = new RpcWebsocketServer({
    port: 4000,
    host: "localhost",
    path: "/"
});

server.register("square", square).public();
server.register("sum", sum).public();
server.register("mul", mul).public();
server.register("fib", (n) => fib(n[0])).protected();
server.register("fibSum", (n) => fibSum(n[0])).protected();
server.register("fact", (n) => factorial(n[0])).protected();

server.setAuth(credentials => credentials.login === "Harwex" && credentials.password === "1111");