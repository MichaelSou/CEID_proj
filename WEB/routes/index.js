const express = require('express')
const router = express.Router()
const passport =  require("passport")

router.get('/', (req, res) => {
    res.render('index')
})

router.post("/",passport.authenticate("local",{failureRedirect:"/", failureFlash: true}),
    function(req, res) {
        if (req.isAuthenticated()){
            if(req.user.isAdmin == true)
            {
                res.redirect("/admin")
            }
            else{res.redirect("/map")}
        }
    }
)

module.exports = router