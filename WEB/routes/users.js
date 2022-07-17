const express = require('express')
const router = express.Router()
var user_controller = require('../controllers/userController')



router.get("/userprofile",user_controller.isLoggedIn, user_controller.cases_visits_history ,(req,res) =>{
    res.render("users/userprofile", {cvhistory, title: "User Profile"});
})

router.post("/userprofile", user_controller.isLoggedIn, user_controller.user_update_cred );


router.get('/new', (req, res) => {
    res.render('users/new', {title: "Register"})
})

router.post("/new", user_controller.user_register);


module.exports = router