const express = require("express");
const app = express();
const fs = require("fs");

app.use("/", express.static("public"));

const code = fs.readFileSync("public/code.wasm");
const wasmModule = new WebAssembly.Module(code);
const instance = new WebAssembly.Instance(wasmModule, {});

app.get("/task-1", (request, response) => {
    response.sendFile(__dirname + "/views/task-1.html");
})

app.get("/task-2", (request, response) => {
    response.sendFile(__dirname + "/views/task-2.html");
})

app.get("/task-3", (request, response) => {
    response.type("html").send(
        `<h1>Practice with WASM. Task 3</h1>
<div>
    <p>sum(10, 10): ${instance.exports.sum(10, 10)}</p>
    <p>sub(10, 5): ${instance.exports.sub(10, 5)}</p>
    <p>mul(10, 10): ${instance.exports.mul(10, 10)}</p>
    <p>div(10, 5): ${instance.exports.div(10, 5)}</p>
</div>`)
});

app.listen(3000, () => {
    console.log("We started");
});
