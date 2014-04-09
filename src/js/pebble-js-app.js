
var options = JSON.parse(localStorage.getItem('options'));
//console.log('read options: ' + JSON.stringify(options));
if (options === null) options = { "language":100,
								  "LocalTime": 15,
								  "TZ1Name" : "Delhi",
								  "TZ1Time" : 22,
								  "TZ2Name" : "Buenos Aires",
								  "TZ2Time" : 11,
								  "TZ3Name" : "Chicago",
								  "TZ3Time" : 6,
								  "invert_color" : "false"};



//function getWeatherFromWoeid(woeid) {
function ReturnTimeZones() {

          Pebble.sendAppMessage({
			"language" : parseInt(options['language']),
			"LocalTime" : parseInt(options['LocalTime']),
			"TZ1Name" : options['TZ1Name'],
			"TZ1Time" : parseInt(options['TZ1Time']),
			"TZ2Name" : options['TZ2Name'],
			"TZ2Time" : parseInt(options['TZ2Time']),
			"TZ3Name" : options['TZ3Name'],
			"TZ3Time" : parseInt(options['TZ3Time']),
			"invert_color" : (options["invert_color"] == "true" ? 1 : 0),
          });

}

Pebble.addEventListener('showConfiguration', function(e) {
  var uri = 'http://dabdemon.github.io/WorldTime/settings2.html?' + //Here you need to enter your configuration webservice
	'language=' + encodeURIComponent(options['language']) +
	'&LocalTime=' + encodeURIComponent(options['LocalTime']) +
    '&TZ1Name=' + encodeURIComponent(options['TZ1Name']) +
    '&TZ1Time=' + encodeURIComponent(options['TZ1Time']) +
	'&TZ2Name=' + encodeURIComponent(options['TZ2Name']) +
    '&TZ2Time=' + encodeURIComponent(options['TZ2Time']) +
	'&TZ3Name=' + encodeURIComponent(options['TZ3Name']) +
    '&TZ3Time=' + encodeURIComponent(options['TZ3Time']) +
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
