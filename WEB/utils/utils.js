exports.add_hours = function(date, hours) {
    return new Date(new Date(date).setHours(date.getHours() + hours));
}


exports.add_days = function(date, days) {
    var result = new Date(date);
    result.setDate(result.getDate() + days);
    return result;
}

