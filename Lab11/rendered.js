const crypto = require("crypto");

const onHash = () => {
    try {
        const startTime = performance.now();
        const hash = crypto.createHash("md5").update(document.querySelector("#data").value)
            .digest("hex");
        const endTime = performance.now();

        document.querySelector("#hash").innerHTML = hash +
            "<br>Length: " + hash.length +
            "<br>Exec time: " + (endTime - startTime);
    } catch (e) {
        document.querySelector("#error").innerHTML = e.message;
    }
}
