var express = require('express');
var app = express();
var path = require('path');


app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));

console.log(path.join(__dirname, 'public'));

app.get('/', function(req, res){ 
    res.render('index',{status:"Press Button To change Status of Led !!"});
});

app.listen(3000, function () {
    console.log('Simple LED Control Server Started on Port: 3000!')
});