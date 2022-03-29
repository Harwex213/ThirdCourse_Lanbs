const express = require("express");
const {
    getAllRepos,
    findRepo,
    createRepo,
    updateRepo,
    destroyRepo,
    getAllCommitsOfRepo, findCommitOfRepo, findRepoOwner, createCommit, destroyCommit, updateCommit,

} = require("../services/reposService");
const { generateForbidden, generateNotFound} = require("../errors/baseErrors");
const { subject } = require("@casl/ability");

const getRepos = async (req, res, next) => {
    res.json(await getAllRepos());
};

const getRepo = async (req, res, next) => {
    res.json(await findRepo({ id: req.params.repoId }));
}

const postRepo = async (req, res, next) => {
    if (req.ability.can("create", "repos") === false) {
        next(generateForbidden());
        return;
    }

    const repo = await createRepo({
        name: req.body.name,
        authorId: req.payload?.id
    });
    res.json(repo);
}

const putRepo = async (req, res, next) => {
    const user = await findRepoOwner({ repoId: req.params.repoId });

    if (req.ability.can("update", subject("repos", { userId: parseInt(user?.id) })) === false) {
        next(generateForbidden());
        return;
    }
    if (req.payload?.role === "user" && req.payload?.id !== req.body.authorId) {
        next(generateForbidden());
        return;
    }

    const affectedCount = await updateRepo({
        id: req.params.repoId,
        name: req.body.name,
        authorId: req.body.authorId
    });

    if (affectedCount > 0) {
        res.sendStatus(200);
    } else {
        next(generateNotFound());
    }
}

const deleteRepo = async (req, res, next) => {
    const user = await findRepoOwner({ repoId: req.params.repoId });

    if (req.ability.can("delete", subject("repos", { userId: parseInt(user?.id) })) === false) {
        next(generateForbidden());
        return;
    }

    const affectedCount = await destroyRepo({
        id: req.params.repoId,
    });

    if (affectedCount > 0) {
        res.sendStatus(200);
    } else {
        next(generateNotFound());
    }
}

const getCommitsOfRepo = async (req, res, next) => {
    res.json(await getAllCommitsOfRepo({ repoId: req.params.repoId }));
}

const getCommitOfRepo = async (req, res, next) => {
    res.json(await findCommitOfRepo({ id: req.params.commitId, repoId: req.params.repoId }));
}

const postCommit = async (req, res, next) => {
    const user = await findRepoOwner({ repoId: req.params.repoId });

    if (req.ability.can("create", subject("commits", { userId: user?.id })) === false) {
        next(generateForbidden());
        return;
    }

    const commit = await createCommit({
        repoId: req.params.repoId,
        message: req.body.message,
    })
    res.json(commit);
}

const putCommit = async (req, res, next) => {
    const user = await findRepoOwner({ repoId: req.params.repoId });

    if (req.ability.can("update", subject("commits", { userId: user?.id })) === false) {
        next(generateForbidden());
        return;
    }

    const affectedCount = await updateCommit({
        id: req.params.commitId,
        repoId: req.params.repoId,
        message: req.body.message,
    })

    if (affectedCount > 0) {
        res.sendStatus(200);
    } else {
        next(generateNotFound());
    }
}

const deleteCommit = async (req, res, next) => {
    const user = await findRepoOwner({ repoId: req.params.repoId });

    if (req.ability.can("delete", subject("commits", { userId: user?.id })) === false) {
        next(generateForbidden());
        return;
    }

    const affectedCount = await destroyCommit({
        id: req.params.commitId,
        repoId: req.params.repoId,
    })

    if (affectedCount > 0) {
        res.sendStatus(200);
    } else {
        next(generateNotFound());
    }
}

const router = express.Router();

router.get("/", getRepos);
router.get("/:repoId", getRepo);
router.post("/", postRepo);
router.put("/:repoId", putRepo);
router.delete("/:repoId", deleteRepo);

router.get("/:repoId/commits", getCommitsOfRepo);
router.get("/:repoId/commits/:commitId", getCommitOfRepo);
router.post("/:repoId/commits", postCommit);
router.put("/:repoId/commits/:commitId", putCommit);
router.delete("/:repoId/commits/:commitId", deleteCommit);

module.exports = router;