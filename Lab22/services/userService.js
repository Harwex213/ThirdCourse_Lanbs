const { sequelize } = require("../sequelize/sequelize");

const getUserByUsername = (username) => {
    return sequelize.model("users").findOne({
        where: {
            username: username
        },
        raw: true
    })
}

const checkUserPassword = async (username, password) => {
    const user = await getUserByUsername(username);

    if (user && user.password === password) {
        return user;
    }

    return null;
}

module.exports = {
    getUserByUsername,
    checkUserPassword
}