const { JSONRPCServer } = require("json-rpc-2.0");

const server = new JSONRPCServer();

server.addMethod("sum", ({ numbers }) => numbers.reduce((previousValue, currentValue) =>
    previousValue + currentValue));
server.addMethod("mul", ({ numbers }) => numbers.reduce((previousValue, currentValue) =>
    previousValue * currentValue));
server.addMethod("div", ({ x, y }) => x / y);
server.addMethod("proc", ({ x, y }) => x / y * 100);

module.exports = server;