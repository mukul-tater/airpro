var express = require('express');
var app = express();
app.set('view engine', 'ejs');
app.use('/assets', express.static('assets'));
app.get('/',function(req, res){
     res.render('index');
 }
);
app.listen('3000',()=>console.log('server working on port 3000...'));
