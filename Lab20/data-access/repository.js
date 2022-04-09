const directory = require("./directory.json");
const fs = require("fs");
const crypto = require("crypto");
// explanation: https://gist.github.com/gordonbrander/2230317?permalink_comment_id=3443509#gistcomment-3443509
const uid = () => {
    let a = new Uint32Array(3);
    crypto.webcrypto.getRandomValues(a);
    return (performance.now().toString(36)+Array.from(a)
        .map(A => A.toString(36)).join("")).replace(/\./g,"");
};

const saveState = () => fs.writeFileSync(__dirname + "/directory.json", JSON.stringify(directory));

exports.getRecords = () => directory;
exports.addRecord = ({ fullName, phoneNumber }) => {
    const newRecord = {
        id: uid(),
        fullName,
        phoneNumber
    };
    directory.push(newRecord);
    saveState();
    return newRecord;
}
exports.updateRecord = ({ id, fullName, phoneNumber }) => {
    const record = directory.find(record => record.id === id);
    if (!record) {
        return null;
    }
    record.fullName = fullName;
    record.phoneNumber = phoneNumber;
    saveState();
    return record;
}
exports.deleteRecord = ({id}) => {
    const recordIndex = directory.findIndex(record => record.id === id);
    if (recordIndex === -1) {
        return false;
    }
    directory.splice(recordIndex, 1);
    saveState();
    return true;
}