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

app.post('/slack_hook', function (req, res) {
  var bodyStr = '';
  req.on("data",function(chunk){
    bodyStr += chunk.toString();
  });
  req.on("end",function(){
    if (bodyStr.indexOf(process.env.SLACK_TOKEN) > -1) {
      moveFigner();
      if (bodyStr.indexOf('przemek') > -1) {
        res.json(200, {text: '🖕🖕🖕🖕🖕🖕🖕', username: 'Elektropalec'});
      } else {
        res.json(200, {text: 'Brawo! Drzwi otworzone!', username: 'Elektropalec'});
      }
      res.end();
    }
  });
})

app.post('/electro', function (req, res) {
  moveFigner();
  res.redirect('/');
})

app.listen(3000, function () {
  console.log('Electro palec listening on port 3000!')
})

moveFigner = function() {
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
    lastClickDate = new Date();
  }
}

