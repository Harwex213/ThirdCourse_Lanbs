const mongoose = require("mongoose");

const facultySchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    faculty: {
        type: String,
        unique: true
    },
    faculty_name: {
        type: String
    }
});

module.exports = mongoose.model("faculty", facultySchema);