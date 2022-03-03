const jwt = require("jsonwebtoken");
const accessKeySecret = "superSecret_access";
const refreshKeySecret = "superSecret_refresh";

const createAccessToken = (payload) => {
    return jwt.sign({...payload}, accessKeySecret, {
        expiresIn: 10 * 60,
    })
}

const createRefreshToken = (payload) => {
    return jwt.sign({...payload}, refreshKeySecret, {
        expiresIn: 10 * 60,
    })
}

const verifyAndDecodeAccessToken = (accessToken) => {
    return jwt.verify(accessToken, accessKeySecret, {
        complete: true,
    });
}

const verifyAndDecodeRefreshToken = (refreshToken) => {
    return jwt.verify(refreshToken, refreshKeySecret, {
        complete: true,
    });
}

module.exports = {
    createAccessToken,
    createRefreshToken,
    verifyAndDecodeAccessToken,
    verifyAndDecodeRefreshToken
}