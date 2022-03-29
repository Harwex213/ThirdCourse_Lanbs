const { sequelize } = require("../sequelize/sequelize");

const getAllRepos = async () => {
    return await sequelize.model("repos").findAll({
        raw: true
    });
}

const findRepo = async ({ id }) => {
    return await sequelize.model("repos").findOne({
        where: {
            id: id
        },
        raw: true
    })
}

const createRepo = async ({ name, authorId }) => {
    return await sequelize.model("repos").create({
        name,
        authorId
    })
}

const updateRepo = async ({ id, name, authorId }) => {
    return await sequelize.model("repos").update({
        name,
        authorId
    }, {
        where: {
            id: id
        }
    })
}

const destroyRepo = async ({ id }) => {
    return await sequelize.model("repos").destroy({
        where: {
            id: id
        }
    })
}

const getAllCommitsOfRepo = async ({ repoId }) => {
    return await sequelize.model("commits").findAll({
        where: {
            repoId: repoId
        },
        raw: true
    });
}

const findCommitOfRepo = async ({ id, repoId }) => {
    return await sequelize.model("commits").findOne({
        where: {
            id: id,
            repoId: repoId
        },
        raw: true
    })
}

const findRepoOwner = async ({ repoId }) => {
    const repo = await sequelize.model("repos").findOne({
        where: {
            id: repoId
        }
    })

    if (repo) {
        return await sequelize.model("users").findOne({
            where: {
                id: repo.authorId
            }
        })
    }

    return null;
}

const createCommit = async ({ repoId, message }) => {
    return await sequelize.model("commits").create({
        repoId,
        message
    })
}

const updateCommit = async ({ id, repoId, message }) => {
    return await sequelize.model("commits").update({
        repoId,
        message,
    }, {
        where: {
            id: id
        }
    })
}

const destroyCommit = async ({ id, repoId }) => {
    return await sequelize.model("commits").destroy({
        where: {
            id: id,
            repoId: repoId
        }
    })
}

module.exports = {
    getAllRepos,
    findRepo,
    createRepo,
    updateRepo,
    destroyRepo,
    getAllCommitsOfRepo,
    findCommitOfRepo,
    findRepoOwner,
    createCommit,
    updateCommit,
    destroyCommit
}
