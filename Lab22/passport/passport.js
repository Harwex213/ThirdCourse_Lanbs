const passport = require("passport");
const LocalStrategy = require("passport-local").Strategy;
const userService = require("../services/userService");

passport.serializeUser((user, done) => done(null, user));
passport.deserializeUser((user, done) => done(null, user));

passport.use(new LocalStrategy(async (username, password, done) =>
{
    const user = await userService.checkUserPassword(username, password);

    return user === null ? done(null, false, {
        message: "Wrong login or password"
    }) : done(null, user);
}))

module.exports = passport;