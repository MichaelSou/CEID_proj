const User = require('../models/user')
const passwordValidator = require('password-validator');
const validator = require('validator')
const passport =  require("passport")
const Visit = require('../models/visit')
const Case = require('../models/case')

var schema = new passwordValidator();
schema
  .is().min(8)
  .is().max(25)
  .has().uppercase()
  .has().lowercase()
  .has().digits()
  .has().not().spaces() 
  .has().symbols();


exports.user_register = async function(req, res, next) {
    if (schema.validate(req.body.password) && validator.isEmail(req.body.email) && validator.matches(req.body.username, '^[a-zA-Z0-9_.-]*$')){
        User.register(new User({email: req.body.email, username: req.body.username}),req.body.password,function(err,user){
            if(err){
                console.log(err);
                if (err.code == 11000){
                    res.render("users/new", {errorMessage:"Email exists.", title: "Register"})
                }
                if ( err.type == "UserExistsError"){
                     res.render("users/new", {errorMessage:"Username exists.", title: "Register"})
                }
                else{res.render("users/new", {errorMessage: err, title: "Register"});}
            }
        passport.authenticate("local")(req,res,function(){
            res.redirect("/");
        })
        })
    }
    else{
        var errors
        if(!(schema.validate(req.body.password))){
            errors = "Invalid password. Password must be 8 characters long and contain at least: 1 lowercase, 1 upercase, 1 number, 1 symbol."
        }
        if(!(validator.isEmail(req.body.email))){
            (errors == null) ? errors =  "Invalid Email." : errors = " Invalid Email. " + errors
        }
        if (!(validator.matches(req.body.username, '^[a-zA-Z0-9_.-]*$'))){
            (errors == null) ? errors =  "Invalid username." : errors = " Invalid username. " + errors
        }
        res.render("users/new", {errorMessage : errors, title: "Register"})
    }
};

exports.user_update_cred = async function(req, res, next) {
    cvhistory = [[]]
    cvhistory[1] = []
    cvhistory[0][0] = "No registered visits"
    cvhistory[1][0] = "No registered cases"
    await Visit.find({username: req.user.username}).then((vis) => {
        if (vis != null){
            for (i=0; i<vis.length; i++){
                cvhistory[0][i] = vis[i].name +" "+ vis[i].createdAt
            }
        }
    })
    await Case.find({username: req.user.username}).then((cas) => {
        if (cas != ""){
            for (i=0; i<cas.length; i++){
                cvhistory[1][i] = cas[i].diagnose_date
            }
        }
    })


    if (req.body.username == "" && req.body.password == ""){
        res.render("users/userprofile", {message: "Both fields are empty.", cvhistory, title: "User Profile"}) }


    else if (req.body.username != ""){
        if (validator.matches(req.body.username, '^[a-zA-Z0-9_.-]*$')){
            User.find({username: req.body.username}).then((cs) => { 
                if (cs[0] == null) {
                    User.updateOne({username: req.user.username}, {$set: {username:req.body.username}}, function(err){
                        if (err) return res.send(500, {error: err});
                        res.render("users/userprofile", {message: "Username changed.", cvhistory, title: "User Profile"})
                })}
                else if (req.body.username == cs[0].username){
                    res.render("users/userprofile", {message: "Username already exists.", cvhistory, title: "User Profile"})
                }
            })
        }else{res.render("users/userprofile", {message: "Username must contain letters and numbers only.", cvhistory, title: "User Profile"})}
    }


    else if (req.body.password != ""){
        if (schema.validate(req.body.password)){
            User.find({username: req.user.username}).then((cs) => { 
                if (cs){
                    cs[0].setPassword(req.body.password, function(){
                        cs[0].save()
                        res.render("users/userprofile", {message: "Password changed.", cvhistory, title: "User Profile"})
        })}})
    }else { res.render("users/userprofile", {cvhistory, message: "Password must be 8 characters long and contain at least: 1 lowercase, 1 upercase, 1 number, 1 symbol.", title: "User Profile"})}
};
}


exports.cases_visits_history = async function(req, res, next){
    cvhistory = [[]]
    cvhistory[1] = []
    cvhistory[0][0] = "No registered visits"
    cvhistory[1][0] = "No registered cases"
    await Visit.find({username: req.user.username}).then((vis) => {
        if (vis != null){
            for (i=0; i<vis.length; i++){
                cvhistory[0][i] = vis[i].name +" "+ vis[i].createdAt
            }
        }
    })
    await Case.find({username: req.user.username}).then((cas) => {
        if (cas != ""){
            for (i=0; i<cas.length; i++){
                cvhistory[1][i] = cas[i].diagnose_date
            }
        }
    })
    return cvhistory, next()
}



exports.isLoggedIn = function(req, res, next) {
    if(req.isAuthenticated()){
        return next();
    }
    res.redirect("/");
};

