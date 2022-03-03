const {
    verifyAndDecodeAccessToken
} = require("../services/authService");

const authMiddleware = (request, response, next) => {
    if (typeof request.cookies.accessToken === "undefined") {
        next();
        return;
    }

    try {
        const decoded = verifyAndDecodeAccessToken(request.cookies.accessToken);
        request.payload = decoded.payload;
    }
    catch (e) {
    }

    next();
}

module.exports = authMiddleware;