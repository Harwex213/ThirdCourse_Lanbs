const express = require("express");
const { getUserById, getAllUsers } = require("../services/userService");
const { generateForbidden } = require("../errors/baseErrors");
const {subject} = require("@casl/ability");

const getUsers = async (req, res, next) => {
    if (req.ability.can("read", subject("users")) === false) {
        next(generateForbidden());
        return;
    }

    res.json(await getAllUsers())
};

const getUser = async (req, res, next) => {
    if (req.ability.can("read", subject("users", { id: parseInt(req.params.userId) })) === false) {
        next(generateForbidden());
        return;
    }

    res.json(await getUserById({ id: req.payload.id }));
}

const router = express.Router();

router.get("/", getUsers);
router.get("/:userId", getUser);

module.exports = router;