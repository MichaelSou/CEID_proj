const { Types } = require('mongoose')
const Visit = require('../models/visit')

exports.register_visit = async function(req, res) {
    var temp = ""
    var type = []
    for (i = 0; i<req.body.types.length; i++){
        if (req.body.types[i] != ","){
            temp = temp + req.body.types[i]
        }else{
            if(temp != "point_of_interest"){
                type.push(temp)
                temp = ""
            }
            else{temp = ""}
        }
    }

    if (req.body.ppl >= 0){
        var visit = new Visit({username: req.user.username, name: req.body.name, types: type, num_ppl: req.body.ppl})
        visit.save()
    }else{
        var visit = new Visit({username: req.user.username, name: req.body.name, types: type, num_ppl: null})
        visit.save()
    }
}