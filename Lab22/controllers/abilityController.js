const express = require("express");

const getAbilities = (req, res) => {
    res.json(req.ability.rules);
}

const router = express.Router();

router.get("/", getAbilities);

module.exports = router;