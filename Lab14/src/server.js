const http = require("http");
const app = require("./app");

http.createServer(app).listen(4000, "localhost", () => {
    console.log("Server started");
});