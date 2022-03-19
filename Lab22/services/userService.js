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

const createUser = async ({ username, password, email }) => {
    return await sequelize.model("users").create({
        username,
        password,
        email,
        role: "user"
    })
}

module.exports = {
    getUserByUsername,
    checkUserPassword,
    createUser
}