const express = require("express");
const bodyParser = require("body-parser");
const jsonServer = require("./rpc");

const app = express();
app.use(bodyParser.json());

app.post("/json-rpc", async (req, res, next) => {
    try {
        const jsonRpcRequest = req.body;
        const jsonRpcResponse = await jsonServer.receive(jsonRpcRequest);
        if (jsonRpcResponse) {
            res.json(jsonRpcResponse);
        } else {
            res.sendStatus(204);
        }
    } catch (e) {
        e.status = 500;
        e.stack = null;
        next(e);
    }
});

app.use((request, response, next) => {
    const error = new Error("Bad request");
    error.status = 400;
    error.stack = null;
    next(error);
});

app.use((error, request, response, next) => {
    response.status(error.status || 500);
    response.json({
        error: {
            message: error.message
        }
    });
});

app.listen(3000, () => {
    console.log("Server successfully started")
});
