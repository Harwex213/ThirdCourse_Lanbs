const passport = require("passport");
const { BasicStrategy, DigestStrategy } = require("passport-http");
const users = require("../data-access/users.json");

const getUser = (username) => users.find(u => u.username === username);

const initBasic = () =>
    passport.use(new BasicStrategy((username, password, done) => {
        const user = getUser(username);
        if (!user) {
            done(null, false, { message: "Wrong username" });
        }
        if (user.password !== password) {
            done(null, false, { message: "Wrong password" });
        }

        done(null, user);
    }));
const initDigest = () =>
    passport.use(new DigestStrategy({ qop: "auth" },(username, done) => {
        const user = getUser(username);
        if (!user) {
            done(null, false);
        }

        done(null, user, user.password);
    }, (params, done) => {
        console.log("Params", params);
        done(null, true);
    }));

switch (process.argv[2]) {
    case "basic":
        initBasic();
        break;
    case "digest":
        initDigest();
        break
    default:
        initBasic();
        break;
}

passport.serializeUser((user, done) => {
    console.log("Serializing user");
    done(null, user);
});

passport.deserializeUser((user, done) => {
    console.log("Deserializing user");
    done(null, user);
});

module.exports = passport;