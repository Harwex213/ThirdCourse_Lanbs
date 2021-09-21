const postJson = (request, response) => {
    let body;
    request.on("data", data => {
        body = JSON.parse(data);
    })
    request.on("end", () => {
        response.end(JSON.stringify({
            "__comment": "Ответ. Лабораторная работа 8/10",
            "x_plus_y": body.x + body.y,
            "Concatination_s_o": `${body.s}: ${body.o.surname}, ${body.o.name}`,
            "Length_m": body.m.length,
        }));
    })
}

module.exports = {
    postJson
}