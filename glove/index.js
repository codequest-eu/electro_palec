const express = require('express')
var app = express()
const path = require("path")
const http = require('http')
var lastClickDate = new Date('1999-01-01')
var delay = 5

const fingerPostOptions = {
    host: process.env.ELECTRO_PALEC_ADDRESS,
    path: '/',
    method: 'POST',
    headers: {
        'Authorization': 'Bearer DUPA.8'
    }
};

app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname+'/index.html'))
})

app.post('/electro', function (req, res) {
  var verificationDate = new Date();
  verificationDate.setSeconds(verificationDate.getSeconds()-delay)
  if  (verificationDate > lastClickDate) {
    var fingerPost = http.request(fingerPostOptions, function(res) {
        res.setEncoding('utf8');
        res.on('data', function (chunk) {
            console.log('Response: ' + chunk);
        });
    });
    fingerPost.end();
    res.redirect('/');
    lastClickDate = new Date();
  } else { res.redirect('/') }
})

app.listen(3000, function () {
  console.log('Electro palec listening on port 3000!')
})

