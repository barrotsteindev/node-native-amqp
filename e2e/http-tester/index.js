const loadTest = require('loadtest').loadTest;
const maxRequests = process.argv[3] || 1000;
const url = process.argv[2] || 'localhost';
const httpUrl = `http://${url}:3000/`;
const testOptions = {
  'url': httpUrl,
  'maxRequests': maxRequests
};

loadTest(testOptions, (err, result) => {
  let timeElapsed = result.totalTimeSeconds;
  console.log(`took ${timeElapsed} to make ${maxRequests} requests`);
  console.dir(result, {depth: null, colors: true});
  if(err || Object.keys(result.errorCodes).length != 0) {
    console.log('Error codes: ' + JSON.stringify(result.errorCodes));
    if(err) {
      console.log(`error: ${err} occured`);
    }
    process.exit(1);
  }
  process.exit();
});
