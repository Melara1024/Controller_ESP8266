var joypad = document.getElementById('joypad');
var turnleft = document.getElementById('turnleft');
var turnright = document.getElementById('turnright');

joypad.addEventListener('touchstart', function(event) {
    joypad.style.backgroundColor = 'red';
    $.ajax({url: "/setLED?ox="+event.changedTouches[0].pageX, success: function(result){
    $("#div1").html(result);}});
    $.ajax({url: "/setLED?oy="+event.changedTouches[0].pageY, success: function(result){
    $("#div1").html(result);}});
}, false);
joypad.addEventListener('touchmove', function(event) {
    event.preventDefault();
    $.ajax({url: "/setLED?px="+event.changedTouches[0].pageX, success: function(result){
    $("#div1").html(result);}});
    $.ajax({url: "/setLED?py="+event.changedTouches[0].pageY, success: function(result){
    $("#div1").html(result);}});
}, false);
joypad.addEventListener('touchend', function(event) {
    joypad.style.backgroundColor = 'blue';
    $.ajax({url: "/setLED?ox="+0, success: function(result){
    $("#div1").html(result);}});
    $.ajax({url: "/setLED?oy="+0, success: function(result){
    $("#div1").html(result);}});
    $.ajax({url: "/setLED?px="+0, success: function(result){
    $("#div1").html(result);}});
    $.ajax({url: "/setLED?py="+0, success: function(result){
    $("#div1").html(result);}});
}, false);

turnleft.addEventListener('touchstart', function(event) {
    turnleft.style.backgroundColor = 'red';
    $.ajax({url: "/setLED?l="+1, success: function(result){
    $("#div1").html(result);}});
}, false);
turnleft.addEventListener('touchmove', function(event) {
    event.preventDefault();
}, false);
turnleft.addEventListener('touchend', function(event) {
    turnleft.style.backgroundColor = 'yellow';
    $.ajax({url: "/setLED?l="+0, success: function(result){
        $("#div1").html(result);}});
}, false);

turnright.addEventListener('touchstart', function(event) {
    turnright.style.backgroundColor = 'red';
    $.ajax({url: "/setLED?r="+1, success: function(result){
    $("#div1").html(result);}});
}, false);
turnright.addEventListener('touchmove', function(event) {
    event.preventDefault();
}, false);
turnright.addEventListener('touchend', function(event) {
    turnright.style.backgroundColor = 'orange';
    $.ajax({url: "/setLED?r="+0, success: function(result){
        $("#div1").html(result);}});
}, false);