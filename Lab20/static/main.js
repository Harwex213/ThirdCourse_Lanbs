document.querySelector("#action-delete")?.addEventListener("click", async () => {
    await fetch("/Delete?id=" + document.querySelector("#action-delete").dataset.id, {
        method: "POST"
    });
    window.location.href = '/';
})