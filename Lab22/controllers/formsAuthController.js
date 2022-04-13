const express = require("express");
const passport = require("../passport/passport");
const path = require("path");

const getLogin = (request, response) => {
    response.sendFile(path.resolve("views/loginForm.html"));
}

const getLogout = (request, response) => {
    request.logout();
    response.redirect("/auth/login");
}

const getResource = (request, response) => {
    if (request.user) {
        response.send(`Hello, ${request.user.username}. Your role - ${request.user.role}`);
        return;
    }

    const isRedirect = request.query.isRedirect;
    if (isRedirect) {
        response.redirect("/auth/login");
    }
    else {
        const error = new Error("user not authenticated");
        error.status = 401;
        throw error;
    }
}

const router = express.Router();

router.get("/login", getLogin);
router.post("/login", passport.authenticate("local", {
    successRedirect: "/auth/resource",
    failureRedirect: "/auth/login"
}));
router.get("/logout", getLogout)
router.get("/resource", getResource);

module.exports = router;