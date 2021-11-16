const fs = require("fs");
const router = require("../router/router")();

router.get("/", (request, response) => {
    const index = fs.readFileSync(__dirname + "/../views/index.html");
    response.writeHead(200, {
        "Content-Type" : "text/html"
    })
    response.end(index);
});

module.exports = router;