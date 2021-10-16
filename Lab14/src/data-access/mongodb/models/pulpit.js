const mongoose = require("mongoose");

const facultySchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    pulpit: {
        type: String,
        unique: true
    },
    pulpit_name: {
        type: String
    },
    faculty: {
        type: String,
        ref: "faculty"
    }
});

module.exports = mongoose.model("pulpit", facultySchema);