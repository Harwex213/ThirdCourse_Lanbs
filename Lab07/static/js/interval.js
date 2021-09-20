const bodyEl = document.querySelector("body");
const wrapperEl = document.createElement("div");

bodyEl.append(wrapperEl);
setInterval(() => {
    wrapperEl.innerHTML += "Hoba... <br>";
}, 5000);