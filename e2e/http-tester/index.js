'use strict';

const loadTest = require('loadtest').loadTest;
const request = require('request');
const maxRequests = process.argv[3] || 1000;
const url = process.argv[2] || 'localhost';
const httpUrl = `http://${url}:3000/`;
console.log(`testing: ${httpUrl}`);
const testOptions = {
  'url': httpUrl,
  'maxRequests': maxRequests
};

function killApi() {
  console.log('killing');
  request.del(httpUrl, (error) => {
    if(error) {
      console.error(`could not close api ${error}`);
    }
    console.log('killed');
  });
}

loadTest(testOptions, (err, result) => {
  let timeElapsed = result.totalTimeSeconds;
  console.log(`took ${timeElapsed} to make ${maxRequests} requests`);
  console.dir(result, {depth: null, colors: true});
  if(err || Object.keys(result.errorCodes).length != 0) {
    console.log('Error codes: ' + JSON.stringify(result.errorCodes));
    if(err) {
      console.log(`error: ${err} occured`);
    }
    killApi();
    setTimeout(process.exit(1), 1000);
  }
  killApi();
  setTimeout(process.exit(), 1000);
});
