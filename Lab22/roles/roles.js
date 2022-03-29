const { defineAbility } = require("@casl/ability");

const guestAbility = defineAbility((can, cannot) => {
    can("read", ["ability", "repos", "commits"]);
});

const userAbility = ({ id }) => defineAbility((can, cannot) => {
    can("read", ["ability", "repos", "commits"]);
    can("read", "users", { id: id });

    can("create", "repos");
    can("create", "commits", { userId: id });
    can("update", "repos", { userId: id });
    can("update", "commits", { userId: id });
    can("delete", "repos", { userId: id });
    can("delete", "commits", { userId: id });
});

const adminAbility = defineAbility((can, cannot) => {
    can("read", "all");
    can("update", "all");
    can("delete", "all");
});

module.exports = {
    guestAbility,
    userAbility,
    adminAbility
}