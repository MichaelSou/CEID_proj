const mongoose = require("mongoose");

const VisitSchema = new mongoose.Schema({
    "username": String,
    "name": String ,
    "types": [{type: String}],
    "num_ppl":Number,
    "createdAt": {type: Date, default: new Date().toISOString()}
})


module.exports = mongoose.model('Visit', VisitSchema)