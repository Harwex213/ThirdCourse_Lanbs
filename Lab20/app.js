const express = require("express");
const bodyParser = require("body-parser");
const { engine } = require("express-handlebars");
const controller = require("./controllers/MainController");

const app = express();

app.engine(".hbs", engine({
    extname: ".hbs",
    helpers: require("./helpers")
}));
app.set("view engine", ".hbs");
app.set('views', './views');

app.use(bodyParser.urlencoded({
    extended: false,
}));
app.use(bodyParser.json());
app.use("/static", express.static(__dirname + "/static"));

app.use("/", controller);

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

app.listen(process.env.PORT || 80, () => {
    console.log("server successfully started");
})