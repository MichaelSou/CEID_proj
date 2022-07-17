const Visit = require('../models/visit')
const Case = require('../models/case')
const POIS = require('../models/POI')
const Util = require('../utils/utils')
const fs = require("fs");
const path = require("path");
const { type } = require('os');

exports.upload_data = async function(req, res){
    const absolutePath = path.join(__dirname, "..", req.file.path);
    const jsonString = fs.readFileSync(absolutePath, "utf-8");
    const jsonObject = JSON.parse(jsonString);
    fs.unlinkSync(absolutePath);

    for (i=0; i<jsonObject.length; i++){
        await POIS.deleteMany({name: jsonObject[i].name})
    }

    POIS.insertMany(jsonObject)
    res.render("admin/load_pois", {title: "Upload POI", message: "POIs inserted!"})
}

exports.delete_data = async function(req, res){
    await POIS.deleteMany({})
    res.render("admin/load_pois", {title: "Upload POI", message: "Deleted all POIs!"})
}


exports.search_stat = async function(req, res){
    var num_vis_by_case = 0
    //counts number of visists 
    var num_vis = await Visit.count({})
    var num_case = await Case.count({})

    await Case.find({}).then(async (cases) => {
        for(i=0; i<cases.length; i++){
            await Visit.count({$and: [{username: cases[i].username}, {createdAt: {$gte: Util.add_days(cases[i].diagnose_date, -7), $lt: Util.add_days(cases[i].diagnose_date, 14)}}]}).then((count) => {
                num_vis_by_case = num_vis_by_case + count
            })
        }
    })

    var rank_types_by_visit = await RankTypesByVisit()
    var rank_types_by_cases = await RankTypesByCases()

    var fgraph = await countOfVisitsPerDay(req.body.from, req.body.to)

    res.render("admin/view_data", {title: "Statistics", num_vis, num_case, num_vis_by_case, fgraph, rank_types_by_visit, rank_types_by_cases})
}

async function countOfVisitsPerDay(from, to){
    if (from == null){return}
    if (to == null){return}
    from = new Date(from)
    to = new Date(to)
    var day = []
    var count = []
    await Visit.aggregate([
        {$match:{
            "createdAt":{$gt:from, $lt: to}
        }},
        {$project:{
            "year":{$year:"$createdAt"}, 
            "month":{$month:"$createdAt"}, 
            "day": {$dayOfMonth:"$createdAt"}
        }},
        {$group:{
            _id:{year:"$year", month:"$month", day:"$day"}, 
            "count":{$sum:1}
      }}
    ]).then((res) => {
        for(i=0; i<res.length; i++){
            day[i] = res[i]._id.day.toString() +"/"+ res[i]._id.month.toString() + "/"+ res[i]._id.year.toString()
            count[i] = [res[i].count]
        }
    })
    return [day, count]


}

async function countOfVisitsofCasesPerDay(from, to){
    
}

async function changeOfVisitsPerhour(from, to){

}
async function changeOfVistisofCasesPerhour(from, to){

}



async function RankTypesByCases(){
    var types = []
    types = await Visit.find().distinct('types')
    var types_numofcases = []
    for(i=0; i<types.length; i++){
        types_numofcases[i] = 0
    }

    await Case.find({}).then(async (cases) => {
        for(i=0; i<cases.length; i++){
            await Visit.find({$and: [{username: cases[i].username}, {createdAt: {$gte: Util.add_days(cases[i].diagnose_date, -7), $lt: Util.add_days(cases[i].diagnose_date, 14)}}]}).then((vis) => {
                if(vis != null){
                    for(j=0; j<vis.length; j++){
                        for(k=0; k<vis[j].types.length; k++){
                            types_numofcases[types.indexOf(vis[j].types[k])] = types_numofcases[types.indexOf(vis[j].types[k])] + 1
                        }
                    }
                }
            })
        }
    })
    var rank_types_by_case = [[]]
    for (var i = 0; i < types.length; i++) {
        rank_types_by_case.push([types[i], types_numofcases[i]]);
    }

    rank_types_by_case.sort(sortFunction);

    function sortFunction(a, b) {
        if (a[0] === b[0]) {
            return 0;
        }
        else {
            return (a[0] > b[0]) ? -1 : 1;
        }
    }

    return rank_types_by_case
}




async function RankTypesByVisit(){
    var types = []
    var vis_type = []
    var types_count = []
    types = await Visit.find().distinct('types')
    for(k=0; k<types.length; k++){
        vis_type[k] = 0
        types_count[k] = await Visit.count({$and: [{types: types[k]}, {num_ppl: {$ne: null}}]})
    }
    await Visit.find({num_ppl: {$ne: null}}).then((ppl) => {
        if(ppl != null){
            for(j=0; j<ppl.length; j++){
                for(i=0; i<ppl[j].types.length; i++){
                    vis_type[types.indexOf(ppl[j].types[i])] = (vis_type[types.indexOf(ppl[j].types[i])] + ppl[j].num_ppl)
                }
            }
        }
    })
    for(i=0; i<vis_type.length; i++){
        vis_type[i] = vis_type[i] / types_count[i]
    }
    var rank_types_by_visit = [[]]
    for (var i = 0; i < types.length; i++) {
        rank_types_by_visit.push([types[i], vis_type[i]]);
    }

    rank_types_by_visit.sort(sortFunction);

    function sortFunction(a, b) {
        if (a[0] === b[0]) {
            return 0;
        }
        else {
            return (a[0] > b[0]) ? -1 : 1;
        }
    }
    return rank_types_by_visit
}