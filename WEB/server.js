if (process.env.NODE_ENV !== 'production'){
    require('dotenv').config()
}



// test script add users, visits, cases
const script = require('./utils/dataScript')

//script.init_users(100)
//script.add_visits(new Date("2022-02-01"), new Date("2022-02-20"), 3, 3, 25)
//script.add_cases(new Date("2022-02-01"), new Date("2022-02-20"), 20)




const passport =  require("passport")
const LocalStrategy = require("passport-local")


const express = require('express')
const expressLayouts = require('express-ejs-layouts')
const app = express()

const bodyParser = require('body-parser')
const mongoose = require("mongoose");
const User = require("./models/user");
const flash = require('express-flash')



var https = require('https');
var fs = require('fs');

var options = {
    key: fs.readFileSync('tst/key.pem'),
    cert: fs.readFileSync('tst/cert.pem')
  };


//require routes
const indexRouter = require('./routes/index')
const userRouter = require('./routes/users')
const mapRouter = require('./routes/map')
const caseRouter = require('./routes/case')
const exposureRouter = require('./routes/exposure')
const adminRouter = require('./routes/admin')

//connect to mongo
mongoose.connect(process.env.DATABASE_URL, { useNewUrlParser: true})
const db = mongoose.connection
app.use(require("express-session")({
    secret:"duck",//decode or encode session
        resave: false,          
        saveUninitialized:false    
    }));

db.on('error', error => console.error(error))
db.on('open', () => console.log('Connected to Mongoose'))

//passport
passport.serializeUser(User.serializeUser());       //session encoding
passport.deserializeUser(User.deserializeUser());   //session decoding
passport.use(new LocalStrategy(User.authenticate()));


app.set('view engine', 'ejs')
app.set('views', __dirname + '/views')
app.use(expressLayouts)
app.use(express.static('public'))
app.use(bodyParser.urlencoded({ limit: '10mb', extended: false }))
app.use(passport.initialize());
app.use(passport.session());



app.use(flash())
app.use((req, res, next) => {
    res.locals.errorMsg = req.flash("error");
    res.locals.user = req.user || null;
    res.locals.currentPath = req.path;
    next();
  }); 


app.use('/', checkLayout, indexRouter)
app.use('/users', checkLayout, userRouter)
app.use('/map', isLoggedIn, mapRouter)
app.use('/case',isLoggedIn, caseRouter)
app.use('/exposure',isLoggedIn, exposureRouter)
app.use('/admin',isAdmin, adminRouter)


app.get("/logout",isLoggedIn, (req,res, next)=>{
    req.logout();
    res.redirect("/");
    app.set('layout', 'layouts/layout')
    next()
});


function isAdmin(req, res, next) {
    if (req.isAuthenticated() && (req.user.isAdmin == true)) {
        checkLayout
        return next();
    }
    return res.redirect("/");
}

function isLoggedIn(req, res, next)
{
    if (req.isAuthenticated()) {
        checkLayout
        return next() 
    }
    else res.redirect('/')
}


function checkLayout(req, res, next)
{
    if (req.isAuthenticated() && (req.user.isAdmin == true)) {
        app.set('layout', 'layouts/layout_admin')
        return next() 
    }
    else if (req.isAuthenticated()) {
        app.set('layout', 'layouts/layout_logged')
        return next() 
    }
    else{
        app.set('layout', 'layouts/layout')
        return next() 
    }
}


// app.listen(process.env.PORT || 3000)


https.createServer(options, app).listen(3000);