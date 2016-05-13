// Ionic Starter App

// angular.module is a global place for creating, registering and retrieving Angular modules
// 'starter' is the name of this angular module example (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
angular.module('starter', ['ionic'])

.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    if(window.cordova && window.cordova.plugins.Keyboard) {
      // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
      // for form inputs)
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);

      // Don't remove this line unless you know what you are doing. It stops the viewport
      // from snapping when text inputs are focused. Ionic handles this internally for
      // a much nicer keyboard experience.
      cordova.plugins.Keyboard.disableScroll(true);
    }
    if(window.StatusBar) {
      StatusBar.styleDefault();
    }
  });
})
$(document).ready(function() {
  $('.loading').fadeIn();
  updateGUI();
  setTimeout(function() {
    $('.loading').fadeOut();
    $('body').fadeIn();
  }, 1000);
});
function pairHome() {
  var device_id = $('input.big-input').first().val();
  $.ajax({
    url: 'http://13.75.95.218/wowdoor/pair-device/' + device_id,
    method: 'GET',
    success: function(res) {
      if(!res) {
        swal({
          title: "Error!",
          text: "Your device id is not correct.",
          type: "error"
        });
      } else {
        localStorage.setItem('paired', true);
        localStorage.setItem('device_id', device_id);
        updateGUI();
      }
    },
    error: function(err) {
      swal({
        title: "Error!",
        text: "Your device id is not correct.",
        type: "error"
      });
    }
  });
}
function enterHome() {
  if(!localStorage.paired || !localStorage.device_id) {
    console.log("Device ID is not found in local storage, returning.");
    return;
  }
  $.ajax({
    url: 'http://13.75.95.218/wowdoor/request-door-open/'+localStorage.getItem('device_id'),
    method: 'GET',
    success: function(res) {
      swal({
        title: "Successful!",
        text: "Your door will open soon!",
        type: "success"
      });
    },
    error: function(err) {
      console.log(err);
      swal({
        title: "Error!",
        text: err.responseText,
        type: "error"
      });
    }
  });
}
function unpairHome() {
  $.ajax({
    url: 'http://13.75.95.218/wowdoor/unpair-device/' + localStorage.getItem('device_id'),
    method: 'GET',
    success: function(res) {
      if(res) {
        localStorage.removeItem('paired');
        localStorage.removeItem('device_id');
        updateGUI();
        swal({
          title: "Successful!",
          text: "Your device has been unpaired successfully.",
          type: "success"
        });
      } else {
        swal({
          title: "Error!",
          text: "Your device id is not correct.",
          type: "error"
        });
      }
    },
    error: function(err) {
      swal({
        title: "Error!",
        text: "Your device id is not correct.",
        type: "error"
      });
    }
  });
}
function updateGUI() {
  if(localStorage.paired) {
    $('.enter-your-home').removeClass('btn-disabled').click(enterHome);
    $('.unpair-your-home').fadeIn().click(unpairHome);
    console.log("UNBINDING!");
    $('.pair-your-home').unbind('click').removeClass('btn-orange').addClass('btn-green').html('<span><i class="fa fa-check"></i> You home is paired</span>');
  } else {
    $('.enter-your-home').addClass('btn-disabled').unbind('click');
    $('.unpair-your-home').fadeOut().unbind('click');
    $('.pair-your-home').addClass('btn-orange').removeClass('btn-green').html('<span><i class="fa fa-wifi"></i> Pair Your Home</span>').click(pairHome);
  }
}
