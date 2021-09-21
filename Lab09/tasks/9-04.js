const http = require("http");

const params = JSON.stringify({
    __comment: "Запрос. Лабораторная работа 8/10",
    x: 1,
    y: 2,
    s: "Сообщение",
    m: ["a", "b", "c", "d"],
    o: {
        surname: "Иванов",
        name: "Иван"
    }
})
const options = {
    host: "localhost",
    port: 4000,
    path: `/task-4`,
    method: "POST",
    headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
}

const request = http.request(options,(response)=>
{
    console.log("StatusCode: ", response.statusCode);
    console.log("StatusMessage: ", response.statusMessage);

    let body = "";
    response.on("data",(data) => {
        body += data;
    });
    response.on("end",() => {
        console.log("Body response: ", JSON.parse(body));
    });
});

request.write(params);
request.end();