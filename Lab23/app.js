const express = require("express");
const passport = require("passport");
const VkStrategy = require("passport-vkontakte").Strategy;
const session = require("express-session")({
    resave: false,
    saveUninitialized: false,
    secret: "Super secret"
});

const app = express();

passport.use(new VkStrategy({
    clientID: "8093023",
    clientSecret: "b6vEdweRyYevM5wW2GRm",
    callbackURL: "http://localhost:3000/auth/vk/callback"
}, (accessToken, refreshToken, params, profile, done) => {
    return done(null, profile);
}));

passport.serializeUser((user, done) => done(null, user));
passport.deserializeUser((user, done) => done(null, user));

app.use(session);
app.use(passport.initialize());
app.use(passport.session());

app.get("/login", (req, res) =>
{
    res.sendFile(__dirname + "/login.html");
});

app.get("/auth/vk", passport.authenticate("vkontakte"));

app.get("/auth/vk/callback", passport.authenticate("vkontakte", {
    failureRedirect: "/login"
}), (req, res) => {
    res.redirect("/resource");
});

app.get("/resource", (req, res) =>
{
    if (req.user)
    {
        res.status(200).send(req.user);
    }
    else
    {
        res.redirect("/login");
    }
});

app.get("/logout", (req, res) =>
{
    req.logout();
    res.redirect("/login");
});

app.listen(3000, () => {
    console.log("server successfully started");
});
