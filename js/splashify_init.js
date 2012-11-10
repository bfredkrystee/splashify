jQuery(document).ready(function($) {
  var splash = $.jStorage.get("splash", 0);
  var splashalways = Drupal.settings.splashify.js_splash_always;
  var now = new Date();
  var nowtimeSeconds = now.getTime() / 1000;

  var referrer = document.referrer + '';
  var hostname = window.location.hostname + '';
  if (referrer.search(hostname) != -1) {
    // This page was loaded from an internal page. Do not show splash.
    return;
  }

  // Display the splash page?
  if(!splash || splash < nowtimeSeconds || splashalways=='1'){
    var expireAfter = Drupal.settings.splashify.js_expire_after;
    var last_url = $.jStorage.get('splashlasturl', '');
    var what_urls = Drupal.settings.splashify.js_mode_settings.urls;

    // Set when the splash variable should expire next.
    $.jStorage.set("splash", nowtimeSeconds + expireAfter);

    var url = '';

    // Determine the url we are working with, which is based on the mode.
    if(Drupal.settings.splashify.js_mode_settings.system_splash != ''){
      // Display the system splash page.
      url = Drupal.settings.splashify.js_mode_settings.system_splash;
    } else if(Drupal.settings.splashify.js_mode_settings.mode == 'sequence'){
      // Display the splash pages in sequence.
      var new_url_index = 0;
      var last_url_index = jQuery.inArray(last_url, what_urls);
      if(last_url_index > -1 && last_url_index+1 < Drupal.settings.splashify.js_mode_settings.total_urls){
        new_url_index = last_url_index + 1;
      }
      url = what_urls[new_url_index];
    } else if(Drupal.settings.splashify.js_mode_settings.mode == 'random'){
      // Display a random splash page.
      var new_url_index = Math.floor(Math.random() * Drupal.settings.splashify.js_mode_settings.total_urls);
      url = what_urls[new_url_index];
    }

    $.jStorage.set('splashlasturl', url);

    // Display the splash page.
    if(Drupal.settings.splashify.js_mode == 'redirect'){
      // Redirect.
      window.location.replace(url);
    } else if(Drupal.settings.splashify.js_mode == 'colorbox'){
      // Open a ColorBox.
      $.colorbox({
        transition:'elastic',
        iframe:true,
        href:url,
        width:Drupal.settings.splashify.js_mode_settings.size_width,
        height:Drupal.settings.splashify.js_mode_settings.size_height
      });
    } else if(Drupal.settings.splashify.js_mode == 'window'){
      // Open a popup window.
      window.open(url, 'splash', Drupal.settings.splashify.js_mode_settings.size);
    }
  }
});
