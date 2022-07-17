const Visit = require('../models/visit')
const Util = require('../utils/utils')
const Case = require('../models/case')

exports.exposure = async function(req, res) {
    var exposure = [[]]
    var index = 0
    await Visit.find({$and: [{username: req.user.username}, {createdAt:{$gte:Util.add_days(new Date(), -7), $lt:new Date()}}]}).then(async (vs) => {
        if (vs != null){
            for (i=0; i<vs.length; i++){
                await Visit.find({$and: [{name: vs[i].name}, {createdAt:{$gte: Util.add_hours(vs[i].createdAt, -2), $lt: Util.add_hours(vs[i].createdAt, 2)}}]}).then(async(v) => {
                    if (v != null){
                        for (j=0; j<v.length; j++){
                            await Case.find({$and: [{username: v[j].username}, {diagnose_date:{$gte: v[j].createdAt, $lt: Util.add_days(v[j].createdAt, 7)}}]}).then((result) => {
                                if(result != ""){
                                    exposure[index] = []
                                    exposure[index][0]=vs[i].name
                                    exposure[index][1]=vs[i].createdAt
                                    index = index + 1
                                }
                            })
                        }
                    }
                })
            }
        }
        res.render('exposure/exposure', {exposure, title: "Potential exposure to covid in the last 7 days."})
    })
}