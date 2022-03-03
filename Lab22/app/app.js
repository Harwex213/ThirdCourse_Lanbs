const express = require("express");
const bodyParser = require("body-parser");
const cookieParser = require("cookie-parser");
const formsAuthController = require("../controllers/formsAuthController");
const tokensAuthController = require("../controllers/tokensAuthController");
const authMiddleware = require("../middleware/authMiddleware");

const app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: true
}));

if (process.argv[2] === "tokens") {
    app.use(cookieParser("cookie_key"));
    
    app.use("/auth/", authMiddleware, tokensAuthController);
}
else {
    const passport = require("../passport/passport");
    const session = require("express-session");

    app.use(session({
        resave: false,
        saveUninitialized: false,
        secret: "secret_value"
    }));
    app.use(passport.initialize());
    app.use(passport.session());

    app.use("/auth/", formsAuthController);
}

app.use((request, response, next) => {
    const error = new Error("Not found");
    error.status = 404;
    error.stack = null;
    next(error);
});

app.use((error, request, response, next) => {
    response.status(error.status || 500);
    response.json({
        error: {
            message: error.original?.message ?? error.message
        }
    });
});


app.listen(3000, "localhost", () => {
    console.log("server successfully started");
});