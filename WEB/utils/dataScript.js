const User = require('../models/user')
const Visit = require('../models/visit')
const Case = require('../models/case')
const POI = require('../models/POI')
const Util = require('./utils')

exports.init_users = async function(N) {
    for (i=0; i<N; i++){
        usrname = "User" + i
        mail = usrname+"@email.com"
        pass = "user" + i + "pass"
        await User.register(new User({email: mail, username: usrname}),pass,function(err,user){})
    }
}

exports.add_visits = async function(start_date, end_date, num_of_vis_per_day, min_numofppl, max_numofppl){
    await Visit.deleteMany({})
    var pois = []
    var users = []
    pois = await POI.find({})
    users = await User.find({})
    var Difference_In_Time = end_date.getTime() - start_date.getTime();
    var Difference_In_Days = Difference_In_Time / (1000 * 3600 * 24);
    for(k=0; k<Difference_In_Days; k++){
        var date1 = Util.add_days(start_date, k)
        
        for(i=0; i<users.length; i++){
            var user = users[getRndInteger(0, users.length-1)]
            for(j=0; j<num_of_vis_per_day; j++){
                var date2 = Util.add_hours(date1, getRndInteger(0, 23))
                date2 = new Date(date2).toISOString()
                var num_of_ppl = getRndInteger(min_numofppl, max_numofppl)
                var poi = pois[getRndInteger(0, pois.length-1)]
                var visit = new Visit({username: user.username, name: poi.name, types: poi.types, num_ppl: num_of_ppl, createdAt: date2})
                await visit.save()
            }
        }
    }
}

exports.add_cases = async function(start_date, end_date, num_of_cases){
    await Case.deleteMany({})
    var users = []
    users = await User.find({})
    for(i=0; i<num_of_cases; i++){
        var user = users[getRndInteger(0, users.length-1)]
        var date = randomDate(start_date, end_date)
        var ncase = new Case({username: user.username, diagnose_date: date})
        await ncase.save()
    }
}


function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
  }


function randomDate(start, end) {
    return new Date(start.getTime() + Math.random() * (end.getTime() - start.getTime()));
  }