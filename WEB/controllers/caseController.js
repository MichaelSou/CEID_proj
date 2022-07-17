const Case = require('../models/case')

exports.register_case = function(req, res) {
    var d_date = new Date(req.body.date)
    console.log(d_date)
    if (d_date == "Invalid Date"){
        res.render("case/case", {message:"You must insert a valid date.", title: "Register Case"})
        return
    }
    Case.find({username: req.user.username}).sort({_id: -1}).limit(1).then((cs) => {
        if (cs[0] == null){
            var ncase = new Case({username: req.user.username, diagnose_date: req.body.date})
            ncase.save()
            res.render("case/case", {message:"Succsesful case registration.", title: "Register Case"})
        }
        else{
            var r_date = new Date(cs[0].diagnose_date.setDate(cs[0].diagnose_date.getDate()+14))
            console.log(r_date)
            console.log(d_date)
            if (d_date > r_date){
                var ncase = new Case({username: req.user.username, diagnose_date: req.body.date})
                ncase.save()
                res.render("case/case", {message: "Succsesful case registration.", title: "Register Case"})
            }
            else{
                res.render("case/case", {message:"You already have registered a case in the last 14-days.", title: "Register Case"})
            }
        }
    })
}
