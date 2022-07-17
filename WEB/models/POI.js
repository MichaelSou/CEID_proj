const mongoose = require("mongoose");

const POISchema = new mongoose.Schema({
    "id": String ,
    "name": String ,"address": String ,
    "types":[],
    "coordinates":{"lat": Number  ,"lng": Number  },
    "rating":Number,
    "rating_n":Number,
    "populartimes":[
        {"name":String,"data":[]},
        {"name":String,"data":[]},
        {"name":String,"data":[]},
        {"name":String,"data":[]},
        {"name":String,"data":[]},
        {"name":String,"data":[]},
        {"name":String,"data":[]}]
})


module.exports = mongoose.model('POI', POISchema)