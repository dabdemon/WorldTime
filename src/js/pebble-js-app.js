
var options = JSON.parse(localStorage.getItem('options'));
//console.log('read options: ' + JSON.stringify(options));
if (options === null) options = { "TZ1Name" : "Gurgaon",
								  "TZ1Hour" : 4,
								  "TZ1Minute": 30,
								  "TZ2Name" : "Buenos Aires",
								  "TZ2Hour" : -4,
								  "TZ2Minute": 0,
								  "TZ3Name" : "Chicago",
								  "TZ3Hour" : -7,
								  "TZ3Minute": 0,
								 // "TZ4Name" : "San Francisco",
								 // "TZ4Hour" : -9,
								 // "TZ4Minute": 0,
								  "invert_color" : "false"};



//function getWeatherFromWoeid(woeid) {
function ReturnTimeZones() {

          Pebble.sendAppMessage({
			"TZ1Name" : options['TZ1Name'],
			"TZ1Hour" : parseInt(options['TZ1Hour']),
			"TZ1Min": parseInt(options['TZ1Minute']),
			"TZ2Name" : options['TZ2Name'],
			"TZ2Hour" : parseInt(options['TZ2Hour']),
			"TZ2Min": parseInt(options['TZ2Minute']),
			"TZ3Name" : options['TZ3Name'],
			"TZ3Hour" : parseInt(options['TZ3Hour']),
			"TZ3Min": parseInt(options['TZ3Minute']),
			//"TZ4Name" : options['TZ4Name'],
			//"TZ4Hour" : parseInt(options['TZ4Hour']),
			//"TZ4Min": parseInt(options['TZ4Minute']),
			 "invert_color" : (options["invert_color"] == "true" ? 1 : 0),
          });

}

Pebble.addEventListener('showConfiguration', function(e) {
  var uri = 'http://dabdemon.github.io/WorldTime/settings.htm?' + //Here you need to enter your configuration webservice
    'TZ1Name=' + encodeURIComponent(options['TZ1Name']) +
    '&TZ1Hour=' + encodeURIComponent(options['TZ1Hour']) +
    '&TZ1Minute=' + encodeURIComponent(options['TZ1Minute']) +
	'&TZ2Name=' + encodeURIComponent(options['TZ2Name']) +
    '&TZ2Hour=' + encodeURIComponent(options['TZ2Hour']) +
    '&TZ2Minute=' + encodeURIComponent(options['TZ2Minute']) +
	'&TZ3Name=' + encodeURIComponent(options['TZ3Name']) +
    '&TZ3Hour=' + encodeURIComponent(options['TZ3Hour']) +
    '&TZ3Minute=' + encodeURIComponent(options['TZ3Minute']) +
	//'&TZ4Name=' + encodeURIComponent(options['TZ4Name']) +
    //'&TZ4Hour=' + encodeURIComponent(options['TZ4Hour']) +
    //'&TZ4Minute=' + encodeURIComponent(options['TZ4Minute']) +
    '&invert_color=' + encodeURIComponent(options['invert_color']);

	//console.log('showing configuration at uri: ' + uri);

  Pebble.openURL(uri);
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e.response) {
    options = JSON.parse(decodeURIComponent(e.response));
    localStorage.setItem('options', JSON.stringify(options));
    //console.log('storing options: ' + JSON.stringify(options));
    ReturnTimeZones();
  } else {
    //console.log('no options received');
  }
});


//Initiate the Appsync (This event is called just once)
Pebble.addEventListener("ready", function(e) {
  //console.log("connect!" + e.ready);
  ReturnTimeZones();

});
