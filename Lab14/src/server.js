const http = require("http");
const isUseGraphQl = require("config").get("useGraphQl");
const app = require(`./${isUseGraphQl ? "appGraphQl" : "app"}`);

http.createServer(app).listen(4000, "localhost", () => {
    console.log("Server started");
});