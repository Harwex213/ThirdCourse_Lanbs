const express = require("express");
const session = require("express-session");
const passport = require("./passportInit");

const app = express();

app.use(session({
    resave: false,
    saveUninitialized: false,
    secret: "SUPER_SECRET"
}))
app.use(passport.initialize());
app.use(passport.session());

app.get("/auth/login", ((request, response, next) => {
    if (request.session.isLogout === undefined)
    {
        request.session.isLogout = false;
    }
    if (request.session.isLogout && request.headers["authorization"]) {
        request.session.isLogout = false;
        delete request.headers["authorization"];
    }

    next();
}), passport.authenticate(process.argv[2] ?? "basic"), (request, response) => {
    response.json({
        message: "Login",
        session: request.session
    });
});
app.get("/auth/logout", (request, response) => {
    request.session.isLogout = true;
    delete request.headers["authorization"];
    response.end("Success logout");
});
app.get("/auth/resource", (request, response) => {
    if (request.session.isLogout !== undefined &&
        request.session.isLogout !== true &&
        request.headers["authorization"]) {
        response.json({
            message: "Resource",
            session: request.session
        });
        return;
    }

    response.redirect("/auth/login");
});

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
            message: error.message
        }
    });
});

app.listen(3000, "localhost", () => {
    console.log("server successfully started");
})