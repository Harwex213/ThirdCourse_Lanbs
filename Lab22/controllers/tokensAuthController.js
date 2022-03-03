const express = require("express");
const { checkUserPassword, getUserByUsername } = require("../services/userService");
const { createAccessToken, createRefreshToken, verifyAndDecodeRefreshToken } = require("../services/authService");
const path = require("path");
const { createClient } = require("redis");

const redisClient = createClient();
(async () => {
    await redisClient.connect();
})();

const getLogin = (request, response) => {
    response.sendFile(path.resolve("views/loginForm.html"));
}

const postLogin = async (request, response) => {
    const { username, password } = request.body;
    const user = await checkUserPassword(username, password);
    if (user === null) {
        response.redirect("/auth/login");
        return;
    }

    const payload = {
        id: user.id,
        username: user.username,
        role: user.role
    };
    const accessToken = createAccessToken(payload)
    const refreshToken = createRefreshToken(payload);

    response.cookie("accessToken", accessToken, {
        httpOnly: true,
        sameSite: "strict"
    });
    response.cookie("refreshToken", refreshToken, {
        httpOnly: true,
        sameSite: "strict",
        path: "/auth/refresh-token"
    });
    response.redirect("/auth/resource");
}

const checkRefreshToken = async (request, response, next) => {
    const createError = (message) => {
        const error = new Error(message);
        error.status = 401;
        return error;
    }

    if (typeof request.cookies.refreshToken === "undefined") {
        next(createError("Refresh token not found"));
        return;
    }

    try {
        const decodedRefresh = verifyAndDecodeRefreshToken(request.cookies.refreshToken);

        const usedRefreshToken = await redisClient.get(request.cookies.refreshToken);
        if (usedRefreshToken !== null) {
            next(createError("Refresh token already has been used"));
            return;
        }
        await redisClient.set(request.cookies.refreshToken, "1");

        const user = await getUserByUsername(decodedRefresh.payload.username);
        if (user === null) {
            next(createError("User doesn't exist"));
            return;
        }

        const payload = {
            id: user.id,
            username: user.username,
            role: user.role
        };
        const accessToken = createAccessToken(payload)
        const refreshToken = createRefreshToken(payload);

        response.cookie("accessToken", accessToken, {
            httpOnly: true,
            sameSite: "strict"
        });
        response.cookie("refreshToken", refreshToken, {
            httpOnly: true,
            sameSite: "strict",
            path: "/auth/refresh-token"
        });
        response.redirect("/auth/resource");
    }
    catch (e) {
        next(createError("Invalid refresh token"));
    }
}

const getLogout = (request, response) => {
    response.clearCookie("accessToken");
    response.clearCookie("refreshToken", {
        path: "/auth/refresh-token"
    });
    response.redirect("/auth/login");
}

const getResource = (request, response) => {
    if (request.payload) {
        response.send(`Hello, ${request.payload.username}. Your role - ${request.payload.role}`);
        return;
    }
    response.redirect("/auth/login");
}

const router = express.Router();

router.get("/login", getLogin);
router.post("/login", postLogin);
router.get("/refresh-token", checkRefreshToken)
router.get("/logout", getLogout)
router.get("/resource", getResource);

module.exports = router;