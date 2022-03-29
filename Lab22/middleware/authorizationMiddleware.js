const roles = require("../roles/roles");

const authorizationMiddleware = (request, response, next) => {
    switch (request.payload?.role) {
        case "user":
            request.ability = roles.userAbility({ id: request.payload.id });
            break;
        case "admin":
            request.ability = roles.adminAbility;
            break;
        default:
            request.ability = roles.guestAbility;
            break;
    }

    next();
}

module.exports = authorizationMiddleware;