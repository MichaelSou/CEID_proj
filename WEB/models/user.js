const mongoose = require("mongoose");
const passportLocalMongoose = require("passport-local-mongoose");

var options = {
    errorMessages: {
        MissingPasswordError: 'No password was given',
        AttemptTooSoonError: 'Account is currently locked. Try again later',
        TooManyAttemptsError: 'Account locked due to too many failed login attempts',
        NoSaltValueStoredError: 'Authentication not possible. No salt value stored',
        IncorrectPasswordError: 'Password or username are incorrect',
        IncorrectUsernameError: 'Password or username are incorrect',
        MissingUsernameError: 'No username was given',
        UserExistsError: 'A user with the given username is already registered'
    }
};



const userSchema = new mongoose.Schema({
    email: {
        type: String,
        required: true,
        unique: true
    },
    username: {
        type: String,
        required : true,
        unique: 'That username is already taken'
    },
    isAdmin: {
        type: Boolean,
        default: 0

        }
})

userSchema.plugin(passportLocalMongoose, options);




module.exports = mongoose.model('User', userSchema)