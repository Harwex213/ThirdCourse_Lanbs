const express = require("express");
const bodyParser = require("body-parser");
const cookieParser = require("cookie-parser");
const authMiddleware = require("../middleware/authMiddleware");
const authorizationMiddleware = require("../middleware/authorizationMiddleware");
const formsAuthController = require("../controllers/formsAuthController");
const tokensAuthController = require("../controllers/tokensAuthController");
const abilityController = require("../controllers/abilityController");
const usersController = require("../controllers/usersController");
const reposController = require("../controllers/reposController");

const app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: true
}));

if (process.argv[2] === "tokens") {
    app.use(cookieParser("cookie_key"));
    
    app.use("/auth/", authMiddleware, tokensAuthController);
    app.use("/api/ability", authMiddleware, authorizationMiddleware, abilityController);
    app.use("/api/users", authMiddleware, authorizationMiddleware, usersController);
    app.use("/api/repos", authMiddleware, authorizationMiddleware, reposController);
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