const fs = require("fs");
const errorResponse = require("../errorResponse");
const { dataDestination } = require("./../const");

const readData = () => fs.readFileSync(dataDestination);
const saveData = (data) => fs.writeFileSync(dataDestination, data);

const successResponse = (response, data) => {
    response.writeHead(400, {
        "Content-Type" : "application/json"
    })
    response.end(JSON.stringify(data));
}

const getCurrentData = () => {
    return JSON.parse(readData().toString());
}

const getIdOfRequest = (requestUrl) => {
    const id = requestUrl.pathname.match(/\/[0-9]+/gi);
    if (id) {
        return id[0].substring(1);
    }
    else {
        throw new Error("Check your request pathname");
    }
}

const getMethod = (connectionInfo) => {
    const { response, routePaths, requestUrl } = connectionInfo;

    if (routePaths.length === 1) {
        if (requestUrl.pathname.length > 1) {
            const studentId = getIdOfRequest(requestUrl);
            const student = getCurrentData().find(student => Number(student.id) === Number(studentId));
            if (student) {
                successResponse(response, student);
                return;
            }
            else {
                throw new Error("Student with such id is not found");
            }
        }

        successResponse(response, getCurrentData());
    }
    else {
        throw new Error("Check your request pathname");
    }
};

const postMethod = (connectionInfo) => {
    const { request, response } = connectionInfo;

    let newStudent;
    request.on("data", (data) => {
        newStudent = JSON.parse(data);
    })
    request.on("end", () => {
        const students = getCurrentData();
        const possibleStudent = students.find(student => Number(student.id) === Number(newStudent.id));
        if (possibleStudent) {
            errorResponse(response, {
                message: "Student with such id is exist"
            });
            return;
        }

        students.unshift(newStudent);
        saveData(JSON.stringify(students));
        successResponse(response, newStudent);
    })
};

const putMethod = (connectionInfo) => {
    const { request, response } = connectionInfo;

    let updatedStudent;
    request.on("data", (data) => {
        updatedStudent = JSON.parse(data);
    })
    request.on("end", () => {
        const students = getCurrentData();
        const studentIndex = students.findIndex(student => Number(student.id) === Number(updatedStudent.id));
        if (studentIndex === -1) {
            errorResponse(response, {
                message: "Student with such id is not found"
            });
            return;
        }

        students[studentIndex] = updatedStudent;
        saveData(JSON.stringify(students));
        successResponse(response, updatedStudent);
    })
};

const deleteMethod = (connectionInfo) => {
    const { response, routePaths, requestUrl } = connectionInfo;

    if (routePaths.length === 1 && requestUrl.pathname.length > 1) {
        const students = getCurrentData();
        const studentId = getIdOfRequest(requestUrl);
        const studentIndex = students.findIndex(student => Number(student.id) === Number(studentId));
        if (studentIndex === -1) {
            errorResponse(response, {
                message: "Student with such id is not found"
            });
            return;
        }

        const deletedStudent = students[studentIndex];
        students.splice(studentIndex, 1);
        saveData(JSON.stringify(students));
        successResponse(response, deletedStudent);
    }
    else {
        throw new Error("Check your request pathname");
    }
};

const methods = {
    "GET": getMethod,
    "POST": postMethod,
    "PUT": putMethod,
    "DELETE": deleteMethod
};

module.exports = methods;