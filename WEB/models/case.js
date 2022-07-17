const mongoose = require("mongoose");

const CaseSchema = new mongoose.Schema({
    "username": String,
    "diagnose_date": Date
})


module.exports = mongoose.model('Case', CaseSchema)