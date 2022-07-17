const POI = require('../models/POI')
const Util = require('../utils/utils')
const Visit = require('../models/visit')



exports.send_map_data = async function(req, res) {
    var pois = []
    var numppl = []
    var numppl2 = []
    var numppl3 = []
    await POI.find({}).then((POI) => {
        pois = POI
    })
    
    for (a=0; a<pois.length; a++){
        numppl[a] = pois[a].name 
        numppl2[a] = 0 //number of people inserted by user
        numppl3[a] = 0 //number of insertions
    }
    for (j=0; j<pois.length; j++){
        await Visit.find({$and: [{name: pois[j].name}, {createdAt:{$gte:Util.add_hours(new Date(), -2), $lt:new Date()}}]}).then((pop) => {
            if (pop != null){
                    for(i=0; i<pop.length; i++){
                        if (pop[i].num_ppl != null){
                            var name = pop[i].name
                            var num = pop[i].num_ppl
                            indx = numppl.indexOf(name)
                            numppl3[indx] = numppl3[indx] + 1
                            numppl2[indx] = (numppl2[indx] + num) / numppl3[indx]
                        }
                    }
            }
        })
    }
    var popp = [[numppl], [numppl2]]
    res.render('map/map', {pois, numppl2})
}