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

const getUserById = async ({ id }) => {
    return await sequelize.model("users").findOne({
        where: {
            id: id
        },
        raw: true
    })
}

const getAllUsers = async () => {
    return await sequelize.model("users").findAll({
        raw: true
    })
}

module.exports = {
    getUserByUsername,
    checkUserPassword,
    createUser,
    getUserById,
    getAllUsers,
}